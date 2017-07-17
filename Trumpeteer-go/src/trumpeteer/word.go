package trumpeteer

type Word struct {
	word string
	duration float64
}

func NewWord(word string, duration float64) *Word {
	w := new(Word)
	w.word = word
	w.duration = duration
	return w
}

func (w* Word) getWord() string {
	return w.word
}

func (w* Word) getDuration() float64 {
	return w.duration
}

