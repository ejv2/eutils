// Words fallback - words to fall back upon
// Only used when either:
//	- On Windows (dictionary API sucks really badly)
//	- A system dictionary could not be found
package words

var fallbackSentences = [7][]string{
	{"the", "quick", "brown", "fox", "jumped", "over", "the", "lazy", "dog"},
	{"tell", "be", "about", "the", "rabbits,", "george"},
	{"stop", "interrupting", "me", "while", "I", "am", "interrupting", "you!"},
	{"the", "genius", "behind", "the", "genre", "of", "ragtime", "is", "elite", "syncopations"},
	{"I", "have", "observed", "that", "the", "experience", "of", "a", "programmer", "is", "a", "function", "of", "their", "denisity", "of", "goto", "statements;", "goto", "considered", "harmful"},
	{"Every", "book", "had", "been,", "rewritten,", "every", "picture", "repainted", "every", "statue", "renamed"},
	{"The", "industrial", "revolution", "and", "its", "consiquences", "have", "been", "a", "disaster", "for", "the", "human", "race"},
}
