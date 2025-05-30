#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DEBUG 1            // comentar esta linha quando for medir tempo

void bs(int n, int * vetor)
{
    int c=0, d, troca, trocou =1;

    while (c < (n-1) & trocou )
        {
        trocou = 0;
        for (d = 0 ; d < n - c - 1; d++)
            if (vetor[d] > vetor[d+1])
                {
                troca      = vetor[d];
                vetor[d]   = vetor[d+1];
                vetor[d+1] = troca;
                trocou = 1;
                }
        c++;
        }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "uso: %s <tamanho_do_array>\n", argv[0]);
        return 1;
    }

    int tamanho = atoi(argv[1]);
    if (tamanho <= 0) {
        fprintf(stderr, "erro: tamanho deve ser maior que 1\n");
        return 1;
    }

    int vetor[tamanho];
    int i;

    for (i=0 ; i<tamanho; i++)              /* init array with worst case for sorting */
        vetor[i] = tamanho-i;

    #ifdef DEBUG
    printf("\nVetor: ");
    for (i=0 ; i<tamanho; i++)              /* print unsorted array */
        printf("[%03d] ", vetor[i]);
    #endif

    clock_t inicio = clock();
    bs(tamanho, vetor);                     /* sort array */
    clock_t fim = clock();

    double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
    printf("\nTempo de execução: %.6f segundos\n", tempo);

    #ifdef DEBUG
    printf("\nVetor: ");
    for (i=0 ; i<tamanho; i++)                              /* print sorted array */
        printf("[%03d] ", vetor[i]);
    #endif

    return 0;
}