// programa principal do projeto "The Boys - 2024/2"
// Autor: Pedro Stahel de Azevedo, GRR20252084

// seus #includes vão aqui
#include <stdlib.h>
#include "entidades.h"
#include "lef.h"
#include "eventos.h"
#include "conjunto.h"
#include "fila.h"

/* Aloca um local e o retorna. */
struct posicao *criaLocal( int dimX, int dimY ) {
	struct posicao *local;

	if ( !( local = malloc( sizeof( struct posicao ) ) ) )
		return NULL;

	local -> x = dimX;
	local -> y = dimY;

	return local;

}

/* Cria todas as variáveis e as inicializa. */
struct mundo *inicializa_mundo() {
	int i, qtdHabilidadesAleatorias;
	struct mundo *mundo;

	/* Se der erro de alocação, retorna NULL. */
	if ( !( mundo = malloc( sizeof( struct mundo ) ) ) )
		return NULL;

	/* Se der erro de alocação, retorna NULL. */
	if ( !( mundo -> herois = malloc( sizeof( struct heroi * ) * N_HEROIS ) ) ) {
		free( mundo );
		mundo = NULL;
		
		return NULL;
	}

	/* Se der erro de alocação, retorna NULL. */
	if ( !( mundo -> bases = malloc( sizeof( struct base * ) * N_BASES ) ) ) {
		free( mundo -> herois );
		mundo -> herois = NULL;
		
		free( mundo );
		mundo = NULL;
		
		return NULL;
	}

	/* Se der erro de alocação, retorna NULL. */
	if ( !( mundo -> missoes = malloc( sizeof( struct missao * ) * N_MISSOES ) ) ) {
		free( mundo -> herois );
		mundo -> herois = NULL;

		free( mundo -> bases );
		mundo -> bases = NULL;

		free( mundo );
		mundo = NULL;

		return NULL;
	}

	/* Inicializa todas as variáveis do mundo. */
	mundo -> numHerois = N_HEROIS;
	mundo -> numBases = N_BASES;
	mundo -> numMissoes = N_MISSOES;
	mundo -> numCompV = N_COMPOSTOS_V;
	mundo -> numHabilidades = N_HABILIDADES;
	mundo -> relogio = T_INICIO;

	/* Se der erro de alocação, retorna NULL. */
	if ( !( mundo -> tamMundo = criaLocal( TAMANHO_MUNDO - 1, TAMANHO_MUNDO - 1 ) ) )
		return NULL;

	/* Inicializa todos os heróis e os insere no vetor de mundo. */
	for ( i = 0; i < N_HEROIS; i++ ) {
		struct heroi *heroi;

		/* Se der erro de alocação, retorna NULL. */
		if ( !( heroi = malloc( sizeof( struct heroi ) ) ) )
			return NULL;

		qtdHabilidadesAleatorias = rand() % 3 + 1;
		/* Se der erro de alocação, retorna NULL. */
		if ( !( heroi -> habilidades = cjto_aleat( qtdHabilidadesAleatorias, N_HABILIDADES ) ) ) {
			free( heroi );

			return NULL;
		}

		heroi -> id = i;
		heroi -> base = N_BASES;
		heroi -> xp = 0;
		heroi -> paciencia = rand() % 101;
		heroi -> velocidade = rand() % 4951 + 50;
		heroi -> vivo = 1;

		mundo -> herois[i] = heroi;
	}

	/* Inicializa todas as bases e as insere no vetor de mundo. */
	for ( i = 0; i < N_BASES; i++ ) {
		struct base *base;

		/* Se der erro de alocação, retorna NULL. */
		if ( !( base = malloc( sizeof( struct base ) ) ) )
			return NULL;

		base -> id = i;
		base -> local = criaLocal( rand() % TAMANHO_MUNDO, rand() % TAMANHO_MUNDO );
		base -> lotacao = rand() % 8 + 3;
		base -> presentes = cjto_cria( N_HEROIS );
		base -> fila = fila_cria();
		base -> filaMaxima = 0;
		base -> missoesRealizadas = 0;

		mundo -> bases[i] = base;
	}

	/* Inicializa todas as missões e as insere no vetor de mundo. */
	for ( i = 0; i < N_MISSOES; i++ ) {
		struct missao *missao;

		/* Se der erro de alocação, retorna NULL. */
		if ( !( missao = malloc( sizeof( struct missao ) ) ) )
			return NULL;

		qtdHabilidadesAleatorias = rand() % 5 + 6;
		missao -> id = i;
		missao -> local = criaLocal( rand() % TAMANHO_MUNDO, rand() % TAMANHO_MUNDO );
		missao -> cumprida = 0;
		missao -> tentativasFeitas = 0;
		missao -> habilidades = cjto_aleat( qtdHabilidadesAleatorias, N_HABILIDADES );

		mundo -> missoes[i] = missao;
	}

	return mundo;

}

int main () {
	struct mundo *mundo;
	struct lef *lef;

	srand( 50 );

  	/* Se der erro, retorna 1. */
	if ( !( mundo = inicializa_mundo() ) )
		return 1;

	/* Se der erro, retorna 1. */
	if ( !( lef = lef_inicializa( mundo ) ) ) {
		mundo = destroiMundo( mundo );

		return 1;
	}
	
	lef_eventos( mundo, lef );

	/* Libera todas as variáveis do mundo. */
	mundo = destroiMundo( mundo );

	/* Libera todas as variáveis da LEF. */
	lef = lef_destroi( lef );

  	return 0;
}