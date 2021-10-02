package main

import (
	"fmt"
	"math/rand"
	"strings"
)

const (
	maxPhrases int = 4
	maxChars   int = 15

	minPhrases int = 1
	minChars   int = 5
)

var (
	chars [][]string
)

func generate(dataset []rune) {
	count := rand.Intn(maxPhrases) + minPhrases
	chars = make([][]string, count)

	for x := 0; x < count; x++ {
		nphrases := rand.Intn(maxPhrases) + minPhrases

		chars[x] = make([]string, nphrases)

		for i := range chars[x] {
			nchars := rand.Intn(maxChars) + minChars
			buf := make([]rune, nchars)

			for j := range buf {
				sel := rand.Intn(len(dataset))
				buf[j] = dataset[sel]
			}

			chars[x][i] = string(buf)
		}
	}
}

func RunChars(dataset []rune) {
	generate(dataset)

	for i, elem := range chars {
		ClearScreen()
		fmt.Printf("Round %d commencing in  ", i+1)
		Countdown(5)

		TimeInput(strings.Join(elem, " "))
		ResetState()
	}
}
