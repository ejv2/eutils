package main

import (
	"bufio"
	"fmt"
	"os"
	"strconv"
)

const (
	modeWords = iota
	modeLetters
	modeNumbers
	modeMixed
)

func modeHelp() {
	fmt.Println(`Available modes:
	Words:		A random selection of words is generated from your language
	Letters:	A random selection of letters jumbled into blocks
	Numbers:	A random selection of numbers jumbled into blocks
	Mixed:		A random mix of all the above

Select each mode with the corresponding mode on the menu.`)
}

func selectMode() int {
	fmt.Println("Welcome to Typer!")
	fmt.Print("Please select your mode below, or press '?' for help\n")
	fmt.Printf(`	%d) Words mode
	%d) Letters mode
	%d) Numbers mode
	%d) Mixed mode
`, modeWords, modeLetters, modeNumbers, modeMixed)

	s := bufio.NewScanner(os.Stdin)
	s.Split(bufio.ScanLines)
	for {
		fmt.Print("Mode:")
		s.Scan()

		t := s.Text()
		if t == "?" {
			modeHelp()
			continue
		}

		num, err := strconv.ParseInt(t, 10, 32)
		if err != nil {
			fmt.Println("EOF")
			os.Exit(1)
		}

		if num <= modeMixed && num >= modeWords {
			return int(num)
		}

		fmt.Println("Invalid mode")
	}
}
