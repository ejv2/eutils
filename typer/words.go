package main

import (
	"io"
	"math/rand"
	"os"
	"runtime"
	"strings"
)

const (
	maxSets  int = 4
	maxWords int = 15
)

var dictPossibilities = []string{
	"/usr/share/dict/words",
	"/usr/share/dict/usa",
	"/usr/share/dict/british",
}

var (
	words [][]string
)

func loadFallback() {
	cpy := fallbackSentences[:]
	rand.Shuffle(len(cpy), func(i, j int) { cpy[i], cpy[j] = cpy[j], cpy[i] })

	for x := range cpy {
		rand.Shuffle(len(cpy[x]), func(i, j int) { cpy[x][i], cpy[x][j] = cpy[x][j], cpy[x][i] })

		upper := strings.ToUpper(cpy[x][0][:1]) // Make the first word uppercase
		cpy[x][0] = upper + cpy[x][0][1:]
	}

	words = cpy
}

// Loads words from the system dictionary
func LoadWords() {
	if runtime.GOOS == "windows" {
		loadFallback()
		return
	}

	var file *os.File
	var err error
	for _, elem := range dictPossibilities {
		file, err = os.Open(elem)
		if err == nil {
			defer file.Close()
			break
		}
	}

	if file == nil {
		loadFallback()
		return
	}

	// This might be kind of bad - maybe change this
	buf, err := io.ReadAll(file)
	if err != nil {
		loadFallback()
		return
	}

	count := rand.Intn(maxWords) + 1
	words = make([][]string, count)
	for i := 0; i < count; i++ {
		w := strings.Split(string(buf), "\n")
		ws := make([]string, rand.Intn(maxWords)+5)

		for j := 0; j < len(ws); j++ {
			ws[j] = w[rand.Int31n(int32(len(w)))]
		}

		words[i] = ws
	}
}

func RunWords() {
}
