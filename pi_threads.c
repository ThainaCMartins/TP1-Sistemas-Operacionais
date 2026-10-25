#define _POSIX_C_SOURCE 200809L
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>

typedef struct { uint64_t inicio, fim; double parcial; } Tarefa;
static void *calcular(void *arg) {
    Tarefa *t = arg;
    double soma = 0.0;
    for (uint64_t k = t->inicio; k < t->fim; ++k)
        soma += (k & 1ULL ? -1.0 : 1.0) / (2.0 * (double)k + 1.0);
    t->parcial = soma;
    return NULL;
}
static double segundos(void) {
    struct timespec t;
    if (clock_gettime(CLOCK_MONOTONIC, &t) != 0) { perror("clock_gettime"); exit(1); }
    return (double)t.tv_sec + (double)t.tv_nsec / 1e9;
}
int main(int argc, char **argv) {
    if (argc < 2 || argc > 3) { fprintf(stderr, "Uso: %s THREADS [TERMOS]\n", argv[0]); return 1; }
    char *end;
    errno = 0;
    long n = strtol(argv[1], &end, 10);
    if (errno || *end || n < 1 || n > 1024) { fprintf(stderr, "THREADS invalido\n"); return 1; }
    uint64_t termos = 1000000000ULL;
    if (argc == 3) {
        errno = 0;
        unsigned long long val = strtoull(argv[2], &end, 10);
        if (errno || *end || val < 1 || argv[2][0] == '-') { fprintf(stderr, "TERMOS invalido\n"); return 1; }
        termos = (uint64_t)val;
    }
    pthread_t *threads = calloc((size_t)n, sizeof(*threads));
    Tarefa *tarefas = calloc((size_t)n, sizeof(*tarefas));
    if (!threads || !tarefas) { perror("calloc"); free(threads); free(tarefas); return 1; }
    uint64_t base = termos / (uint64_t)n, resto = termos % (uint64_t)n, cursor = 0;
    double t0 = segundos();
    for (long i = 0; i < n; ++i) {
        uint64_t tamanho = base + ((uint64_t)i < resto ? 1 : 0);
        tarefas[i].inicio = cursor;
        tarefas[i].fim = cursor + tamanho;
        cursor += tamanho;
        int rc = pthread_create(&threads[i], NULL, calcular, &tarefas[i]);
        if (rc) { fprintf(stderr, "pthread_create falhou (%d)\n", rc); exit(1); }
    }
    for (long i = 0; i < n; ++i) pthread_join(threads[i], NULL);
    double soma = 0.0;
    for (long i = 0; i < n; ++i) soma += tarefas[i].parcial;
    double tempo = segundos() - t0;
    printf("threads=%ld termos=%llu pi=%.15f tempo_interno=%.6f\n", n, (unsigned long long)termos, 4.0*soma, tempo);
    free(threads); free(tarefas);
    return 0;
}
