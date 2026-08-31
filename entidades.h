#ifndef ENTIDADES
#define ENTIDADES

#include "fila.h"
#include "conjunto.h"

/* Posições cartesianas. */
struct posicao {
	int x;
	
	int y;
};

struct heroi {
	/* Identifica unicamente o herói. */
	int id;

	/* Conjunto de habilidades que o herói possui. */
	struct cjto_t *habilidades;
	
	/* Indica o quão paciente uma pessoa é. */
	int paciencia;

	/* Indica a velocidade de deslocamento do herói. */
	int velocidade;
	
	/* Número de missões em que o herói já participou. */
	int xp;
	
	/* ID da base em que o herói se encontra. */
	int base;

	/* Indica se o herói está vivo(1) ou morto(0), ou seja, inutilizável. */
	int vivo;
};

struct base {
	/* Identifica unicamente uma base. */
	int id;

	/* Número máximo de heróis naquela base. */
	int lotacao;

	/* Conjunto dos IDs dos heróis que estão atualmente na base. */
	struct cjto_t *presentes;

	/* Fila onde os heróis esperam pra poder entrar na base. */
	struct fila_t *fila;
	
	/* Posição da base no mundo. */
	struct posicao *local;

	/* Identifica quantas missões foram realizadas por essa base. */
	int missoesRealizadas;

	/* Identifica o máximo de pessoas que esteve na fila. */
	int filaMaxima;
};

struct missao {
	/* Identifica unicamente cada missão */
	int id;

	/* Conjunto de habilidades necessárias pra concluir a missão. */
	struct cjto_t *habilidades;

	/* Posição da missão no mundo. */
	struct posicao *local;

	/* Indica se a missão já foi cumprida(1) ou não(0). */
	int cumprida;

	/* Indica quantas tentativas já foram feitas para cumprir a missão. */
	int tentativasFeitas;
};

struct mundo {
	/* Identifica a quantidade de heróis no mundo. */
	int numHerois;
	/* Vetor representando todos os heróis. */
	struct heroi **herois;
	
	/* Identifica a quantidade de bases no mundo. */
	int numBases;
	/* Vetor representando todas as bases. */
	struct base **bases;
	
	/* Identifica a quantidade de missões a serem cumpridas. */
	int numMissoes;
	/* Vetor representando todas as missões. */
	struct missao **missoes;
	
	/* Número de habilidades distintas possíveis. */
	int numHabilidades;
	
	/* Número de Compostos V disponíveis. */
	int numCompV;

	/* Coordenadas máximas do plano cartesiano. */
	struct posicao *tamMundo;
	
	/* Indica o tempo atual no mundo. */
	int relogio;
};

/* Desaloca todos os elementos do mundo. 
 * Retorno: NULL */
struct mundo *destroiMundo( struct mundo *mundo );

#endif

