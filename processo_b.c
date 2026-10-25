#define _POSIX_C_SOURCE 200809L
#include "protocolo.h"
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
static volatile sig_atomic_t ativo = 1;
static void parar(int s) { (void)s; ativo = 0; }
int main(void) {
    signal(SIGINT, parar); signal(SIGTERM, parar);
    struct mq_attr req = { .mq_maxmsg = 10, .mq_msgsize = sizeof(Pedido) };
    struct mq_attr resp = { .mq_maxmsg = 10, .mq_msgsize = sizeof(Resposta) };
    mqd_t pedidos = mq_open(FILA_PEDIDOS, O_CREAT | O_RDONLY, 0600, &req);
    mqd_t respostas = mq_open(FILA_RESPOSTAS, O_CREAT | O_WRONLY, 0600, &resp);
    if (pedidos == (mqd_t)-1 || respostas == (mqd_t)-1) { perror("mq_open"); return 1; }
    while (ativo) {
        Pedido p;
        if (mq_receive(pedidos, (char*)&p, sizeof(p), NULL) == -1) { if (errno == EINTR) continue; perror("mq_receive"); break; }
        Resposta r = { .soma = p.n1 + p.n2, .cliente = p.cliente, .id = p.id };
        sleep(1);
        if (mq_send(respostas, (char*)&r, sizeof(r), 0) == -1) { if (errno == EINTR) continue; perror("mq_send"); break; }
        printf("[B pid=%ld] respondeu a A pid=%ld id=%lu\n", (long)getpid(), (long)p.cliente, p.id);
        fflush(stdout);
    }
    mq_close(pedidos); mq_close(respostas);
    return 0;
}
