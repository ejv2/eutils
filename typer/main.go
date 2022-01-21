// Typer - A simple typing trainer for your terminal
// By Ethan Marshall - 2021
package main

import (
	"flag"
	"fmt"
	"math/rand"
	"os/exec"
	"time"

	"github.com/ethanv2/eutils/typer/core"
	"github.com/ethanv2/eutils/typer/chars"
	"github.com/ethanv2/eutils/typer/words"
)


// Flag values
var flags core.Flags

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

func flagsInit() {
	flag.IntVar(&flags.Count, "c", 0, "Exit after this many rounds")
	flag.BoolVar(&flags.Lower, "l", false, "Make all words lowercase")
	flag.BoolVar(&flags.Alpha, "a", false, "Make all words just letters")

	flag.Parse()
}

func main() {
	rand.Seed(time.Now().Unix())
	banner()
	flagsInit()

	mode := selectMode()
	words.InitWords()
	ttyInit()

	switch mode {
	case modeWords:
		words.RunWords(flags)
	case modeLetters:
		chars.RunChars([]rune(core.Alphabet))
	case modeNumbers:
		chars.RunChars([]rune(core.Numbers))
	case modeMixed:
		break
	default:
		panic("invalid round type generated")
	}
}
