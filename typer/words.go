package main

import (
	"io"
	"fmt"
	"time"
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
	dictInit bool
	dictBuf  []byte = nil
	words    [][]string
)

// Load and process built-in fallback sentences
func loadFallback() {
	cpy := fallbackSentences[:]
	rand.Shuffle(len(cpy), func(i, j int) { cpy[i], cpy[j] = cpy[j], cpy[i] })

	for x := range cpy {
		rand.Shuffle(len(cpy[x]), func(i, j int) { cpy[x][i], cpy[x][j] = cpy[x][j], cpy[x][i] })

		// TODO: Figure out how to make the first word upper case
		//upper := strings.ToUpper(cpy[x][0][:1]) + cpy[x][0][1:]
	}

	words = cpy
}

// Load system dictionary into buffer, if present
func InitWords() {
	if runtime.GOOS == "windows" {
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
		return
	}

	// This might be kind of bad - maybe change this
	buf, err := io.ReadAll(file)
	if err != nil {
		return
	}

	dictInit = true
	dictBuf = buf
}

// Loads words from the system dictionary
func LoadWords() {
	if !dictInit {
		loadFallback()
		return
	}

	count := rand.Intn(maxWords) + 1
	words = make([][]string, count)

	for i := 0; i < count; i++ {
		w := strings.Split(string(dictBuf), "\n")
		ws := make([]string, rand.Intn(maxWords)+5)

		for j := 0; j < len(ws); j++ {
			ws[j] = w[rand.Int31n(int32(len(w)))]
		}

		words[i] = ws
	}
}

func countdown(count int) {
	for i := count; i >= 0; i-- {
		fmt.Printf("\b%d", i)
		time.Sleep(time.Second)
	}
}

func RunWords() {
	LoadWords()

	for i, elem := range words {
		ClearScreen()
		fmt.Printf("Round %d commencing in  ", i+1)
		countdown(5)

		TimeInput(strings.Join(elem, " "))
		ResetState()
	}
}
