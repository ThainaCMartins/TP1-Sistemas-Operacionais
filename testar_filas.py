#!/usr/bin/env python3
"""Executa automaticamente os quatro cenários e registra a saída real dos processos."""
import subprocess,time,signal,pathlib,json
root=pathlib.Path(__file__).parent
result=[]
for na,nb in [(1,1),(2,1),(1,2),(2,2)]:
    subprocess.run([str(root/'limpar_filas')],stdout=subprocess.DEVNULL,stderr=subprocess.DEVNULL)
    with (root/f'teste_{na}A_{nb}B.log').open('w') as f:
        procs=[]
        try:
            for _ in range(nb): procs.append(subprocess.Popen([str(root/'processo_b')],stdout=f,stderr=subprocess.STDOUT))
            time.sleep(.3)
            for _ in range(na): procs.append(subprocess.Popen([str(root/'processo_a')],stdout=f,stderr=subprocess.STDOUT))
            time.sleep(4)
        finally:
            for p in procs: p.send_signal(signal.SIGTERM)
            for p in procs:
                try: p.wait(timeout=2)
                except subprocess.TimeoutExpired: p.kill();p.wait()
    s=(root/f'teste_{na}A_{nb}B.log').read_text()
    result.append({'cenario':f'{na}A/{nb}B','somas_exibidas':s.count(' = '),'respostas_de_outro_cliente':s.count('RESPOSTA DE OUTRO CLIENTE'),'mensagens_B':s.count('respondeu a A')})
    print(result[-1])
subprocess.run([str(root/'limpar_filas')],stdout=subprocess.DEVNULL,stderr=subprocess.DEVNULL)
(root/'resumo_filas.json').write_text(json.dumps(result,indent=2,ensure_ascii=False))
