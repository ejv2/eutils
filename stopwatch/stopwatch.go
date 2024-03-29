package main

import (
	"flag"
	"fmt"
	"os"
	"os/exec"
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
	base, until          time.Time
	pauseStart           time.Time
	pausedFor            time.Duration
	hour, min, sec, msec int64 = 0, 0, 0, 0
	paused                     = false
	timer                      = false
)

func setupTerminal() {
	// disable input buffering
	exec.Command("stty", "-F", "/dev/tty", "cbreak", "min", "1").Run()
	// do not display entered characters on the screen
	exec.Command("stty", "-F", "/dev/tty", "-echo", "-isig").Run()
}

func resetTerminal() {
	// enable input buffering
	exec.Command("stty", "-F", "/dev/tty", "icanon").Run()
	// re-display entered characters on the screen
	exec.Command("stty", "-F", "/dev/tty", "echo", "isig").Run()
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
	if upto > 0 {
		timer = true
		until = base.Add(upto)
	}

	return true
}

func wait(c chan bool, unpaused chan int) {
	time.Sleep(time.Millisecond)
	<-unpaused

	c <- true
}

func inputLoop(c chan int) {
	buf := make([]byte, 4)
	for {
		i, err := os.Stdin.Read(buf[:])
		str := string(buf[:i])
		rs := []rune(str)

		if err != nil {
			os.Exit(1)
		}

		for _, r := range rs {
			switch r {
			case 'q':
				c <- quit
			case '\x03': // Ctrl-C
				c <- quit
			case '\x04': // Ctrl-D
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
				fmt.Printf("\nUnknown command: %q\n", r)

			}
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

		fmt.Println(cols[1])
		os.Exit(0)
	}

	hour, min, sec, msec = formatDuration(dur)
}

func main() {
	inter := make(chan int)
	waiter := make(chan bool)
	unpaused := make(chan int, 1)
	base = time.Now()
	pauseStart = time.Now()

	setupTerminal()
	defer resetTerminal()

	if !setupFlags() {
		return
	}

	go inputLoop(inter)

	for {
		go wait(waiter, unpaused)
		if !paused {
			unpaused <- 1
		}

	waiting:
		for {
			select {

			case action := <-inter:
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
						pausedFor += time.Since(pauseStart)
						unpaused <- 1
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
					panic("invalid event loop message")
				}
				continue
			case <-waiter:
				break waiting

			}
		}

		if !paused {
			if timer {
				timerLoop()
			} else {
				countLoop()
			}
			fmt.Printf("\r%02d:%02d:%02d:%03d", hour, min, sec, msec)
		}
	}
}
