// Typer - A simple typing trainer for your terminal
// By Ethan Marshall - 2021
package main

import (
	"fmt"
)

var (
	mode int
)

func banner() {
	fmt.Println("Typer - A simple typing trainer for your terminal")
	fmt.Print("Copyright (C) 2021 - Ethan Marshall\n\n")
}

func main() {
	banner()

	mode = selectMode()
	switch mode {
	case modeWords:
		RunWords()
	case modeLetters:
		break
	case modeNumbers:
		break
	case modeMixed:
		break
	default:
		fmt.Println("Invalid round type detected!")
		return
	}
}
