#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "entidades.h"
#include "fila.h"
#include "eventos.h"
#include "conjunto.h"
#include "fprio.h"
#include "lef.h"

void chega( struct mundo *mundo, int tempo, int idHeroi, int idBase, struct lef *lef ) {
    int espera, tamFila, qtdBase;
    struct evento *eventoNovo;

    if ( !( eventoNovo = malloc( sizeof( struct evento ) ) ) )
        return;

    /* Inicializa o novo evento. */
    eventoNovo -> heroi = mundo -> herois[idHeroi];
    eventoNovo -> heroi -> base = idBase;
    eventoNovo -> base = mundo -> bases[idBase];
    eventoNovo -> missao = NULL;

    tamFila = fila_tamanho( eventoNovo -> base -> fila );
    /* Se houver erro na checagem da cardinalidade, encerra a função. */
    if ( ( qtdBase = cjto_card( eventoNovo -> base -> presentes ) ) == -1 ) {
        free( eventoNovo );

        return;
    }

    /* Se ainda houver espaço espaço na base e o tamanho fila for 0, o herói espera. 
     * Senão, é feito um cálculo para ver se ele espera ou não. */
    if ( qtdBase < eventoNovo -> base -> lotacao && tamFila == 0 )
        espera = 1;
    else
        espera = eventoNovo -> heroi -> paciencia > ( 10 * tamFila );

    lef -> numEventos++;

    /* Se espera, adiciona ESPERA na LEF. 
     * Caso contrário, adiciona DESISTE na LEF. */
    if ( espera ) {
        if ( fprio_insere( lef -> filaEventos, eventoNovo, ESPERA, tempo ) == -1 ) {
            free( eventoNovo );
            eventoNovo = NULL;

            return;
        }

        printf( "%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) ESPERA\n", tempo, idHeroi, idBase,
        qtdBase, eventoNovo -> base -> lotacao );

        return;
    }

    if ( fprio_insere( lef -> filaEventos, eventoNovo, DESISTE, tempo ) == -1 ) {
        free( eventoNovo );
        eventoNovo = NULL;

        return;
    }

    printf( "%6d: CHEGA HEROI %2d BASE %d (%2d/%2d) DESISTE\n", tempo, idHeroi, idBase,
    qtdBase, eventoNovo -> base -> lotacao );

}

void avisa( struct mundo *mundo, int tempo, int idBase, struct lef *lef ) {
    int tamFila = fila_tamanho( mundo -> bases[idBase] -> fila );
    int numHeroisBase = cjto_card( mundo -> bases[idBase] -> presentes );
    int idHeroiAdd;

    printf( "%6d: AVISA PORTEIRO BASE %d (%2d/%2d) FILA [ ", tempo, idBase,
    numHeroisBase, mundo -> bases[idBase] -> lotacao );
    fila_imprime( mundo -> bases[idBase] -> fila );
    printf( " ]\n" );

    /* Enquanto houver espaço e a fila existir, adiciona ENTRA na LEF. */
    while ( numHeroisBase < mundo -> bases[idBase] -> lotacao && tamFila > 0 ) {
        struct evento *eventoNovo;

        if ( !( eventoNovo = malloc( sizeof( struct evento ) ) ) )
            return;

        eventoNovo -> base = mundo -> bases[idBase];
        eventoNovo -> missao = NULL; 
        
        if ( !fila_retira( eventoNovo -> base -> fila, &idHeroiAdd ) ) {
            free( eventoNovo );
            eventoNovo = NULL;
            
            return;
        }
        printf( "%6d: AVISA PORTEIRO BASE %d ADMITE %2d\n", tempo, idBase, idHeroiAdd );

        tamFila--;
        eventoNovo -> heroi = mundo -> herois[idHeroiAdd];
        if ( cjto_insere( eventoNovo -> base -> presentes, idHeroiAdd ) == -1 ) {
            free( eventoNovo );

            return;
        }
        
        if ( fprio_insere( lef -> filaEventos, eventoNovo, ENTRA, tempo ) == -1 ) {
            free( eventoNovo );
            eventoNovo = NULL;
            
            return;
        }
        numHeroisBase++;

    }
    lef -> numEventos++;
    
}

