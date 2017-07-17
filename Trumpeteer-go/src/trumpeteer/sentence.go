package trumpeteer

type Sentence struct {
	words []*Word
	interWordPause float64
}

func NewSentence() *Sentence {
	s := new(Sentence)
	s.interWordPause = 1;
	return s
}

func (s *Sentence) SetInterWordPause(pause float64) {
	s.interWordPause = pause
}

func (s *Sentence) AddWord(word *Word) {
	s.words = append(s.words, word)
}

func (s *Sentence) GetWords() []*Word {
	return s.words
}
