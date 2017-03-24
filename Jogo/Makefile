CFLAGS=-Wall -g
FICHEIROS=cgi.h estado.c estado.h jogo.c Makefile

install: jogo
	sudo cp jogo /usr/lib/cgi-bin
	touch install

jogo: jogo.o estado.o
	cc -o jogo jogo.o estado.o

jogo.zip: $(FICHEIROS)
	zip -9 jogo.zip $(FICHEIROS)

doc:
	doxygen -g
	doxygen

clean:
	rm -rf *.o jogo Doxyfile latex html install

estado.o: estado.c estado.h
jogo.o: jogo.c cgi.h estado.h
