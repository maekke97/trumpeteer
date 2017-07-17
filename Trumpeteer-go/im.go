package main

import(
	"trumpeteer"
	"strings"
)

func main() {
	sentence := "Look ma, no hands!"

	trumper := trumpeteer.NewTyper()


	for _,  word := range strings.Split(sentence, " ") {
		trumper.AddWord(word, 2)
	}

	trumper.TypeWords()
}
	
