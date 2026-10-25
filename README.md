# TP1 — Sistemas Operacionais

**CEFET-MG · Prof.ª Michelle Hanne**  
**Dupla:** Thainá Martins e Giovanna Laura

Este repositório contém as implementações em C das duas questões do Trabalho Prático 1.

## Compilar (Linux)

```bash
make
```

## Questão 1 — π usando POSIX Threads

```bash
./pi_threads 4 1000000000
python3 medir.py computador1
```

O código usa a série de Leibniz-Grégory, soma parciais independentes e `pthread_join`. O script mede 5 execuções por configuração de 1, 2, 4, 8, 16 e 32 threads, com `/usr/bin/time -p`. Execute em um segundo computador e produza gráficos com:

```bash
python3 graficos.py medicoes_computador1.csv medicoes_computador2.csv
```

**Limitação experimental:** medições realizadas em apenas um ambiente não substituem as duas plataformas exigidas no enunciado. Refaça configurações com CV > 5%.

## Questão 2 — Filas POSIX

Em terminais separados: `./processo_b` e `./processo_a`. Pare com Ctrl+C e limpe com `./limpar_filas`. Para executar automaticamente os cenários 1A/1B, 2A/1B, 1A/2B e 2A/2B: `python3 testar_filas.py`.

A versão experimental possui uma fila de pedidos e uma fila de respostas compartilhada. Com múltiplos A, uma resposta pode ser consumida pelo cliente errado. O programa detecta a divergência pelo PID e identificador do pedido. Uma evolução seria usar uma fila de resposta exclusiva por cliente.

## Sinais

Os processos A e B tratam SIGINT/SIGTERM para encerrar. Consulte o PID com `pgrep processo_b` e experimente `kill -TERM PID`.

**Observação:** são necessários Linux com suporte a `mqueue`, GCC e Python 3; gráficos precisam de matplotlib. Logs e medições devem identificar a máquina efetivamente utilizada.
