#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//#define DEBUG 1            // comentar esta linha quando for medir tempo
#define TAMANHO_BASE 1000000

void verifica_ordenado(int *vetor, int tamanho) {
    for (int i = 0; i < tamanho - 1; i++) {
        if (vetor[i] > vetor[i + 1]){
            printf("ERROO!! Vetor nao ordenado!!\n");
        }
    }
    printf("SUCESSOO!!! Vetor ordenado!!\n");
}

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
    printf("Execução com %d elementos\n", TAMANHO_BASE);
    clock_t inicio = clock();
    int vetor[TAMANHO_BASE];

    inicializa_vetor(vetor, TAMANHO_BASE);
    printa_vetor(vetor, TAMANHO_BASE, "\nVetor desordenado: ");

    bubble_sort(vetor, TAMANHO_BASE);                     /* sort array */
    clock_t fim = clock();

    printa_vetor(vetor, TAMANHO_BASE, "\nVetor ordenado: ");

    double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
    printf("\nTempo de execução: %.6f segundos\n", tempo);
    verifica_ordenado(vetor, TAMANHO_BASE);

    return 0;
}