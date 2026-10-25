#!/usr/bin/env python3
"""Uso: python3 graficos.py medicoes_pc1.csv medicoes_pc2.csv"""
import csv, sys, statistics
from collections import defaultdict
import matplotlib.pyplot as plt
if len(sys.argv)<3: sys.exit('Informe os CSVs de duas máquinas distintas.')
dados=defaultdict(list)
for arquivo in sys.argv[1:]:
    with open(arquivo,newline='') as f:
        for r in csv.DictReader(f): dados[(r['maquina'],int(r['threads']))].append(float(r['segundos']))
maquinas=sorted({k[0] for k in dados})
with open('resumo.csv','w',newline='') as f:
    w=csv.writer(f); w.writerow(['maquina','threads','media_s','desvio_s','cv_percent','tempo_normalizado'])
    for m in maquinas:
        base=statistics.mean(dados[m,1])
        for n in (1,2,4,8,16,32):
            v=dados[m,n]; media=statistics.mean(v); desvio=statistics.stdev(v)
            w.writerow([m,n,media,desvio,100*desvio/media,media/base])
for m in maquinas:
    xs=[1,2,4,8,16,32]; ys=[statistics.mean(dados[m,n]) for n in xs]
    plt.figure(); plt.plot(xs,ys,marker='o'); plt.xticks(xs); plt.xlabel('Número de threads'); plt.ylabel('Tempo médio (s)'); plt.title(f'Tempo médio — {m}'); plt.grid(alpha=.3); plt.tight_layout(); plt.savefig(f'tempo_{m}.png',dpi=180); plt.close()
plt.figure()
for m in maquinas:
    base=statistics.mean(dados[m,1]); plt.plot([1,2,4,8,16,32],[statistics.mean(dados[m,n])/base for n in (1,2,4,8,16,32)],marker='o',label=m)
plt.xticks([1,2,4,8,16,32]); plt.xlabel('Número de threads'); plt.ylabel('Tempo normalizado (Tn/T1)'); plt.title('Comparação normalizada'); plt.legend(); plt.grid(alpha=.3); plt.tight_layout(); plt.savefig('tempos_normalizados.png',dpi=180)
print('Gerados resumo.csv e três gráficos.')
