// Copyright 2022 - Ethan Marshall
// For license details, see eutils licence in root of repository

// wtest is a minimal test tool for web clients and servers. It accepts every
// request and prints request information to standard error.
package main

import (
	"log"
	"net/http"
)

func main() {
	http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
		log.Printf("%s [%s] %s", r.Method, r.RemoteAddr, r.RequestURI)
		for key, val := range r.Header {
			log.Println("\t", key, "=>", val)
		}
	})

	log.Println("Server listening on :8080")
	log.Panic(http.ListenAndServe(":8080", nil))
}
