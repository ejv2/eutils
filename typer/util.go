package main

import (
	"os"
)

func ClearScreen() {
	os.Stdout.Write([]byte("[H[2J")) // Should work for most ANSII terminals
}
