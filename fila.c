#include <stdio.h>
#include <stdlib.h>
#include "fila.h"

struct fila_t *fila_cria () {
	struct fila_t *novaFila;

	if ( !( novaFila = malloc( sizeof( struct fila_t ) ) ) )
		return NULL;

	/* Inicializa as variáveis. */
	novaFila -> prim = NULL;
	novaFila -> ult = NULL;
	novaFila -> num = 0;

	return novaFila;

}

struct fila_t *fila_destroi (struct fila_t *f) {
	struct fila_nodo_t *ant, *proximo;

	
	if ( f == NULL )
		return f;

	/* Percorre a fila toda e desaloca todos os nodos. */
	ant = f -> prim;
	while ( ant != NULL ) {
		proximo = ant -> prox; 
		free( ant );
		ant = proximo;
	}

	/* Desaloca a fila em si. */
	free( f );
	f = NULL;

	return f;

}

int fila_insere (struct fila_t *f, int item) {
	struct fila_nodo_t *novoNodo;

	if ( !( novoNodo = malloc( sizeof( struct fila_nodo_t ) ) ) )
		return 0;

	if ( f == NULL ) {
		free( novoNodo );
		
		return 0;
	}

	/* Inicializa as variáveis do novo nodo. */
	novoNodo -> item = item;
	novoNodo -> prox = NULL;
	
	f -> num++;
	/* Se não houver nada na fila, o novo nodo é o primeiro e último elemento. */
	if ( f -> num == 1 ) {
		f -> prim = novoNodo;
		f -> ult = novoNodo;

		return 1;
	}

	/* O novo nodo é o próximo do antigo último. */
	f -> ult -> prox = novoNodo;
	f -> ult = novoNodo;

	return 1;
	
}

int fila_retira (struct fila_t *f, int *item) {
	struct fila_nodo_t *primeiro;

	if ( f == NULL || f -> num == 0 )
		return 0;

	primeiro = f -> prim;
	*item = primeiro -> item;
	/* O novo primeiro é o antigo segundo na fila. */
	f -> prim = primeiro -> prox;
	f -> num--;
	/* Se agora o tamanho for 0, o último elemento vira NULL. 
	 * Caso contrário, ele permanece o mesmo. */
	if ( f -> num == 0 )
		f -> ult = NULL;
	
	free( primeiro );
	primeiro = NULL; 

	return 1;

}

int fila_tamanho (struct fila_t *f) {
	if ( f == NULL )
		return -1;

	return f -> num;

}

void fila_imprime (struct fila_t *f) {
	struct fila_nodo_t *atual;

	if ( f == NULL || f -> num == 0 )
		return;

	/* Percorre a fila toda e imprime todos os elementos. */
	atual = f -> prim;
	while ( atual != f -> ult ) {
		printf( "%d ", atual -> item );
		atual = atual -> prox;
	}

	printf( "%d", atual -> item );
}