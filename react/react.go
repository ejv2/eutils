package main

import (
	"flag"
	"fmt"
	"math/rand"
	"os"
	"time"
)

var (
	count int
)

func flags() {
	flag.IntVar(&count, "c", 0, "Take this many samples")

	flag.Parse()
}

func main() {
	flags()

	res := make([]time.Duration, count)
	for i := 0; count == 0 || (count > 0 && i < count); i++ {
		var c [1]byte
		wait := time.Duration(rand.Int63n(4)+1) * time.Second

		fmt.Println("Wait for the prompt and press enter...")
		time.Sleep(wait)
		fmt.Print("NOW!")

		now := time.Now()
		os.Stdin.Read(c[:])

		diff := time.Now().Sub(now)
		fmt.Printf("%v\n", diff)

		if count != 0 {
			res[i] = diff
		}

		fmt.Print("Press enter to continue...")
		os.Stdin.Read(c[:])
	}

	var total float64
	var avg float64

	for _, elem := range res {
		total += float64(elem.Milliseconds())
	}
	avg = float64(total) / float64(count)

	fmt.Printf(`Result summary
==============

Total trials run: %d
Average reaction time (ms): %f
`, count, avg)
}
