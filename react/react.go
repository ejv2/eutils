package main

import (
	"fmt"
	"math/rand"
	"os"
	"time"
)

func main() {
	for {
		var c [1]byte
		wait := time.Duration(rand.Int63n(4) + 1) * time.Second

		fmt.Println("Wait for the prompt and press enter...")
		time.Sleep(wait)
		fmt.Print("NOW!")

		now := time.Now()
		os.Stdin.Read(c[:])

		diff := time.Now().Sub(now)
		fmt.Printf("%v\n", diff)

		fmt.Print("Press enter to reset...")
		os.Stdin.Read(c[:])
	}
}
