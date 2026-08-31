#include <stdlib.h>
#include "entidades.h"
#include "fila.h"
#include "conjunto.h"

struct mundo *destroiMundo( struct mundo *mundo ) {
    int i;

    /* Percorre todos os heróis e vai desalocando seus componentes. */
    for ( i = 0; i < mundo -> numHerois; i++ ) {
        mundo -> herois[i] -> habilidades = cjto_destroi( mundo -> herois[i] -> habilidades );
        
        free( mundo -> herois[i] );
        mundo -> herois[i] = NULL;
    }
    /* Desaloca o vetor de heróis em si. */
    free( mundo -> herois );
    mundo -> herois = NULL;

    /* Percorre todas as bases e vai desalocando seus componentes. */
    for ( i = 0; i < mundo -> numBases; i++ ) {
        mundo -> bases[i] -> presentes = cjto_destroi( mundo -> bases[i] -> presentes );
        
        mundo -> bases[i] -> fila = fila_destroi( mundo -> bases[i] -> fila );
        
        free( mundo -> bases[i] -> local );
        mundo -> bases[i] -> local = NULL;

        free( mundo -> bases[i] );
        mundo -> bases[i] = NULL;
    }
    /* Desaloca o vetor de bases em si. */
    free( mundo -> bases );
    mundo -> bases = NULL;

    /* Percorre todas as missões e vai desalocando seus componentes. */
    for ( i = 0; i < mundo -> numMissoes; i++ ) {
        mundo -> missoes[i] -> habilidades = cjto_destroi( mundo -> missoes[i] -> habilidades );
        
        free( mundo -> missoes[i] -> local );
        mundo -> missoes[i] -> local = NULL;

        free( mundo -> missoes[i] );
        mundo -> missoes[i] = NULL;
    }
    /* Desaloca o vetor de missões em si. */
    free( mundo -> missoes );
    mundo -> missoes = NULL;

    /* Desaloca a posição do mundo. */
    free( mundo -> tamMundo );
    mundo -> tamMundo = NULL;

    /* Desaloca o mundo em si. */
    free( mundo );
    mundo = NULL;

    return mundo;
}
