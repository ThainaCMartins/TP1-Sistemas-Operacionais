#!/usr/bin/env python3
"""Uso: python3 medir.py IDENTIFICADOR_MAQUINA"""
import csv, statistics, subprocess, sys, platform
from pathlib import Path
if len(sys.argv) != 2:
    sys.exit('Uso: python3 medir.py IDENTIFICADOR_MAQUINA')
maquina = sys.argv[1]
print('Máquina:', maquina, platform.platform(), 'CPU:', platform.processor())
print('Verifique a carga com top antes de iniciar.')
saida = Path(f'medicoes_{maquina}.csv')
with saida.open('w', newline='') as f:
    w = csv.writer(f); w.writerow(['maquina','threads','repeticao','segundos','saida_programa'])
    for n in (1,2,4,8,16,32):
        tempos=[]
        for repeticao in range(1,6):
            p=subprocess.run(['/usr/bin/time','-p','./pi_threads',str(n),'1000000000'],capture_output=True,text=True)
            if p.returncode: sys.exit(p.stderr)
            linha=next((x for x in p.stderr.splitlines() if x.startswith('real ')),None)
            if linha is None: sys.exit('Comando /usr/bin/time não retornou real')
            duracao=float(linha.split()[1]); tempos.append(duracao)
            w.writerow([maquina,n,repeticao,duracao,p.stdout.strip()]); f.flush()
            print(maquina,n,repeticao,duracao)
        media=statistics.mean(tempos)
        cv=100*statistics.stdev(tempos)/media
        print(f'{n} threads: média={media:.3f}s CV={cv:.2f}%'+(' (REFAZER: CV > 5%)' if cv>5 else ''))
print('Salvo em',saida)
