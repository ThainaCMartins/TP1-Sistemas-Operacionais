# Resultados observados — execução de desenvolvimento

Os quatro cenários de filas POSIX foram executados por aproximadamente 4 segundos cada em ambiente Linux de desenvolvimento, com logs produzidos na execução local.

| Cenário | Somas exibidas | Respostas de outro cliente | Respostas enviadas por B |
|---|---:|---:|---:|
| 1A/1B | 3 | 0 | 4 |
| 2A/1B | 3 | 0 | 4 |
| 1A/2B | 4 | 0 | 4 |
| 2A/2B | 6 | 2 | 8 |

A contagem de respostas B pode ser superior à contagem de somas exibidas por A devido ao término dos processos e ao agendamento concorrente. Na configuração 2A/2B foram observadas duas respostas consumidas pelo cliente errado. São observações de uma execução específica, não garantias determinísticas.

A Questão 1 teve testes parciais com 1 bilhão de termos no ambiente de execução, mas os coeficientes de variação superaram 5% para diversas configurações e não foram verificadas duas máquinas distintas. Esses resultados não satisfazem integralmente as exigências estatísticas do enunciado. Execute `python3 medir.py computador1` e repita em um segundo computador para obter o conjunto completo.
