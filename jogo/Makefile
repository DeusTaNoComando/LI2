CFLAGS=-Wall

install: jogo
	sudo cp jogo /usr/lib/cgi-bin
	touch install

jogo: main.o jogo.o
	cc -o jogo main.o jogo.o
