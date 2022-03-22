package main

import (
	"flag"
	"fmt"
	"math"
	"os"
	"os/exec"
	"strconv"
	"strings"
	"time"
)

const (
	quit = iota
	reset
	pause
	clear
	nop
	help
)

var (
	base                 time.Time
	until                time.Time
	pauseStart           time.Time
	pausedFor            time.Duration
	hour, min, sec, msec int64 = 0, 0, 0, 0
	paused               bool  = false
	timer                bool  = false
)

func setupTerminal() {
	// disable input buffering
	exec.Command("stty", "-F", "/dev/tty", "cbreak", "min", "1").Run()
	// do not display entered characters on the screen
	exec.Command("stty", "-F", "/dev/tty", "-echo").Run()
}

func setupDuration(i string, f string) bool {
	s := strings.Split(i, ":")
	if len(s) < 4 {
		fmt.Fprintf(os.Stderr, "stopwatch: not enough flags to -%s: %q\nExpected format: hh:mm:ss:ms\n", f, i)
		return false
	}

	hour, _ = strconv.ParseInt(s[0], 10, 32)
	min, _ = strconv.ParseInt(s[1], 10, 32)
	sec, _ = strconv.ParseInt(s[2], 10, 32)
	msec, _ = strconv.ParseInt(s[3], 10, 32)

	min = int64(math.Min(float64(min), 59))
	sec = int64(math.Min(float64(sec), 59))
	msec = int64(math.Min(float64(msec), 999))

	return true
}

func formatDuration(d time.Duration) (h int64, m int64, s int64, ms int64) {
	total := d.Nanoseconds()

	h = total / int64(time.Hour)
	hmod := total % int64(time.Hour)

	m = hmod / int64(time.Minute)
	mmod := hmod % int64(time.Minute)

	s = mmod / int64(time.Second)
	smod := mmod % int64(time.Second)

	ms = smod / int64(time.Millisecond)

	return
}

func setupFlags() bool {
	var start time.Duration
	var upto time.Duration

	flag.BoolVar(&paused, "p", false, "Start paused")
	flag.DurationVar(&start, "t", 0, "Start time")
	flag.DurationVar(&upto, "c", 0, "Run in timer mode, counting from...")

	flag.Parse()

	base = base.Add(-start)
	if paused {
		pauseStart = time.Now()
	}
	if upto > 0 {
		timer = true
		until = base.Add(upto)
	}

	return true
}

func wait(c chan bool) {
	time.Sleep(time.Millisecond)

	c <- true
}

func inputLoop(c chan int) {
	for {
		var i rune
		var in [1]byte
		_, err := os.Stdin.Read(in[:])

		i = rune(in[0])

		if err != nil {
			os.Exit(1)
		}

		switch i {
		case 'q':
			c <- quit
		case 'c':
			c <- reset
		case 'l':
			c <- clear
		case '\f':
			c <- clear
		case '\n':
			c <- nop
		case ' ':
			c <- pause
		case 'h':
			c <- help
		case '?':
			c <- help
		default:
			fmt.Printf("\nUnknown command: %q\n", i)

		}
	}
}

func countLoop() {
	now := time.Now()
	dur := now.Sub(base.Add(pausedFor))
	hour, min, sec, msec = formatDuration(dur)
}

func timerLoop() {
	now := time.Now()
	dur := until.Sub(now.Add(-pausedFor))

	if dur.Milliseconds() == 0 {
		fmt.Print("\a\n")

		cols := [2]string{"\x1b[31m", "\x1b[0m"}
		for i := 0; i < 5; i++ {
			fmt.Printf("\r%sTimer up!", cols[i%2])
			time.Sleep(500 * time.Millisecond)
		}

		fmt.Println()
		os.Exit(0)
	}

	hour, min, sec, msec = formatDuration(dur)
}

func main() {
	inter := make(chan int)
	waiter := make(chan bool)
	base = time.Now()

	setupTerminal()

	if !setupFlags() {
		return
	}

	go inputLoop(inter)

	for {
		go wait(waiter)

	waiting:
		for {
			var action int
			select {

			case action = <-inter:
				switch action {
				case quit:
					fmt.Print("\n")
					return
				case pause:
					paused = !paused

					if paused {
						pauseStart = time.Now()
						fmt.Println("\nPaused...")
					} else {
						pausedFor += time.Now().Sub(pauseStart)
						fmt.Println("Unpaused...")
					}
				case reset:
					base = time.Now()
					pauseStart = time.Now()
					pausedFor = 0
					hour, min, sec, msec = 0, 0, 0, 0
					fmt.Println("\nTimer reset")
				case clear:
					os.Stdout.Write([]byte("[H[2J")) // Should work for most ANSII terminals
				case nop:
					fmt.Print("\n")
				case help:
					fmt.Println(`
Available actions:
	h: help menu
	?: help menu
	q: quit
	c: clear/reset timer
	Control-l/l: clear/reset screen
	<space>: pause timer

Any keybinding can be pressed at any time and will take effect immediately`)
				default:
					fmt.Println("\nWARNING: Unknown command")
				}
				continue
			case <-waiter:
				break waiting

			default:
				continue

			}
		}

		if !paused {
			if timer {
				timerLoop()
			} else {
				countLoop()
			}

			fmt.Printf("\r%02d:%02d:%02d:%04d", hour, min, sec, msec)
		}
	}
}
