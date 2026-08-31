#include <stdio.h>
#include <stdlib.h>
#include "fprio.h"

struct fprio_t *fprio_cria () {
	struct fprio_t *novaFila;

	/* Se der erro de alocação, retorna NULL. */
	if ( !( novaFila = malloc( sizeof( struct fprio_t ) ) ) )
		return NULL;

	/* Inicializa os itens da nova fila. */	
	novaFila -> prim = NULL;
	novaFila -> fim = NULL;
	novaFila -> num = 0;

	return novaFila;

}

struct fprio_t *fprio_destroi (struct fprio_t *f) {
	struct fpnodo_t *ant, *proximo;

	ant = f -> prim;
	/* Vai liberando todos os itens e nodos da fila. */
	while ( ant != NULL ) {
		proximo = ant -> prox;
		if ( ant -> item != NULL )
			free( ant -> item );
		free( ant );
		ant = proximo;
	}

	/* Libera a fila e a iguala a NULL. */
	free( f );
	f = NULL;

	return f;

}

int fprio_insere (struct fprio_t *f, void *item, int tipo, int prio) {
	struct fpnodo_t *novoNodo;	
	struct fpnodo_t *ant, *proximo;

	/* Retorna -1 se der erro de alocação. */	
	if ( !( novoNodo = malloc( sizeof( struct fpnodo_t ) ) ) )
		return -1;

	/* Retorna -1 se recebeu uma fila ou item nulo. */
	if ( f == NULL || item == NULL ) {
		free( novoNodo );
		return -1;
	}

	/* Coloca as variáveis do novoNodo em seus respectivos espaços. */
	novoNodo -> item = item;
	novoNodo -> prio = prio;
	novoNodo -> tipo = tipo;

	ant = f -> prim;
	
	/* Se não houver nenhum elemento na fila, coloca na primeira posição */
	if ( f -> num == 0 ) {
		novoNodo -> prox = NULL;
		f -> prim = novoNodo;
		f -> fim = novoNodo;
		f -> num++;

		return f -> num;
	}

	/* Percorre todo a fila para checar se há ponteiros que são iguais. */
	while ( ant != NULL ) {
		if ( ant -> item == item ) {
			free( novoNodo );

			return -1;
		}
		
		ant = ant -> prox;
	}

	/* Já temos certeza que não há nenhum ponteiro repetido. 
	 * Portanto, podemos aumentar o tamanho. */
	f -> num++;
	
	/* Já que ant percorreu a fila toda, devemos reiniciá-la. */	
	ant = f -> prim;

	/* Checa se o novoNodo deve ser colocado já na 1a posição. */
	if ( ant -> prio > prio ) {
		novoNodo -> prox = ant;
		f -> prim = novoNodo;

		return f -> num;
	}
	
	proximo = ant -> prox;	
	while ( proximo != NULL && proximo -> prio <= prio ) {
		ant = proximo;
		proximo = proximo -> prox;
	}

	/* Coloca na última posição. */	
	if ( proximo == NULL ) {
		novoNodo -> prox = NULL;
		f -> fim = novoNodo;
		ant -> prox = novoNodo;

		return f -> num;
	}

	/* Coloca-se entre o ant e o proximo. */		
	novoNodo -> prox = proximo;
	ant -> prox = novoNodo;

	return f -> num;

}

void *fprio_retira (struct fprio_t *f, int *tipo, int *prio) {
	void *item;
	struct fpnodo_t *aux;
		
	if ( ( f == NULL || f -> num == 0 ) || ( tipo == NULL || prio == NULL ) )
		return NULL;

	aux = f -> prim;
	*tipo = aux -> tipo;
	*prio = aux -> prio;
	item = aux -> item;
	f -> num--; /* Diminui o tamanho. */
	
	/* O novo início é o próximo do antigo início. */
	f -> prim = aux -> prox;
	/* Libera o antigo início. */
	free( aux );
	aux = NULL;

	return item;	

}

int fprio_tamanho (struct fprio_t *f) {
	
	if ( f == NULL )
		return -1;

	return f -> num;

}

void fprio_imprime (struct fprio_t *f) {
	struct fpnodo_t *ant;

	if ( f == NULL || f -> num == 0 )
		return;

	ant = f -> prim;
	/*Para o loop no penúltimo elemento, evitando espaços desnecessários*/
	while ( ant -> prox != NULL ) {
		printf( "(%d %d) ", ant -> tipo, ant -> prio );
		ant = ant -> prox;
	}
	printf( "(%d %d)", ant -> tipo, ant -> prio );
	
}
