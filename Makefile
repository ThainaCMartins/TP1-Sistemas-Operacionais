CC=gcc
CFLAGS=-O2 -Wall -Wextra -std=c11
all: pi_threads processo_a processo_b limpar_filas
pi_threads: pi_threads.c
	$(CC) $(CFLAGS) -o $@ $< -lm -lpthread
processo_a: processo_a.c protocolo.h
	$(CC) $(CFLAGS) -o $@ processo_a.c -lrt
processo_b: processo_b.c protocolo.h
	$(CC) $(CFLAGS) -o $@ processo_b.c -lrt
limpar_filas: limpar_filas.c protocolo.h
	$(CC) $(CFLAGS) -o $@ limpar_filas.c -lrt
clean:
	rm -f pi_threads processo_a processo_b limpar_filas
.PHONY: all clean