void espera( struct mundo *mundo, int tempo, int idHeroi, int idBase, struct lef *lef ) {
    int tamFila;
    struct evento *eventoNovo;
    
    if ( !( eventoNovo = malloc( sizeof( struct evento ) ) ) )
        return;

    eventoNovo -> heroi = NULL;
    eventoNovo -> base = mundo -> bases[idBase];
    eventoNovo -> missao = NULL;

    /* Adiciona o herói na fila e avisa o porteiro, adicionando AVISA na LEF. */
    if ( fila_insere( eventoNovo -> base -> fila, idHeroi ) ) {
        tamFila = fila_tamanho( eventoNovo -> base -> fila );
        printf( "%6d: ESPERA HEROI %2d BASE %d (%2d)\n", tempo, idHeroi, idBase, tamFila - 1 );
        /* Checa se a fila atual não é a maior fila que a base já teve. */
        if ( eventoNovo -> base -> filaMaxima < tamFila )
            eventoNovo -> base -> filaMaxima = tamFila;

        if ( fprio_insere( lef -> filaEventos, eventoNovo, AVISA, tempo ) == -1 ) {
            free( eventoNovo );
            eventoNovo = NULL;

            return;
        }

        lef -> numEventos++;

        return;
    }    
    
    free( eventoNovo );
    eventoNovo = NULL;

}

void desiste( struct mundo *mundo, int tempo, int idHeroi, struct lef *lef ) {
    /* Acha um id aleatório de uma nova base. */
    int idBaseAtual, idNovaBase = rand() % mundo -> numBases;
    struct evento *eventoNovo;

    if ( !( eventoNovo = malloc( sizeof( struct evento ) ) ) )
        return;

    eventoNovo -> heroi = mundo -> herois[idHeroi];
    eventoNovo -> base = mundo -> bases[idNovaBase];
    eventoNovo -> missao = NULL;

    idBaseAtual = eventoNovo -> heroi -> base;

    /* Adiciona VIAJA na LEF. */
    if ( fprio_insere( lef -> filaEventos, eventoNovo, VIAJA, tempo ) == -1 ) {
        free( eventoNovo );
        eventoNovo = NULL;

        return;
    }
    lef -> numEventos++;
    printf( "%6d: DESISTE HEROI %2d BASE %d\n", tempo, idHeroi, idBaseAtual );

}

void entra( struct mundo *mundo, int tempo, int idHeroi, int idBase, struct lef *lef ) {
    int qtdBase, lotacaoBase;
    int tpb; /* Tempo de permanência na base. */
    struct evento *eventoNovo;

    if ( !( eventoNovo = malloc( sizeof( struct evento ) ) ) )
        return;

    eventoNovo -> heroi = mundo -> herois[idHeroi];
    eventoNovo -> base = mundo -> bases[idBase];
    eventoNovo -> missao = NULL;
    qtdBase = cjto_card( eventoNovo -> base -> presentes );
    lotacaoBase = eventoNovo -> base -> lotacao;

    tpb = 15 + eventoNovo -> heroi -> paciencia * ( rand() % 20 + 1 );

    /* Adiciona o evento SAI na LEF, que será executado após "acabar" o tpb. */
    if ( fprio_insere( lef -> filaEventos, eventoNovo, SAI, tempo + tpb ) == -1 ) {
        free( eventoNovo );
        eventoNovo = NULL;

        return;
    }
    lef -> numEventos++;
    printf( "%6d: ENTRA HEROI %2d BASE %d (%2d/%2d) SAI %d\n", tempo, idHeroi, idBase, qtdBase,
    lotacaoBase, tempo + tpb );

}

