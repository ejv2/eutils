// Copyright 2022 - Ethan Marshall
// For license details, see eutils licence in root of repository

// wtest is a minimal test tool for web clients and servers. It accepts every
// request and prints request information to standard error.
package main

import (
	"flag"
	"log"
	"net/http"
	"os"
	"sync"
)

var (
	ListenAddr = flag.String("addr", ":8080", "Listen address for the server")
)

func main() {
	flag.Parse()
	log.SetOutput(os.Stdout)

	mut := new(sync.Mutex)
	http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
		mut.Lock()
		defer mut.Unlock()
		log.Printf("%s [%s] %s", r.Method, r.RemoteAddr, r.RequestURI)
		for key, val := range r.Header {
			log.Println("\t", key, "=>", val)
		}
	})

	log.Println("Server listening on", *ListenAddr)
	log.Panic(http.ListenAndServe(*ListenAddr, nil))
}
