#define _POSIX_C_SOURCE 200809L
#include "protocolo.h"
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
static volatile sig_atomic_t ativo = 1;
static void parar(int s) { (void)s; ativo = 0; }
int main(void) {
    signal(SIGINT, parar); signal(SIGTERM, parar);
    struct mq_attr req = { .mq_maxmsg = 10, .mq_msgsize = sizeof(Pedido) };
    struct mq_attr resp = { .mq_maxmsg = 10, .mq_msgsize = sizeof(Resposta) };
    mqd_t pedidos = mq_open(FILA_PEDIDOS, O_CREAT | O_WRONLY, 0600, &req);
    mqd_t respostas = mq_open(FILA_RESPOSTAS, O_CREAT | O_RDONLY, 0600, &resp);
    if (pedidos == (mqd_t)-1 || respostas == (mqd_t)-1) { perror("mq_open"); return 1; }
    unsigned int seed = (unsigned int)time(NULL) ^ (unsigned int)getpid();
    unsigned long id = 0;
    while (ativo) {
        Pedido p = { .n1 = (int)(rand_r(&seed)%100), .n2 = (int)(rand_r(&seed)%100), .cliente = getpid(), .id = ++id };
        if (mq_send(pedidos, (char*)&p, sizeof(p), 0) == -1) { if (errno == EINTR) continue; perror("mq_send"); break; }
        Resposta r;
        if (mq_receive(respostas, (char*)&r, sizeof(r), NULL) == -1) { if (errno == EINTR) continue; perror("mq_receive"); break; }
        if (r.cliente != p.cliente || r.id != p.id)
            printf("[A pid=%ld] RESPOSTA DE OUTRO CLIENTE! esperado id=%lu; recebido pid=%ld id=%lu\n", (long)getpid(), p.id, (long)r.cliente, r.id);
        else printf("[A pid=%ld] %d + %d = %d\n", (long)getpid(), p.n1, p.n2, r.soma);
        fflush(stdout);
    }
    mq_close(pedidos); mq_close(respostas);
    return 0;
}