void sai( struct mundo *mundo, int tempo, int idHeroi, int idBase, struct lef *lef ) {
    int idNovaBase, qtdBase;
    struct evento *eventoNovo1, *eventoNovo2;

    if ( !( eventoNovo1 = malloc( sizeof( struct evento ) ) ) )
        return;

    eventoNovo1 -> heroi = mundo -> herois[idHeroi];
    eventoNovo1 -> base = mundo -> bases[idBase];
    eventoNovo1 -> missao = NULL;

    /* Retira o herói da base e acha uma nova para ele. */
    if ( cjto_retira( eventoNovo1 -> base -> presentes, idHeroi ) != -1 ) {
        idNovaBase = rand() % mundo -> numBases;
        eventoNovo1 -> base = mundo -> bases[idNovaBase];
        /* Adiciona o evento VIAJA na LEF. */
        if ( fprio_insere( lef -> filaEventos, eventoNovo1, VIAJA, tempo ) == -1 ) {
            free( eventoNovo1 );
            eventoNovo1 = NULL;

            return;
        }
        if ( !( eventoNovo2 = malloc( sizeof( struct evento ) ) ) )
            return;

        eventoNovo2 -> heroi = mundo -> herois[idHeroi];
        eventoNovo2 -> base = mundo -> bases[idBase];
        eventoNovo2 -> missao = NULL;

        /* Adiciona o evento AVISA na LEF. */
        if ( fprio_insere( lef -> filaEventos, eventoNovo2, AVISA, tempo ) == -1 ) {
            free( eventoNovo2 );
            eventoNovo2 = NULL;

            return;
        }
        lef -> numEventos++;
        qtdBase = cjto_card( eventoNovo2 -> base -> presentes );
        printf( "%6d: SAI HEROI %2d BASE %d (%2d/%2d)\n", tempo, idHeroi, idBase,
        qtdBase, eventoNovo2 -> base -> lotacao );

        return;
    }
    
    free( eventoNovo1 );
    eventoNovo1 = NULL;

}

/* Calcula a distância entre certos locais 
 * Baseando-se no cálculo de distância entre pontos no plano cartesiano. */
int calculaDistancia( struct posicao *novaBase, struct posicao *baseAtual ) {
    int distanciaX, distanciaY, distanciaTotal;

    distanciaX = novaBase -> x - baseAtual -> x;
    distanciaY = novaBase -> y - baseAtual -> y;

    distanciaTotal = sqrt( distanciaX * distanciaX + distanciaY * distanciaY );

    return distanciaTotal;

}

void viaja( struct mundo *mundo, int tempo, int idHeroi, int idNovaBase, struct lef *lef ) {
    int distancia, duracao;
    struct base *baseAtual = mundo -> bases[mundo -> herois[idHeroi] -> base];
    struct evento *eventoNovo;

    if ( !( eventoNovo = malloc( sizeof( struct evento ) ) ) )
        return;

    eventoNovo -> heroi = mundo -> herois[idHeroi];
    eventoNovo -> base = mundo -> bases[idNovaBase];
    eventoNovo -> missao = NULL;

    distancia = calculaDistancia( eventoNovo -> base -> local, baseAtual -> local );
    duracao = distancia / eventoNovo -> heroi -> velocidade;

    /* Insere o evento CHEGA na LEF
     * Ele será executado somente após a duração da viagem. */
    if ( fprio_insere( lef -> filaEventos, eventoNovo, CHEGA, tempo + duracao ) == -1 ) {
        free( eventoNovo );
        eventoNovo = NULL;

        return;
    }
    lef -> numEventos++;
    printf( "%6d: VIAJA HEROI %2d BASE %d DIST %d VEL %d CHEGA %d\n", tempo, idHeroi, idNovaBase,
    distancia, eventoNovo -> heroi -> velocidade, tempo + duracao );

}

