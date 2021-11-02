package util

import (
	"fmt"
	"os"
	"time"
)

func ClearScreen() {
	os.Stdout.Write([]byte("[H[2J")) // Should work for most ANSII terminals
}

func Countdown(count int) {
	for i := count; i >= 0; i-- {
		fmt.Printf("\b%d", i)
		time.Sleep(time.Second)
	}
}
