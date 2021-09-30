package main

import (
	"os"
)

func TimeInput(expected string) int64 {
	ClearScreen()

	var i [1]byte
	for {
		os.Stdin.Read(i[:])
		if i[0] == byte('\n') {
			break
		}
	}

	return 0
}
