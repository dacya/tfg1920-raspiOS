.PHONY: main.pdf all clean

all: main.pdf

main.pdf: main.tex
	latexmk -pdf -pdflatex="pdflatex -interactive=nonstopmode" -use-make main.tex

clean:
	latexmk -CA

