package main

import (
	"math/rand"
)

const (
	maxPhrases int = 4
	maxChars   int = 15

	minPhrases int = 1
	minChars   int = 5
)

var (
	chars [][]rune
)

func generate(dataset []rune) {
	nphrases := rand.Intn(maxPhrases) + minPhrases

	chars = make([][]rune, nphrases)

	for i := range chars {
		nchars   := rand.Intn(maxChars) + minChars
		chars[i] = make([]rune, nchars)

		for j := range chars[i] {
			sel := rand.Intn(len(dataset))
			chars[i][j] = dataset[sel]
		}
	}
}

func RunChars(dataset []rune) {
	generate(dataset)
}
