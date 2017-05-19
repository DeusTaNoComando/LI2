CFLAGS=-Wall -g
FICHEIROS=cgi.h estado.c estado.h jogo.c Makefile ler.c escrever.c

install: jogo
	sudo cp jogo /usr/lib/cgi-bin
	sudo cp LEADERS.dat /usr/lib/cgi-bin
	sudo cp SAVE.dat /usr/lib/cgi-bin
	touch install

jogo: jogo.o estado.o ler.o escrever.o
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
escrever.o: escrever.c escrever.h estado.c
ler.o: ler.c ler.h estado.c
