reader: reader.c
	gcc -Wall `pkg-config --cflags sndfile` -c reader.c -o reader -lfftw3 -lm

clean:
	rm -f reader
