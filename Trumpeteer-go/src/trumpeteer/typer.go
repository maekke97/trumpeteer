package trumpeteer

import(
	"bytes"
	"gopkg.in/gographics/imagick.v2/imagick"
)

type Typer struct {
	backGround, foreGround *imagick.PixelWand
	pencil *imagick.DrawingWand
	sentence Sentence
}

func NewTyper() *Typer {
	typer := new(Typer)

	typer.initializeBackground()
	typer.initializeForeGround()
	// Keep last, since it requires the foreground to be initialized.
	typer.initializePencil()

	return typer
}

func (t *Typer) AddWord(word string, duration float64) {
	w := NewWord(word, duration)
	t.sentence.AddWord(w)
}

func (t *Typer) TypeWords() {
	var buffer bytes.Buffer

	magickWand := imagick.NewMagickWand()
	for _, word := range t.sentence.GetWords() {
		for _, char := range word.getWord() {
			buffer.WriteRune(char)
			magickWand.NewImage(800, 900, t.backGround)

			magickWand.AnnotateImage(t.pencil, 100,100,0,buffer.String())
		}

		buffer.WriteRune(' ')
		magickWand.NewImage(800, 900, t.backGround)
		magickWand.AnnotateImage(t.pencil, 100,100,0,buffer.String())
}

	magickWand.WriteImages("out.png", true)
}

func (t *Typer) initializePencil()  {
	t.pencil = imagick.NewDrawingWand()
	t.pencil.SetFillColor(t.foreGround)
	t.pencil.SetFontFamily("Times New")
	t.pencil.SetFontSize(18.0)
}

func (t *Typer) initializeForeGround() {
	t.foreGround = imagick.NewPixelWand()
	t.foreGround.SetColor("white")
}

func (t *Typer) initializeBackground() {
	t.backGround = imagick.NewPixelWand()
	t.backGround.SetColor("black")
}



	
