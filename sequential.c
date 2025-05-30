#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DEBUG 1            // comentar esta linha quando for medir tempo
#define TAMANHO_BASE 1000

void printa_vetor(int *vetor, int tamanho, char* mensagem) {
#ifdef DEBUG
    printf("%s: ", mensagem);
    for (int i = 0; i < tamanho; i++)
        printf("[%03d] ", vetor[i]);
    printf("\n");
#endif
}

void inicializa_vetor(int *vetor, int tamanho) {
    for (int i=0 ; i<tamanho; i++)              /* init array with worst case for sorting */
        vetor[i] = tamanho-i;
}

void bubble_sort(int * vetor, int tamanho)
{
    int c=0, d, troca, trocou =1;

    while (c < (tamanho-1) & trocou )
        {
        trocou = 0;
        for (d = 0 ; d < tamanho - c - 1; d++)
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
        printf("uso: %s <multiplicador_de_trabalho>\n", argv[0]);
        return 1;
    }

    //multiplicador do tamanho do problema para escalabilidade fraca
    int multiplicador_trabalho = atoi(argv[1]);
    if (multiplicador_trabalho <= 0) {
        printf("valor invalido para multiplicador de trabalho.\n");
        return 1;
    }

    int tamanho_total = TAMANHO_BASE * multiplicador_trabalho;

    int vetor[tamanho_total];

    inicializa_vetor(vetor, tamanho_total);

    printa_vetor(vetor, tamanho_total, "\nVetor desordenado: ");

    clock_t inicio = clock();
    bubble_sort(vetor, tamanho_total);                     /* sort array */
    clock_t fim = clock();

    printa_vetor(vetor, tamanho_total, "\nVetor ordenado: ");

    double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
    printf("\nTempo de execução: %.6f segundos\n", tempo);

    return 0;
}