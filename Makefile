build:
	g++ source.cpp -o source -lcorona -lpng -lm -ljpeg -g -Wall

clean:
	rm source
