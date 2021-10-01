package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
	"time"
)

const (
	colorRed    = "\x1b[31m"
	colorBlue   = "\x1b[34m"
	colorYellow = "\x1b[33m"
	colorReset  = "\x1b[0m"
)

var (
	entered   string = ""
	processed string = ""
	spinner   rune

	mistakes int = 0
	total    int = 0
)

func inputLoop(done chan bool, expected string) {
	in := bufio.NewScanner(os.Stdin)
	in.Split(bufio.ScanRunes)

	for {
		success := in.Scan()
		text := in.Text()
		if !success {
			fmt.Println("IO Error")
			os.Exit(1)
		}

		if text == "\n" {
			continue
		}

		entered += text

		if entered[len(entered)-1] != expected[len(entered)-1] {
			processed += colorRed + string(expected[len(entered)-1]) + colorReset
			mistakes++
		} else {
			processed += colorBlue + text + colorReset
		}

		total++

		if len(entered) == len(expected) {
			break
		}
	}

	done <- true
}

func spinnerLoop(set []rune, stop chan bool) {
	spinner = set[0]
	i := 0

loop:
	for {
		if i >= len(set)-1 {
			i = 0
		} else {
			i++
		}

		spinner = set[i]

		select {
		case <-stop:
			break loop
		default:
			time.Sleep(time.Millisecond * 100)
		}
	}
}

func ResetState() {
	entered = ""
	processed = ""
}

func TimeInput(expected string) int64 {
	ClearScreen()
	start := time.Now()

	spinners := []rune{'⣾', '⣽', '⣻', '⢿', '⡿', '⣟', '⣯', '⣷'}
	taken := time.Since(start)

	done := make(chan bool)
	stop := make(chan bool)

	go inputLoop(done, expected)
	go spinnerLoop(spinners, stop)

loop:
	for {
		ClearScreen()

		fmt.Printf("%s%s\n\n", processed, expected[len(entered):])

		msec := int(taken.Milliseconds()) - (int(taken.Seconds()) * 1000)
		wc := len(strings.Split(expected[:len(entered)], " ")) - 1

		cpm := float64(len(entered)) / float64(taken.Minutes())
		wpm := float64(wc)/float64(taken.Minutes())

		fmt.Printf(`%s%c%s %s%.2d:%.2d:%.3d%s
	Words typed: %d
	Characters per minute: %.2f
	Estimated words per minute: %.2f
	Total mistakes: %d`, colorRed, spinner, colorReset, colorBlue, int(taken.Minutes()), int(taken.Seconds()), msec, colorReset, wc, cpm, wpm, mistakes)

		select {
		case <-done:
			stop <- true
			break loop
		default:
			time.Sleep(time.Millisecond * 10)
			taken = time.Since(start)
		}

	}

	return 0
}
