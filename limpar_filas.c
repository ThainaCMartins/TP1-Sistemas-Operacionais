#include "protocolo.h"
#include <mqueue.h>
#include <stdio.h>
int main(void) {
    if (mq_unlink(FILA_PEDIDOS) == -1) perror("mq_unlink pedidos");
    if (mq_unlink(FILA_RESPOSTAS) == -1) perror("mq_unlink respostas");
    return 0;
}
