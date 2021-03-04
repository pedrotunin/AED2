#include "trie.h"
#include <stdlib.h>

#include <stdio.h>
#include <iostream>
using namespace std;


no *criarNo(void) {

    no *novo = (no*) malloc(sizeof(no));
    novo->tipo = 'I';
    int n = 0;
    while (n < TAMANHO_ALFABETO) {
        novo->filho[n] = NULL;
        n++;
    }

    return novo;
}

void apagarArvore(no *raiz) {

    if (raiz == NULL) return;
    else {
        int n = 0;
        while (n < TAMANHO_ALFABETO) {
            apagarArvore(raiz->filho[n]);
            n++;
        }
        free(raiz);
    }
}

void adicionarPalavra(char *palavra, no *raiz) {

    if (buscaPalavra(palavra, raiz)) return;

    no *tmp = raiz;
    int tamanho = strlen(palavra);

    for (int indice = 0; indice < tamanho; indice++) {

        int indiceChar = CHAR_TO_INDEX(palavra[indice]);

        if (tmp->filho[indiceChar] == NULL) { 
            tmp->filho[indiceChar] = criarNo();
        }
        tmp = tmp->filho[indiceChar];

    }
    tmp->tipo = 'P';
}

int buscaPalavra(char *palavra, no *raiz) {

    if (raiz == NULL) return 0;

    no *tmp = raiz;
    int tamanho = strlen(palavra);
    int index;
    for (index = 0; index < tamanho; index++) {
        int charIndex = CHAR_TO_INDEX(palavra[index]);
        if (tmp->filho[charIndex] == NULL) return 0;
        tmp = tmp->filho[charIndex];
    }
    if (tmp->tipo == 'I') return 0;
    else return 1;
}

int numeroDeNos(no *r) {

    int total = 0;

    if (r != NULL) total++;

    for(int i = 0; i < TAMANHO_ALFABETO; i++) {
        if (r->filho[i] != NULL) {
            total += numeroDeNos(r->filho[i]);
        }
    }

    return total;
}

int numeroDePalavras(no *r) {
    
    int total = 0;

    if (r->tipo == 'P') total++;

    for(int i = 0; i < TAMANHO_ALFABETO; i++) {
        if (r->filho[i] != NULL) {
            total += numeroDePalavras(r->filho[i]);
        }
    }

    return total;
}

int altura(no *r) {

    int max = 0;
    int total = 0;

    for(int i = 0; i < TAMANHO_ALFABETO; i++) {
        if (r->filho[i]) {
            total = 1 + altura(r->filho[i]);
        }
        if (total > max) max = total;
    }

    return max;
}

int qtdFilhos(no *raiz) {
    int total = 0;
    for (int i = 0; i < TAMANHO_ALFABETO; i++) {
        if (raiz->filho[i]) total++;
    }
    return total;
}

no *remover(char *palavra, no *raiz, int nivel) {
    if (raiz == NULL) return NULL;

    if (nivel == strlen(palavra)) {
        if (raiz->tipo == 'P') raiz->tipo = 'I';
        if (qtdFilhos(raiz) == 0) {
            free(raiz);
            raiz = NULL;
        }
        return raiz;
    }

    int indice = CHAR_TO_INDEX(palavra[nivel]);
    raiz->filho[indice] = remover(palavra, raiz->filho[indice], nivel + 1);
    
    if (qtdFilhos(raiz) == 0 && raiz->tipo == 'I') {
        free(raiz);
        raiz = NULL;
    }
    return raiz;
}

void removerPalavra(char *palavra, no *raiz) {

    if(raiz == NULL || buscaPalavra(palavra, raiz) == 0) return;
    else remover(palavra, raiz, 0);

}
