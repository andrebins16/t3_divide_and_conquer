#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <math.h>

//#define DEBUG 1           // comentar esta linha quando for medir tempo
#define TAMANHO_BASE 1000000

void verifica_ordenado(int *vetor, int tamanho) {
    for (int i = 0; i < tamanho - 1; i++) {
        if (vetor[i] > vetor[i + 1]){
            printf("ERROO!! Vetor nao ordenado!!\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

    }
    printf("SUCESSOO!!! Vetor ordenado!!\n");
}

//printa o vetor se DEBUG estiver ligado
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

//intercala o vetor
int * intercala(int *vetor, int tam) {
	int *vetor_auxiliar;
	int i1, i2, i_aux;

	vetor_auxiliar = (int *)malloc(sizeof(int) * tam);

	i1 = 0;
	i2 = tam / 2;

	for (i_aux = 0; i_aux < tam; i_aux++) {
		if (((vetor[i1] <= vetor[i2]) && (i1 < (tam / 2)))
		    || (i2 == tam))
			vetor_auxiliar[i_aux] = vetor[i1++];
		else
			vetor_auxiliar[i_aux] = vetor[i2++];
	}

	return vetor_auxiliar;
}

int filho_esquerda(int rank) { return 2 * rank + 1; }
int filho_direita(int rank)  { return 2 * rank + 2; }
int pai(int rank) { return (rank - 1) / 2; }

int main(int argc, char **argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Status status;
    int folhas = (size + 1) / 2; // quantidade de folhas
    int delta = (int)ceil((float)TAMANHO_BASE / folhas); // calcula o delta com base na quantidade de folhas e o tamanho do vetor
    int *vetor;
    int tam_vetor;
    double inicio = 0.0, fim = 0.0;

    //raiz só inicializa o vetor
    if(rank == 0){
        printf("Execução com %d elementos\n", TAMANHO_BASE);
        inicio = MPI_Wtime();
        tam_vetor = TAMANHO_BASE;
        vetor = malloc(tam_vetor * sizeof(int));
        inicializa_vetor(vetor, tam_vetor);
        printa_vetor(vetor, tam_vetor, "\nVetor desordenado:");

    }else{ // demais processos esperam receber o vetor. com probe verifica tamanho do vetor, aloca, e so depois recebe mensagem
        MPI_Probe(pai(rank), 0, MPI_COMM_WORLD, &status);
        MPI_Get_count(&status, MPI_INT, &tam_vetor);
        vetor = malloc(tam_vetor * sizeof(int));
        MPI_Recv(vetor, tam_vetor, MPI_INT, pai(rank), 0, MPI_COMM_WORLD, &status);
    }

    // se o grao for pequeno suficiente conquista (ordena) - (é uma folha)
    if (tam_vetor <= delta) {
        bubble_sort(vetor, tam_vetor);

    } else { // se nao, divide o vetor
        int meio = tam_vetor / 2;
        int filho_esq = filho_esquerda(rank);
        int filho_dir = filho_direita(rank);

        //envia o vetor para os filhos
        if (filho_esq < size)
            MPI_Send(&vetor[0], meio, MPI_INT, filho_esq, 0, MPI_COMM_WORLD);
        if (filho_dir < size)
            MPI_Send(&vetor[meio], tam_vetor - meio, MPI_INT, filho_dir, 0, MPI_COMM_WORLD);
        
        // recebe o vetor dos filhos
        if (filho_esq < size)
            MPI_Recv(&vetor[0], meio, MPI_INT, filho_esq, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        if (filho_dir < size)
            MPI_Recv(&vetor[meio], tam_vetor - meio, MPI_INT, filho_dir, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        //intercala
        vetor = intercala(vetor, tam_vetor);
    }

    //raiz. logo acabou, imprime
    if(rank == 0){
        fim = MPI_Wtime();
        printa_vetor(vetor, tam_vetor, "\nVetor ordenado:");
        printf("\nTempo de execução: %.6f segundos\n", fim - inicio);
        verifica_ordenado(vetor, tam_vetor);

    }else{ // demais processos enviam o vetor para o pai
        MPI_Send(vetor, tam_vetor, MPI_INT, pai(rank), 0, MPI_COMM_WORLD);
    }

    free(vetor);
    MPI_Finalize();
    return 0;
}
