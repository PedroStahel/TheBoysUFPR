#ifndef LEF
#define LEF

#include "fprio.h"
#include "entidades.h"

struct lef {
    struct fprio_t *filaEventos;
    long numEventos;
};

/* Gera alguns eventos, que serão adicionados na LEF. */
struct lef *lef_inicializa( struct mundo *mundo );

/* Administra todos os eventos da LEF. */
void lef_eventos( struct mundo *mundo, struct lef *lef );

/* Desaloca todos os componentes da LEF. 
 * Retorno: NULL */
struct lef *lef_destroi( struct lef *lef );

#endif