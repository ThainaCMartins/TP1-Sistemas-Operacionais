#ifndef PROTOCOLO_H
#define PROTOCOLO_H
#include <sys/types.h>
#define FILA_PEDIDOS "/tp1so_pedidos_2026"
#define FILA_RESPOSTAS "/tp1so_respostas_2026"
typedef struct { int n1, n2; pid_t cliente; unsigned long id; } Pedido;
typedef struct { int soma; pid_t cliente; unsigned long id; } Resposta;
#endif
