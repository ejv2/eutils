package main

import (
	"flag"
	"fmt"
	"math/rand"
	"os"
	"time"
)

var (
	count    *int           = flag.Int("c", 0, "Take this many samples")
	interval *time.Duration = flag.Duration("d", 0, "Wait this long between samples; do not randomize")
)

func main() {
	flag.Parse()

	res := make([]time.Duration, *count)
	for i := 0; *count == 0 || (*count > 0 && i < *count); i++ {
		var wait time.Duration
		var c [1]byte

		if interval.Nanoseconds() == 0 {
			wait = time.Duration(rand.Int63n(4)+1) * time.Second
		} else {
			wait = *interval
		}

		fmt.Println("Wait for the prompt and press enter...")
		time.Sleep(wait)
		fmt.Print("NOW!")

		now := time.Now()
		os.Stdin.Read(c[:])

		diff := time.Now().Sub(now)
		fmt.Printf("%v\n", diff)

		if *count != 0 {
			res[i] = diff
		}

		fmt.Print("Press enter to continue...")
		os.Stdin.Read(c[:])
	}

	var total time.Duration
	var avg time.Duration

	for _, elem := range res {
		total += elem
	}
	avg = total / time.Duration(*count)

	fmt.Printf(`Result summary
==============

Total trials run: %d
Average reaction time: %v
`, *count, avg)
}
