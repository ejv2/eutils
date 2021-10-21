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

func RunWords(f core.Flags) {
	LoadWords(f)

	for i, elem := range words {
		if f.Count > 0 && i >= f.Count {
			fmt.Println()
			return
		}

		util.ClearScreen()
		fmt.Printf("Round %d commencing in  ", i+1)
		util.Countdown(5)

		core.TimeInput(strings.Join(elem, " "))
		core.ResetState()
	}
}
