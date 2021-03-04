#include <stdlib.h>
#include "btree.h"


btNo *btCriarNo(const unsigned char ordem) {

    btNo *novo = (btNo*) malloc(sizeof(btNo));
    novo->numChaves = 0;
    novo->ehFolha = 1;
    novo->chaves = (int*) malloc(sizeof(*novo->chaves) * ordem);
    novo->filhos = (btNo**) malloc(sizeof(*novo->filhos) * (ordem + 1));
    return novo;

}

bTree btCriar(const unsigned char ordem) {
    bTree b;
    
    b.ordem = ordem;
    b.raiz = btCriarNo(ordem);

    return b;
}

void btDestruir(btNo *no) {
    if (no == NULL) return;
    if (!no->ehFolha) {
        for (int i = 0; i < no->numChaves + 1; i++)
             btDestruir(no->filhos[i]);
    }
    free(no->chaves);
    free(no->filhos);
    free(no);
}

void btDestruir(bTree b) {
    btDestruir(b.raiz);
}

int buscarChave(int n, const int *a, int chave) {
    int lo = -1;
    int hi = n;
    int mid;

    while(lo + 1 < hi) {
        mid = (lo + hi) / 2;
        if (a[mid] == chave) return mid;
        else if (a[mid] < chave) lo = mid;
        else hi = mid;
    }

    return hi;
}

btNo *inserirInterno(btNo *b, int chave, int *mediana, const unsigned char ordem);
void btInserir(bTree b, int chave) {
    
    btNo *b1;
    btNo *b2;
    int mediana;

    b2 = inserirInterno(b.raiz, chave, &mediana, b.ordem);

    if(b2) {
        b1 = btCriarNo(b.ordem);
        for (unsigned char i = 0; i < b.ordem; i++){
            b1->chaves[i] = b.raiz->chaves[i];
            b1->filhos[i] = b.raiz->filhos[i];
        } 
        b1->filhos[b.ordem] = b.raiz->filhos[b.ordem];
        b1->ehFolha = b.raiz->ehFolha;
        b.raiz->ehFolha = 0;
        b1->numChaves = b.raiz->numChaves;
        b.raiz->numChaves = 1;
        b.raiz->chaves[0] = mediana;
        b.raiz->filhos[0] = b1;
        b.raiz->filhos[1] = b2;
    }

}

btNo *inserirInterno(btNo *b, int chave, int *mediana, const unsigned char ordem) {
    int pos, mid;
    btNo *b2;

    pos = buscarChave(b->numChaves, b->chaves, chave);
    if (pos < b->numChaves and b->chaves[pos] == chave) return 0;

    if (b->ehFolha) {
        for (int i = b->numChaves; i > pos; i--)
            b->chaves[i] = b->chaves[i-1];
        b->chaves[pos] = chave;
        b->numChaves++;
    }
    else {
        b2 = inserirInterno(b->filhos[pos], chave, &mid, ordem);
        if (b2) {
            for (int i = b->numChaves; i > pos; i--) {
                b->chaves[i] = b->chaves[i-1];
                b->filhos[i+1] = b->filhos[i];
            }
            b->chaves[pos] = mid;
            b->filhos[pos+1] = b2;
            b->numChaves++;
        }
    }

    if (b->numChaves >= ordem) {
        mid = b->numChaves/2;
        *mediana = b->chaves[mid];
        b2 = btCriarNo(ordem);
        b2->numChaves = b->numChaves - mid - 1;
        b2->ehFolha = b->ehFolha;

        for (int i = mid + 1; i < b->numChaves; i++)
            b2->chaves [i-(mid+1)] = b->chaves[i];
        if (!b->ehFolha)
            for (int i = mid + 1; i < b->numChaves + 1; i++)
                b2->filhos[i-(mid+1)] = b->filhos[i];

        b->numChaves = mid;
        return b2;
    }
    else return 0;
}

int btBuscar(btNo *b, int chave) {

    if (b->numChaves == 0) return 0;

    int pos = buscarChave(b->numChaves, b->chaves, chave);

    if (pos < b->numChaves and b->chaves[pos] == chave) return 1;
    return(!b->ehFolha and btBuscar(b->filhos[pos], chave));

}

int btBuscar(bTree b, int chave) {
    return btBuscar(b.raiz, chave);
}

int btAltura(bTree b) {
    
    int altura = 0;
    btNo *tmp;
    for (tmp = b.raiz; !tmp->ehFolha; tmp = tmp->filhos[0]) altura++;
    return altura + 1;

}

int btContaNos(btNo *raiz) {
    if (raiz->ehFolha) return 1;
    int cont = 0;

    for (int i = 0; i < raiz->numChaves + 1; i++) {
        cont += btContaNos(raiz->filhos[i]);
    }

    return cont + 1;
}

int btContaNos(bTree b) {
    return btContaNos(b.raiz);
}

int btContaChaves(btNo *raiz) {

    if (raiz->ehFolha) return raiz->numChaves;
    int cont = raiz->numChaves;

    for (int i = 0; i < raiz->numChaves + 1; i++) {
        cont += btContaChaves(raiz->filhos[i]);
    }
    return cont;

}

int btContaChaves(bTree t) {
    return btContaChaves(t.raiz);
}
