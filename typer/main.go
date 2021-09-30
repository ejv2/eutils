// Typer - A simple typing trainer for your terminal
// By Ethan Marshall - 2021
package main

import (
	"fmt"
	"math/rand"
	"os/exec"
	"time"
)

const (
	alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
	numbers  = "1234567890"
)

func banner() {
	fmt.Println("Typer - A simple typing trainer for your terminal")
	fmt.Print("Copyright (C) 2021 - Ethan Marshall\n\n")
}

func ttyInit() {
	// disable input buffering
	exec.Command("stty", "-F", "/dev/tty", "cbreak", "min", "1").Run()
	// do not display entered characters on the screen
	exec.Command("stty", "-F", "/dev/tty", "-echo").Run()
}

func main() {
	rand.Seed(time.Now().Unix())
	banner()

	mode := selectMode()
	InitWords()
	ttyInit()

	for {
		switch mode {
		case modeWords:
			RunWords()
		case modeLetters:
			RunChars([]rune(alphabet))
		case modeNumbers:
			RunChars([]rune(numbers))
		case modeMixed:
			break
		default:
			fmt.Println("Invalid round type detected!")
			return
		}
	}
}
