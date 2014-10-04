reader: reader.c
	gcc -Wall `pkg-config --cflags sndfile` -c reader.c -o reader

clean:
	rm -f reader
