#include "uf.h"

grafo* criarGrafo(int V, int E) {
    grafo* g = (grafo*) malloc( sizeof(grafo) );
    g->V = V;
    g->E = E;
    g->VetorDeArestas = (aresta*) malloc(g->E * sizeof(aresta) );
    return g;
}

void destruirGrafo(grafo *g) {
    free(g->VetorDeArestas);
    free(g);
}

subset *Make_Subset(int tamanho) {
    subset *subconjuntos = (subset*) malloc(tamanho * sizeof(subset));
    for (int i=0; i<tamanho; i++) {
        Make_Set(subconjuntos, i);
    }
    return subconjuntos;
}

void Destroy_Subset(subset *s) {
    free(s);
}

void Make_Set(subset subconjuntos[], int i) {
    subconjuntos[i].pai = i;
    subconjuntos[i].rank = 0;
}

// Funcao que procura o representante (pai) do elemento i com compressao de caminho.
int Find_Set(subset subconjuntos[], int i) {

    if (i != subconjuntos[i].pai) {
        subconjuntos[i].pai = Find_Set(subconjuntos, subconjuntos[i].pai);
    }

    return subconjuntos[i].pai;
}

// Funcao que junta os conjuntos de x e y com uniao ponderada.
void Union(subset subconjuntos[], int x, int y) {

    x = Find_Set(subconjuntos, x);
    y = Find_Set(subconjuntos, y);

    if (subconjuntos[x].rank > subconjuntos[y].rank)
        subconjuntos[y].pai = x;
    else {
        subconjuntos[x].pai = y;
        if (subconjuntos[x].rank == subconjuntos[y].rank)
            subconjuntos[y].rank++;
    }

}

// Funcao utilizada para verificar se o grafo tem ou nao ciclo
bool TemCiclo( grafo* g ) {

    bool tem_ciclo = false;
    subset *s = Make_Subset(g->V);

    for (int i = 0; i < g->E; i++) {
        int rx = Find_Set(s, g->VetorDeArestas[i].origem);
        int ry = Find_Set(s, g->VetorDeArestas[i].destino);
        if (rx != ry)
        	Union(s, rx, ry);
        else {
            tem_ciclo = true;
        }
    }

    Destroy_Subset(s);
    return tem_ciclo;
}

int QuantidadeComponentesConexas( grafo* g ) {

    subset *s = Make_Subset(g->V);
    for(int i=0; i<g->E; i++) {
        int rx = Find_Set(s, g->VetorDeArestas[i].origem);
        int ry = Find_Set(s, g->VetorDeArestas[i].destino);
        if (rx!=ry)
        	Union(s, rx, ry);
    }
	int nc = 0;
    for (int i = 0; i < g->V; i++) 
        if (i == s[i].pai) nc++;
    
    Destroy_Subset(s);
    return nc;
}

int MaiorComponenteConexa( grafo* g ) {
   
    subset *s = Make_Subset(g->V);
    for(int i=0; i<g->E; i++) {
        int rx = Find_Set(s, g->VetorDeArestas[i].origem);
        int ry = Find_Set(s, g->VetorDeArestas[i].destino);
        if (rx!=ry)
        	Union(s, rx, ry);
    }

    int maior = 0;
    int aux[g->V];

    for (int i = 0; i < g->V; i++) aux[i] = 0;
    for (int i = 0; i < g->V; i++) {
        int pai = Find_Set(s, i);
        aux[pai]++;
    }
    for (int i = 0; i < g->V; i++)
        if (aux[i] > maior) maior = aux[i];

    Destroy_Subset(s);
    return maior;
}

int partition (aresta *arr, int low, int high) {

    int pivot = arr[high].peso;
    int i = low - 1;

    for (int j = low; j <= high - 1; j++) {

        if (arr[j].peso < pivot) {
            i++;
            aresta t = arr[i];
            arr[i] = arr[j];
            arr[j] = t;
        }

    }
    aresta t = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = t;
    return i + 1;

}

void quickSort (aresta *arr, int low, int high) {

    if (low < high) {
        int pi = partition(arr, low, high);

        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }

}

aresta *order(aresta *v, int tamanho) {
    quickSort(v, 0, tamanho - 1);
    return v;
}

int SomaPesoArestasDaArvoreGeradoraMinima( grafo* g ) {
    
    int soma = 0;
    subset *s = Make_Subset(g->V);

    g->VetorDeArestas = order(g->VetorDeArestas, g->E);

    for (int i = 0; i < g->E; i++) {
        int rx = Find_Set(s, g->VetorDeArestas[i].origem);
        int ry = Find_Set(s, g->VetorDeArestas[i].destino);
        if (rx != ry) {
            soma += g->VetorDeArestas[i].peso;
            Union(s, rx, ry);
        }
    }

    Destroy_Subset(s);
    return soma;
}