void morre( struct mundo *mundo, int tempo, int idHeroi, int idBase, int idMissao, struct lef *lef ) {
    struct evento *eventoNovo;

    if ( !( eventoNovo = malloc( sizeof( struct evento ) ) ) )
        return;

    eventoNovo -> heroi = NULL;
    eventoNovo -> base = mundo -> bases[idBase];
    eventoNovo -> missao = NULL;

    /* Retira o herói morto da base dele. */
    if ( cjto_retira( eventoNovo -> base -> presentes, idHeroi ) == -1 ) {
        free( eventoNovo );
        eventoNovo = NULL;

        return;
    }

    mundo -> herois[idHeroi] -> vivo = 0;

    /* Insere o evento AVISA na LEF. */
    if ( fprio_insere( lef -> filaEventos, eventoNovo, AVISA, tempo ) == -1 ) {
        free( eventoNovo );
        eventoNovo = NULL;

        return;
    }

    lef -> numEventos++;
    printf( "%6d: MORRE HEROI %2d MISSAO %d\n", tempo, idHeroi, idMissao );

}

/* Incrementa a experiência de todos os heróis presentes em certa base. */
void incrementaExperiencia( struct mundo *mundo, int idBase ) {
    int i;

    for ( i = 0; i < mundo -> numHerois; i++ ) {
        if ( mundo -> herois[i] -> vivo && cjto_pertence( mundo -> bases[idBase] -> presentes, i ) == 1 )
            mundo -> herois[i] -> xp++;
    }
}

/* Checa se a base contem todas as habilidades necessárias para certa missão. 
 * Retorna 1 se contiver e 0 caso contrário. */
int checaBase( struct mundo *mundo, int idBase, int idMissao ) {
    int i, checaContem;
    /* O aux é necessário para não sobreescrever o habilidadesBase 
     * Ou seja, serve para que não haja leak de memória. */
    struct cjto_t *habilidadesBase, *aux = NULL;

    if ( !( habilidadesBase = cjto_cria( N_HABILIDADES ) ) )
        return 0;

    /* Se herói estiver na base, as suas habilidades são adicionadas ao conjunto. */
    for ( i = 0; i < mundo -> numHerois; i++ ) {
        if ( mundo -> herois[i] -> vivo && cjto_pertence( mundo -> bases[idBase] -> presentes, i ) == 1 ) {
            if ( !( aux = cjto_uniao( habilidadesBase, mundo -> herois[i] -> habilidades ) ) ) {
                cjto_destroi( habilidadesBase );

                return 0;
            }

            cjto_destroi( habilidadesBase );
            
            habilidadesBase = aux;
        }
    }
    
    /* Analisa se a base contém todas as habilidades necessárias. */
    checaContem = cjto_contem( habilidadesBase, mundo -> missoes[idMissao] -> habilidades );
    
    habilidadesBase = cjto_destroi( habilidadesBase );

    if ( checaContem == 1 )
        return checaContem;
    
    return 0;

}

/* Encontra o herói mais experiente de uma certa base. */
struct heroi *heroiExperiente( struct mundo *mundo, int idBase ) {
    int i, xpMaior = -1, idHeroiXP = -1;

    for ( i = 0; i < mundo -> numHerois; i++ ) {
        if ( mundo -> herois[i] -> vivo && cjto_pertence( mundo -> bases[idBase] -> presentes, i ) == 1 )
            if ( mundo -> herois[i] -> xp > xpMaior ) {
                xpMaior = mundo -> herois[i] -> xp;
                idHeroiXP = i;
            }
    }

    if ( idHeroiXP != -1 )
        return mundo -> herois[idHeroiXP];

    return NULL;
}

