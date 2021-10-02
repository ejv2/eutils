package words

import (
	"fmt"
	"strings"

	"github.com/ethanv2/eutils/typer/util"
	"github.com/ethanv2/eutils/typer/core"
)

const (
	maxSets  int = 4
	maxWords int = 15
)

var (
	words    [][]string
)

func RunWords() {
	LoadWords()

	for i, elem := range words {
		util.ClearScreen()
		fmt.Printf("Round %d commencing in  ", i+1)
		util.Countdown(5)

		core.TimeInput(strings.Join(elem, " "))
		core.ResetState()
	}
}
