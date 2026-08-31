#ifndef EVENTOS
#define EVENTOS

/* O tipo de cada evento é uma constante. */
#define CHEGA 1
#define AVISA 2
#define ESPERA 3
#define DESISTE 4
#define ENTRA 5
#define SAI 6
#define VIAJA 7
#define MORRE 8
#define MISSAO 9
#define FIM 10

/* Define a quantidade de cada elemento no mundo. */
#define T_INICIO 0
#define FIM_DO_MUNDO 525600
#define TAMANHO_MUNDO 20000
#define N_HABILIDADES 10
#define N_HEROIS 50
#define N_BASES 10
#define N_MISSOES 5256
#define N_COMPOSTOS_V 30

#include "entidades.h"
#include "lef.h"

/* Estrutura que define todos os eventos da LEF. */
struct evento {
    struct heroi *heroi;
    struct base *base;
    struct missao *missao;
};

/* Representa a chegada de um herói a certa base
 * Define se ele espera ou não nela.*/
void chega( struct mundo *mundo, int tempo, int idHeroi, int idBase, struct lef *lef );

/* Avisa o porteiro da base sobre os heróis na fila. */
void avisa( struct mundo *mundo, int tempo, int idBase, struct lef *lef );

/* O herói entra na fila de espera da base. */
void espera( struct mundo *mundo, int tempo, int idHeroi, int idBase, struct lef *lef );

/* O herói desiste de entrar na base e vai para outra. */
void desiste( struct mundo *mundo, int tempo, int idHeroi, struct lef *lef );

/* O herói entra na base
* Já é definido por quanto tempo ele ficará nela. */
void entra( struct mundo *mundo, int tempo, int idHeroi, int idBase, struct lef *lef );

/* O herói sai da base e procura outra. */
void sai( struct mundo *mundo, int tempo, int idHeroi, int idBase, struct lef *lef );

/* O herói se desloca para uma base. */
void viaja( struct mundo *mundo, int tempo, int idHeroi, int idNovaBase, struct lef *lef );

/* O herói morre. */
void morre( struct mundo *mundo, int tempo, int idHeroi, int idBase, int idMissao, struct lef *lef );

/* É buscada a base válida( Possui todas as habilidades ) mais próxima
 * Então, essa base realizará a missão.
 * Contudo, caso não existam bases válidas,é visto se ainda existem compostos v
 * Se existir e tempo for múltiplo de 2500, o herói mais experiente da base morre.
 * Caso tudo acima esteja errado, a missão é reajendada para o próximo dia. */
void missao( struct mundo *mundo, int tempo, int idMissao, struct lef *lef );

/* Detalha as estatísticas do programa. */
void fim( struct mundo *mundo, int tempo, struct lef *lef );

#endif