void missao( struct mundo *mundo, int tempo, int idMissao, struct lef *lef ) {
    int menorDistancia, i, distancia, basesValidas[N_BASES],
    tam = 0, idBasePerto;
    struct posicao *localMissao = mundo -> missoes[idMissao] -> local;
    struct evento *eventoNovo;
    struct cjto_t *habilidadesBase, *aux = NULL;

    /* Percorre todas as bases para analisar se são válidas ou não. 
     * Caso alguma seja, o seu id é colocado no vetor. */
    for ( i = 0; i < mundo -> numBases; i++ ) {
        if ( checaBase( mundo, i, idMissao ) ) {
            basesValidas[tam] = i;
            tam++;
        }
    }

    mundo -> missoes[idMissao] -> tentativasFeitas++;
    printf( "%6d: MISSAO %d TENT %d HAB REQ: [ ", tempo, idMissao,
            mundo -> missoes[idMissao] -> tentativasFeitas );
    cjto_imprime( mundo -> missoes[idMissao] -> habilidades );
    printf( " ]\n" );

    /* Entra no if se houver alguma base válida. 
     * Caso haja, ele checa todas para encontrar aquela mais próxima da missão. */
    if ( tam > 0 ) {
        menorDistancia = calculaDistancia( mundo -> bases[basesValidas[0]] -> local, localMissao );
        idBasePerto = basesValidas[0];
        for ( i = 1; i < tam; i++ ) {
            distancia = calculaDistancia( mundo -> bases[basesValidas[i]] -> local, localMissao );
            if ( distancia < menorDistancia ) {
                idBasePerto = basesValidas[i];
                menorDistancia = distancia;
            }
        }

        mundo -> missoes[idMissao] -> cumprida = 1;
        mundo -> bases[idBasePerto] -> missoesRealizadas++;

        if ( !( habilidadesBase = cjto_cria( N_HABILIDADES ) ) )
            return;
        
        /* Checa quais são todas as habilidades da base válida mais próxima da missão. */
        for ( i = 0; i < mundo -> numHerois; i++ ) {
            if ( mundo -> herois[i] -> vivo && cjto_pertence( mundo -> bases[idBasePerto] -> presentes, i ) == 1 ) {
                if ( !( aux = cjto_uniao( habilidadesBase, mundo -> herois[i] -> habilidades ) ) ) {
                    cjto_destroi( habilidadesBase );

                    return;
                }

                cjto_destroi( habilidadesBase );
            
                habilidadesBase = aux;
            }
        }

        incrementaExperiencia( mundo, idBasePerto );
        
        printf( "%6d: MISSAO %d CUMPRIDA BASE %d HABS: [ ", tempo, idMissao, idBasePerto );
        cjto_imprime( habilidadesBase );
        printf( " ]\n" );

        habilidadesBase = cjto_destroi( habilidadesBase );

        return;
    }

    if ( !( eventoNovo = malloc( sizeof( struct evento ) ) ) )
        return;

    eventoNovo -> missao = mundo -> missoes[idMissao];

    /* Vê se existem compostos v e se tempo é múltiplo de 2500. 
     * Após essas checagens, verifica qual é a base mais próxima e que tenha heróis presentes. */
    if ( mundo -> numCompV > 0 && tempo % 2500 == 0 ) {
        mundo -> numCompV--;
        mundo -> missoes[idMissao] -> cumprida = 1;

        idBasePerto = -1;
        /* Inicializa menorDistancia com a maior distância possível. */
        menorDistancia = TAMANHO_MUNDO;
        for ( i = 0; i < N_BASES; i++ ) {
            distancia = calculaDistancia( mundo -> bases[i] -> local, localMissao );
            if ( cjto_card( mundo -> bases[i] -> presentes ) > 0 && distancia < menorDistancia ) {
                idBasePerto = i;
                menorDistancia = distancia;
            }
        }

        /* Entra aqui somente se não existe nenhuma base com heróis nela. */
        if ( menorDistancia == TAMANHO_MUNDO ) {
            free( eventoNovo );

            return;
        }

        mundo -> bases[idBasePerto] -> missoesRealizadas++;

        if ( !( eventoNovo -> heroi = heroiExperiente( mundo, idBasePerto ) ) ) {
            free( eventoNovo );
            
            return;
        }
            
        eventoNovo -> base = mundo -> bases[idBasePerto];

        /* Insere o evento MORRE na LEF, que matará o herói mais experiente da base. */
        if ( fprio_insere( lef -> filaEventos, eventoNovo, MORRE, tempo ) == -1 ) {
            free( eventoNovo );
            eventoNovo = NULL;

            return;
        }

        incrementaExperiencia( mundo, idBasePerto );
        
        printf( "%6d: MISSAO %d CUMPRIDA BASE %d HABS: [ ", tempo, idMissao, idBasePerto );
        cjto_imprime( mundo -> missoes[idMissao] -> habilidades );
        printf( " ]\n" );

        return;
    }

    eventoNovo -> heroi = NULL;
    eventoNovo -> base = NULL;
    /* Insere na LEF o evento MISSAO novamente, sinalizando que a missão era impossível. 
     * Além de que, ela será executada novamente só no dia seguinte. */
    if ( fprio_insere( lef -> filaEventos, eventoNovo, MISSAO, tempo + 24*60 ) == -1 ) {
        free( eventoNovo );
        eventoNovo = NULL;

        return;
    }

    printf( "%6d: MISSAO %d IMPOSSIVEL\n", tempo, idMissao );

}

