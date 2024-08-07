package main

import (
	"flag"
	"fmt"
	"os"
	"strings"
	"time"
)

var Format = time.UnixDate

func init() {
	flag.BoolFunc("email", "RFC1123", func(string) error {
		Format = time.RFC1123Z
		return nil
	})
	flag.BoolFunc("email-tz", "RFC1123 (with textual timezone)", func(string) error {
		Format = time.RFC1123
		return nil
	})
	flag.BoolFunc("std", "RFC822", func(string) error {
		Format = time.RFC822Z
		return nil
	})
	flag.BoolFunc("std-tz", "RFC822 (with textual timezone)", func(string) error {
		Format = time.RFC822
		return nil
	})
	flag.BoolFunc("ansic", "ansi c format", func(string) error {
		Format = time.ANSIC
		return nil
	})
	flag.BoolFunc("mutt", "mutt date format", func(string) error {
		Format = "Mon, 2 Jan 2006 15:04:05 -0700"
		return nil
	})
}

func main() {
	flag.Parse()
	if flag.NArg() == 0 {
		fmt.Fprintln(os.Stderr, "tolocal: missing date string argument")
		os.Exit(1)
	}

	ds := strings.Join(flag.Args(), " ")
	tm, err := time.Parse(Format, ds)
	if err != nil {
		fmt.Fprintln(os.Stderr, "tolocal:", err)
		os.Exit(1)
	}

	fmt.Println(tm.Local())
}
