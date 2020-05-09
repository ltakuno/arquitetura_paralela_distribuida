def algoritmo_cubico(v):
    max_ate_agora = 0
    n = len(v)
    for l in range(1, n+1):
        for u in range(l, n+1):
            soma = 0
            for i in range(l, u + 1):
                soma = soma + v[i-1]
            max_ate_agora = max(max_ate_agora, soma)
    return max_ate_agora



maximo = algoritmo_cubico([10,5,-17,20,50,-1,3,-30,10])
# subsequencia contigua de soma máxima = [20,50,-1,3]
# https://pt.wikipedia.org/wiki/Sublista_cont%C3%ADgua_de_soma_m%C3%A1xima

print(maximo)
