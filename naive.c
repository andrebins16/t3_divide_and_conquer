#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define DEBUG 1            // comentar esta linha quando for medir tempo
#define TAMANHO_BASE 1000

void pai_printa(char *mensagem, int rank) {
    if(rank == 0)
        printf("%s\n", mensagem);
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

void intercala(int *vetor, int tamanho) {
	int *vetor_auxiliar;
	int i1, i2, i_aux;

	vetor_auxiliar = (int *)malloc(sizeof(int) * tamanho);

	i1 = 0;
	i2 = tamanho / 2;

	for (i_aux = 0; i_aux < tamanho; i_aux++) {
		if (((vetor[i1] <= vetor[i2]) && (i1 < (tamanho / 2)))|| (i2 == tamanho))
			vetor_auxiliar[i_aux] = vetor[i1++];
		else
			vetor_auxiliar[i_aux] = vetor[i2++];
	}

    for (int i = 0; i < tamanho; i++)
        vetor[i] = vetor_auxiliar[i];

    free(vetor_auxiliar);
}

int filho_esquerda(int rank) { return 2 * rank + 1; }
int filho_direita(int rank)  { return 2 * rank + 2; }

int main(int argc, char **argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc != 2) {
        if (rank == 0)
            printf("uso: %s <multiplicador_de_trabalho>\n", argv[0]);
        MPI_Finalize();
        return 1;
    }

    //multiplicador do tamanho do problema para escalabilidade fraca
    int multiplicador_trabalho = atoi(argv[1]);
    if (multiplicador_trabalho <= 0) {
        if (rank == 0)
            printf("valor invalido para multiplicador de trabalho.\n");
        MPI_Finalize();
        return 1;
    }

    int tamanho_total = TAMANHO_BASE * multiplicador_trabalho;

    int folhas = (size + 1) / 2;
    int delta = tamanho_total / folhas;

    if(rank == 0){
        printf("Folhas: %d\n", folhas);
        printf("Delta: %d\n", delta);
    }

    MPI_Finalize();
    return 0;
}
