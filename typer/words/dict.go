package words

import (
	"io"
	"math/rand"
	"os"
	"runtime"
	"strings"
	"unicode"
	"unicode/utf8"

	"github.com/ejv2/eutils/typer/core"
)

var dictPossibilities = []string{
	"/usr/share/dict/words",
	"/usr/share/dict/usa",
	"/usr/share/dict/british",
}

var (
	dictInit bool
	dictBuf  []byte
)

// Load and process built-in fallback sentences.
func loadFallback() {
	cpy := fallbackSentences[:]
	rand.Shuffle(len(cpy), func(i, j int) { cpy[i], cpy[j] = cpy[j], cpy[i] })

	for x := range cpy {
		rand.Shuffle(len(cpy[x]), func(i, j int) { cpy[x][i], cpy[x][j] = cpy[x][j], cpy[x][i] })

		r, count := utf8.DecodeRuneInString(cpy[x][0])
		if count == 0 || r == utf8.RuneError {
			panic("fallback: invalid UTF-8 in fallback sentence")
		}
		r = unicode.ToUpper(r)

		if len(cpy[x][0]) == count {
			cpy[x][0] = string(r)
		} else {
			cpy[x][0] = string(r) + cpy[x][0][count:]
		}
	}

	words = cpy
}

// Load system dictionary into buffer, if present.
func InitWords() {
	if runtime.GOOS == "windows" {
		return
	}

	var file *os.File
	var err error
	for _, elem := range dictPossibilities {
		file, err = os.Open(elem)
		if err == nil {
			defer file.Close()
			break
		}
	}

	if file == nil {
		return
	}

	// This might be kind of bad - maybe change this
	buf, err := io.ReadAll(file)
	if err != nil {
		return
	}

	dictInit = true
	dictBuf = buf
}

// Loads words from the system dictionary.
func LoadWords(f core.Flags) {
	if !dictInit {
		loadFallback()
		return
	}

	count := rand.Intn(maxWords) + 1
	words = make([][]string, count)

	for i := 0; i < count; i++ {
		w := strings.Split(string(dictBuf), "\n")
		ws := make([]string, rand.Intn(maxWords)+5)

		for j := 0; j < len(ws); j++ {
			word := w[rand.Int31n(int32(len(w)))]
			if f.Lower {
				word = strings.ToLower(word)
			}
			if f.Alpha {
				word = strings.ReplaceAll(word, "'", "")
			}

			ws[j] = word
		}

		words[i] = ws
	}
}
