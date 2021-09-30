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
		num, err := strconv.ParseInt(t, 10, 32)

		if err == nil && num <= modeMixed && num >= modeWords {
			return int(num)
		} else {
			fmt.Println("Invalid difficulty")
		}
	}
}
