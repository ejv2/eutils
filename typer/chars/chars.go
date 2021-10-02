package chars

import (
	"fmt"
	"strings"

	"github.com/ethanv2/eutils/typer/util"
	"github.com/ethanv2/eutils/typer/core"
)

var (
	chars [][]string
)

func RunChars(dataset []rune) {
	generate(dataset)

	for i, elem := range chars {
		util.ClearScreen()
		fmt.Printf("Round %d commencing in  ", i+1)
		util.Countdown(5)

		core.TimeInput(strings.Join(elem, " "))
		core.ResetState()
	}
}
