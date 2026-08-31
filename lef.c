#include <stdlib.h>
#include "entidades.h"
#include "fprio.h"
#include "lef.h"
#include "eventos.h"

struct lef *lef_inicializa( struct mundo *mundo ) {
    int i, idBase, tempo;
    struct lef *lef;
    struct evento *eventoVazio;

    if ( !( lef = malloc( sizeof( struct lef ) ) ) )
        return NULL;

    if ( !( lef -> filaEventos = fprio_cria() ) )
        return NULL;

    lef -> numEventos = 0;

    /* Para todos os heróis, é adicionado o evento CHEGA na LEF para uma base aleatória. */
    for ( i = 0; i < mundo -> numHerois; i++ ) {
        struct evento *eventoNovo;

        if ( !( eventoNovo = malloc( sizeof( struct evento ) ) ) )
            return NULL;
        
        idBase = rand() % mundo -> numBases;
        tempo = rand() % 4321;

        eventoNovo -> heroi = mundo -> herois[i];
        eventoNovo -> base = mundo -> bases[idBase];
        eventoNovo -> missao = NULL;

        if ( fprio_insere( lef -> filaEventos, eventoNovo, CHEGA, tempo ) == -1 )
            return NULL;

    }

    /* São adicionadas todas as missões na LEF, que ocorrerão em um tempo aleatório. */
    for ( i = 0; i < mundo -> numMissoes; i++ ) {
        struct evento *eventoNovo;

        if ( !( eventoNovo = malloc( sizeof( struct evento ) ) ) )
            return NULL;
        
        tempo = rand() % ( FIM_DO_MUNDO + 1 );
        
        eventoNovo -> heroi = NULL;
        eventoNovo -> base = NULL;
        eventoNovo -> missao = mundo -> missoes[i];

        if ( fprio_insere( lef -> filaEventos, eventoNovo, MISSAO, tempo ) == -1 )
            return NULL;

    }
    
    if ( !( eventoVazio = malloc( sizeof ( struct evento ) ) ) )
        return NULL;
    
    eventoVazio -> heroi = NULL;
    eventoVazio -> base = NULL;
    eventoVazio -> missao = NULL;
 
    /* É adicionado o evento FIM na LEF, o qual será o último evento de todos. */
    if ( fprio_insere( lef -> filaEventos, eventoVazio, FIM, FIM_DO_MUNDO ) == -1 )
        return NULL;

    return lef;

}

void lef_eventos( struct mundo *mundo, struct lef *lef ) {
    int tipo;
    struct evento *eventoAtual;

    /* Executa todos os eventos da LEF enquanto o fim não chega. */
    do {
        if ( !( eventoAtual = ( struct evento * ) fprio_retira( lef -> filaEventos, &tipo, &mundo -> relogio ) ) )
            return;

        switch ( tipo ) {
            case CHEGA:
                if ( eventoAtual -> heroi == NULL || eventoAtual -> base == NULL ) {
                    free( eventoAtual );

                    return;
                }
                
                chega( mundo, mundo -> relogio, eventoAtual -> heroi -> id, eventoAtual -> base -> id, lef );
                break;
            case AVISA:
                if ( eventoAtual -> base == NULL ) {
                    free( eventoAtual );

                    return;
                }
                
                avisa( mundo, mundo -> relogio, eventoAtual -> base -> id, lef );
                break;
            case ESPERA:
                if ( eventoAtual -> heroi == NULL || eventoAtual -> base == NULL ) {
                    free( eventoAtual );

                    return;
                }

                espera( mundo, mundo -> relogio, eventoAtual -> heroi -> id, eventoAtual -> base -> id, lef );
                break;
            case DESISTE:
                if ( eventoAtual -> heroi == NULL ) {
                    free( eventoAtual );

                    return;
                }
                
                desiste( mundo, mundo -> relogio, eventoAtual -> heroi -> id, lef );
                break;
            case ENTRA:
                if ( eventoAtual -> heroi == NULL || eventoAtual -> base == NULL ) {
                    free( eventoAtual );

                    return;
                }

                entra( mundo, mundo -> relogio, eventoAtual -> heroi -> id, eventoAtual -> base -> id, lef );
                break;
            case SAI:
                if ( eventoAtual -> heroi == NULL || eventoAtual -> base == NULL ) {
                    free( eventoAtual );

                    return;
                }

                sai( mundo, mundo -> relogio, eventoAtual -> heroi -> id, eventoAtual -> base -> id, lef );
                break;
            case VIAJA:
                if ( eventoAtual -> heroi == NULL || eventoAtual -> base == NULL ) {
                    free( eventoAtual );

                    return;
                }

                viaja( mundo, mundo -> relogio, eventoAtual -> heroi -> id, eventoAtual -> base -> id, lef );
                break;
            case MORRE:
                if ( eventoAtual -> heroi == NULL || eventoAtual -> base == NULL || eventoAtual -> missao == NULL ) {
                    free( eventoAtual );

                    return;
                }

                morre( mundo, mundo -> relogio, eventoAtual -> heroi -> id, eventoAtual -> base -> id, eventoAtual -> missao -> id, lef );
                break;
            case MISSAO:
                if ( eventoAtual -> missao == NULL ) {
                    free( eventoAtual );

                    return;
                }

                missao( mundo, mundo -> relogio, eventoAtual -> missao -> id, lef );
                break;
            case FIM:
                fim( mundo, mundo -> relogio, lef );
                break;
        }
        if ( eventoAtual != NULL )
            free( eventoAtual );

    } while ( tipo != FIM );

}

struct lef *lef_destroi( struct lef *lef ) {
    lef -> filaEventos = fprio_destroi( lef -> filaEventos );
    
    free( lef );
    lef = NULL;

    return lef;

}