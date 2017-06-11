CFLAGS=-Wall -g
FICHEIROS=cgi.h estado.c estado.h jogo.c Makefile ler.c escrever.c ler.h escrever.h LEADERS.dat SAVE.dat Imagens/*

install: jogo
	sudo cp jogo /usr/lib/cgi-bin
	sudo cp Imagens/* /var/www/html/images
	sudo cp LEADERS.dat /usr/lib/cgi-bin
	sudo cp SAVE.dat /usr/lib/cgi-bin
	sudo chown -R www-data:www-data /usr/lib/cgi-bin
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
