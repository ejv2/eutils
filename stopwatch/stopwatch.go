package main

import (
	"fmt"
	"os"
	"os/exec"
	"time"
	"flag"
	"strings"
	"strconv"
)

const (
	quit = iota
	reset
	pause
	clear
	help
)

var (
	hour, min, sec, msec int64 = 0, 0, 0, 0
	paused bool = false
)


func setupTerminal() {
	// disable input buffering
	exec.Command("stty", "-F", "/dev/tty", "cbreak", "min", "1").Run()
	// do not display entered characters on the screen
	exec.Command("stty", "-F", "/dev/tty", "-echo").Run()
}

func setupFlags() bool {
	var starttime string = ""

	flag.BoolVar(&paused, "p", false, "Start paused")
	flag.StringVar(&starttime, "t", "", "Start time")

	flag.Parse()

	if starttime != "" {
		s := strings.Split(starttime, ":")
		if len(s) < 4 {
			fmt.Fprintf(os.Stderr, "stopwatch: not enough flags to -t: %q\n", starttime)
			return false
		}

		hour, _ = strconv.ParseInt(s[0], 10, 32)
		min, _  = strconv.ParseInt(s[1], 10, 32)
		sec, _  = strconv.ParseInt(s[2], 10, 32)
		msec, _  = strconv.ParseInt(s[3], 10, 32)
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
		case ' ':
			c <- pause
		case 'h' :
			c <- help
		case '?':
			c <- help
		default:
			fmt.Printf("\nUnknown command: %q\n", i)

		}
	}
}

func main() {
	inter := make(chan int)
	waiter := make(chan bool)

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
						fmt.Println("\nPaused...")
					} else {
						fmt.Println("Unpaused...")
					}
				case reset:
					hour, min, sec, msec = 0, 0, 0, 0
					fmt.Println("\nTimer reset")
				case clear:
					os.Stdout.Write([]byte("[H[2J")) // Should work for most ANSII terminals
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
			if msec+1 >= 1000 {
				sec++
				msec = 0
			} else {
				msec++
			}

			if sec >= 60 {
				min++
				sec = 0
				msec = 0

			}

			if min >= 60 {
				hour++
				min = 0
				sec = 0
				msec = 0
			}


			fmt.Printf("\r%02d:%02d:%02d:%04d", hour, min, sec, msec)
		}
	}
}
