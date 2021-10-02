package chars

import (
	"math/rand"
)

const (
	maxPhrases int = 4
	maxChars   int = 15

	minPhrases int = 1
	minChars   int = 5
)

func generate(dataset []rune) {
	count := rand.Intn(maxPhrases) + minPhrases
	chars = make([][]string, count)

	for x := 0; x < count; x++ {
		nphrases := rand.Intn(maxPhrases) + minPhrases

		chars[x] = make([]string, nphrases)

		for i := range chars[x] {
			nchars := rand.Intn(maxChars) + minChars
			buf := make([]rune, nchars)

			for j := range buf {
				sel := rand.Intn(len(dataset))
				buf[j] = dataset[sel]
			}

			chars[x][i] = string(buf)
		}
	}
}
