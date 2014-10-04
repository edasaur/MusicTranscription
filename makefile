SND = `pkg-config --cflags sndfile`
PACKAGES = -lfftw3 -lsndfile -lm

reader.o: reader.c
	gcc -Wall -c $(SND) reader.c -o reader.o $(PACKAGES)

reader: reader.o
	gcc -Wall $(SND) reader.o -o reader $(PACKAGES)

clean:
	rm reader
	rm reader.o