void fim( struct mundo *mundo, int tempo, struct lef *lef ) {
    int i, missoesCumpridas = 0, heroisMortos = 0, maximoMissoes, minimoMissoes, totalTentativas = 0;

    /* Checa a quantidade de missões cumpridas no total. */
    for ( i = 0; i < N_MISSOES; i++ )
        if ( mundo -> missoes[i] -> cumprida )
            missoesCumpridas++;

    /* Checa a quantidade de heróis mortos no total. */
    for ( i = 0; i < N_HEROIS; i++ )
        if ( mundo -> herois[i] -> vivo == 0 )
            heroisMortos++;

    maximoMissoes = mundo -> missoes[0] -> tentativasFeitas;
    minimoMissoes = mundo -> missoes[0] -> tentativasFeitas;
    totalTentativas = totalTentativas + mundo -> missoes[0] -> tentativasFeitas;
    /* Calcula o total de tentativas feitas para as missões. 
     * Além disso, calcula a quantidade máxima e mínima de tentativas para as missões. */
    for ( i = 1; i < N_MISSOES; i++ ) {
        totalTentativas = totalTentativas + mundo -> missoes[i] -> tentativasFeitas;
        if ( mundo -> missoes[i] -> tentativasFeitas > maximoMissoes )
            maximoMissoes = mundo -> missoes[i] -> tentativasFeitas;
        else if ( mundo -> missoes[i] -> tentativasFeitas < minimoMissoes )
            minimoMissoes = mundo -> missoes[i] -> tentativasFeitas;
    }

    printf( "%d: FIM\n", tempo );

    /* Imprime as estatísticas de todos os heróis. */
    for ( i = 0; i < N_HEROIS; i++ ) {
        printf( "HEROI %2d ", i );
        if ( mundo -> herois[i] -> vivo )
            printf( "VIVO PAC %3d VEL %4d EXP %4d HABS [ ", mundo -> herois[i] -> paciencia,
            mundo -> herois[i] -> velocidade, mundo -> herois[i] -> xp );
        else
            printf( "MORTO PAC %3d VEL %4d EXP %4d HABS [ ", mundo -> herois[i] -> paciencia,
            mundo -> herois[i] -> velocidade, mundo -> herois[i] -> xp );
        cjto_imprime( mundo -> herois[i] -> habilidades );
        printf( " ]\n" );
    }
    printf( "\n" );

    /* Imprime as estatísticas de todas as bases. */
    for ( i = 0; i < N_BASES; i++ ) {
        printf( "BASE %2d LOT %2d FILA MAX %2d MISSOES %d\n", i, mundo -> bases[i] -> lotacao,
        mundo -> bases[i] -> filaMaxima, mundo -> bases[i] -> missoesRealizadas );
    }
    printf( "\n" );

    printf( "EVENTOS TRATADOS: %ld\n", lef -> numEventos );
    
    printf( "MISSOES CUMPRIDAS: %d/%d (%.1f%%)\n", missoesCumpridas, N_MISSOES,
    ( ( float ) missoesCumpridas / N_MISSOES ) * 100.0 );
    
    printf( "TENTATIVAS/MISSAO: MIN %d, MAX %d, MEDIA %.1f\n", minimoMissoes, maximoMissoes,
    ( ( float ) totalTentativas / N_MISSOES ) );
    
    printf( "TAXA MORTALIDADE: %.1f%%\n", ( ( float ) heroisMortos / N_HEROIS ) * 100.0 );

}