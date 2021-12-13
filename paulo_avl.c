#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

/* Autor.....: Paulo Henrique Diniz de Lima Alencar
 * Matrícula.: 494837
 * Curso.....: CC
 *
 * AVL:
 * 
 * Inserir................... [ok]
 * Contar quantidade de nós.. [ok]
 * Encontrar Altura.......... [ok]
 * Verificar se é AVL........ [ok]
 * Remover................... [Aparentemente ok]
 *
 */


// Nó da AVL
struct no {
  int chave;
  struct no *esq, *dir;
  int bal;
};

typedef struct no No;

// Função: determina a qtd de nós presentes em uma árvore
int qtd_nos(No *pt) {
  if (pt == NULL) return 0;
  return qtd_nos(pt->esq) + qtd_nos(pt->dir) + 1;
}

// Função: outra maneira de determinadar a qtd de nós presentes em um árvore
int qtd_nos_2(No *pt, int *cont) {
  if (pt == NULL) return 0;
  else {
    qtd_nos_2(pt->esq, cont);
    qtd_nos_2(pt->dir, cont);
  }
  *cont += 1;
}

// Função: Utilizando o percurso em Pos-ordem para determinar qtd de nós em uma árvore, mas poderia ser feito em Pre-odem ou In-ordem 
int qtd_nos_3(No *pt, int *cont) {
  if (pt -> esq != NULL)
    qtd_nos_3(pt->esq, cont);
  if (pt -> dir != NULL)
    qtd_nos_3(pt->dir, cont);
  *cont += 1;
}

// Função: dado dois elementos retorna o maior deles
int maior(int a, int b) {
  return (a > b) ? a : b;
}

// Função: determina a altura de uma arvore (recursivamente)
int altura(No *pt) {
  if (pt == NULL) return 0;
  return maior(altura(pt->esq), altura(pt->dir)) + 1;
}

/* 
    Função: determina a altura de uma Árvore cheia h = log2(n+1). Porém AVL é uma ABB completa e balanceada. No entanto, nos testes que fiz 
    a altura calculada recursivamente e a altura calculada através da expressão h = log2(n+1) estavam dando a mesma altura, ou
    foi coencidência ou a utilização da função ceil() permitiu isso. Por via das dúvidas dei preferência em usar a versão recursiva (linha 62)
*/
int altura_(int n) {
  return ceil(log10(n+1) / log10(2));
}

// Função: verifica se árvore é AVL - Para calcular o 'bal' o percurso precisa ser Pos-Ordem
bool verificar_AVL(No *pt) {
  int hd, he, fb;
  bool flag = true;

  if (pt->esq != NULL)
    flag = verificar_AVL(pt->esq);  
  if (pt->dir != NULL)
    flag = verificar_AVL(pt->dir);
  
  if (flag) { 
    hd = altura(pt->dir);
    he = altura(pt->esq);
    fb = hd - he;

    if (fb == -1 || fb == 0 || fb == 1 && fb == pt->bal) flag = true;
    else {                                               
      flag = false;
      printf("No (%d) nao esta balanceado, bal = %d\n",pt->chave, pt->bal);
    }
    printf("fb = %d, bal = %d\n",fb,pt->bal);
  }
  return flag;
}

// Procedimento: responsável por iniciar um Nó
void iniciarNo(No ***pt, int chave) {
  No *novo = (No*) malloc (sizeof(No));
  novo -> esq = novo -> dir = NULL;
  novo -> bal = 0;
  novo -> chave = chave;
  (**pt) = novo;
}

// Procedimento: Caso1 - para Inserção (Rotação simples ou dupla à direita)
void caso1(No **pt, bool *h) {
  No *ptu, *ptv;

  ptu = (*pt)->esq;
  if (ptu->bal == -1) { // Rotação Simples
    (*pt)->esq = ptu->dir;
    ptu->dir = (*pt);
    (*pt) = ptu;
    (*pt)->dir->bal = 0;
  } else {             // Rotação Dupla
    ptv = ptu->dir;
    ptu->dir = ptv->esq;
    ptv->esq = ptu;
    (*pt)->esq = ptv->dir;
    ptv->dir = (*pt);
    
    if (ptv->bal == -1) {
      (*pt)->bal = 1;
      ptu->bal = 0;
    } else if (ptv->bal == 1) {
      (*pt)->bal = 0;
      ptu->bal = -1;
    } else {
      (*pt)->bal = 0;
      ptu->bal = 0;
    }
    (*pt) = ptv;
  }
  (*pt)->bal = 0; *h = false; 
}

// Procedimento: Caso2 - para Inserção (Rotação simples ou dupla à esquerda)
void caso2(No **pt, bool *h) {
  No *ptu, *ptv;

  ptu = (*pt)->dir;
  if (ptu->bal == 1) { // Rotação Simples
    (*pt)->dir = ptu->esq;
    ptu->esq = (*pt);
    (*pt) = ptu;
    (*pt)->esq->bal = 0;
  } else {             // Rotação Dupla
    ptv = ptu->esq;
    ptu->esq = ptv->dir;
    ptv->dir = ptu;
    (*pt)->dir = ptv->esq;
    ptv->esq = (*pt);
    
    if (ptv->bal == 1) {
      (*pt)->bal = -1;
      ptu->bal = 0;
    } else if (ptv->bal == -1){
      (*pt)->bal = 0;
      ptu->bal = 1;
    } else {
      (*pt)->bal = 0;
      ptu->bal = 0;
    }
    (*pt) = ptv;
  }
  (*pt)->bal = 0; *h = false; 
}

// Procedimento: responsável por inserir na AVL
// Entrada: chave para inserir e ponteiro para nó raiz da AVL
void inserirAVL(int x, No **pt, bool *h) {
  if ((*pt) == NULL) {
    iniciarNo(&pt, x);
    *h = true;  
  } else {
    if (x == (*pt)->chave) {
      printf("elem. ja existe\n");
      return;
    }
    if (x < (*pt)->chave) {
      inserirAVL(x, &((*pt)->esq), h);
      if (*h) {
        switch((*pt)->bal) {
          case 1: (*pt)->bal = 0; *h = false;
          break;
          case 0: (*pt)->bal = -1;
          break;
          case -1: caso1(pt, h);
          break;
        }
      }
    } else {
      inserirAVL(x, &((*pt)->dir), h);
      if (*h) {
        switch((*pt)->bal) {
          case -1: (*pt)->bal = 0; *h = false;
          break;
          case 0: (*pt)->bal = 1;
          break;
          case 1: caso2(pt, h);
          break;
        }
      }
    }
  }
}

// Procedimento: responsável por exibir a AVL em Pré-Ordem
void preordem_(No *pt) {
  printf("%d(bal=%d) ",pt->chave,pt->bal); // chave (bal)
  if (pt->esq != NULL)
    preordem_(pt->esq);
  if (pt->dir != NULL)
    preordem_(pt->dir);
}

void preordem(No *pt) {
  if (pt != NULL) preordem_(pt);
  else            printf("arvore esta vazia!\n");
}

// Procedimento: Caso1R - para remover (Rotação simples ou dupla à direita)
void caso1R(No **pt, bool *h) {
  No *ptu, *ptv;

  ptu = (*pt)->esq;
  if (ptu->bal <= 0) { // Rotação simples
    (*pt)->esq = ptu->dir;
    ptu->dir = (*pt);
    (*pt) = ptu;
    if (ptu->bal == -1) {
      ptu->bal = (*pt)->bal = 0;
      *h = true;
    } else {
      ptu->bal = 1;
      (*pt)->dir->bal = -1;
      *h = false;
    }
  } else {            // Rotação dupla
    ptv = ptu->dir;
    ptu->dir = ptv->esq;
    ptv->esq = ptu;
    (*pt)->esq = ptv->dir;
    ptv->dir = (*pt);
    (*pt) = ptv;

    switch(ptv->bal) {
      case -1: ptu->bal = 0; (*pt)->dir->bal = 1;
      break;
      case 0:  ptu->bal = 0; (*pt)->dir->bal = 0;
      break;
      case 1:  ptu->bal = -1; (*pt)->dir->bal = 0;
      break;  
    }
    (*pt)->bal = 0; *h = true;
  }
}

// Procedimento: Caso2R - para remover (Rotação simples ou dupla à esquerda)
void caso2R(No **pt, bool *h) {
  No *ptu, *ptv;

  ptu = (*pt)->dir;
  if (ptu->bal >= 0) {
    (*pt)->dir = ptu->esq;
    ptu->esq = (*pt);
    (*pt) = ptu;
    if (ptu->bal == 1) { 
      ptu->bal = (*pt)->bal = 0;
      *h = true;
    } else {
      ptu->bal = -1;
      (*pt)->esq->bal = 1;
      *h = false;
    }
  } else { 
    ptv = ptu->esq;
    ptu->esq = ptv->dir;
    ptv->dir = ptu;
    (*pt)->dir = ptv->esq;
    ptv->esq = (*pt);
    (*pt) = ptv;

    switch(ptv->bal) {
      case -1: ptu->bal = 0; (*pt)->esq->bal = -1;
      break;
      case 0:  ptu->bal = 0; (*pt)->esq->bal = 0; 
      break;
      case 1:  ptu->bal = 1; (*pt)->esq->bal = 0;
      break;  
    }
    (*pt)->bal = 0; *h = true;
  }
}

// Procedimento: reponsável por realizar o balanceamento
void balancear(No **pt, char R, bool* h) {
  if (*h) {
    if (R == 'D') {
      switch ((*pt)->bal) {
        case 1: (*pt)->bal = 0; break;
        case 0: (*pt)->bal = -1; *h = false; break;
        case -1: caso1R(pt, h); break;
      }
    } else {
      switch((*pt)->bal) {
        case -1: (*pt)->bal = 0; break;
        case 0: (*pt)->bal = 1; *h = false; break;
        case 1: caso2R(pt, h); break;
      }
    }
  }
}

// Procedimento: realiza a troca de dois "elementos"
void troca(No **a, No **b) {
  No* aux;
  int temp;

  aux = (*b);
  (*b) = (*a);
  (*a) = aux;

  aux = (*b)->esq;
  (*b)->esq = (*a)->esq;
  (*a)->esq = aux;
  
  aux = (*b)->dir;
  (*b)->dir = (*a)->dir;
  (*a)->dir = aux;

  temp = (*b)->bal;
  (*b)->bal = (*a)->bal;
  (*a)->bal = temp;
}

// Procedimento: realiza uma remoção na AVL
// Entrada: chave para remover e ponteiro para nó raiz da AVL
void removerAVL(int x, No **pt, bool *h) {
  No *aux;
  if ((*pt) == NULL) {
    printf("Elem. nao existe"); *h = false;
  } else {
    if (x < (*pt)->chave) {
      removerAVL(x, &(*pt)->esq, h);
      balancear(pt,'E',h);
    } else {
      if (x > (*pt)->chave) {
        removerAVL(x, &(*pt)->dir, h);
        balancear(pt,'D',h);
      } else {
        aux = (*pt);
        if ((*pt)->esq == NULL) {
          (*pt) = (*pt)->dir; *h = true;
          free(aux);
        } else {
          if ((*pt)->dir == NULL) {
            (*pt) = (*pt)->esq; *h = true;
            free(aux);
          } else {
            No *s = (*pt)->dir;
            if (s->esq == NULL) {
              s->esq = (*pt)->esq;
              s->bal = (*pt)->bal;
              (*pt) = s;
              *h = true;
              free(aux);
            } else {
              No *PaiS;
              while (s->esq != NULL) {
                PaiS = s;
                s = s -> esq; 
              }
              troca(pt, &PaiS->esq);
              removerAVL(x, &(*pt)->dir, h);
            }
            balancear(pt,'D',h);
          }
        }
      }
    }
  }
}

// Função principal para testes.
int main (void) {
  No *ptraiz = NULL;
  bool h;
  
  /*
 
  // Primeira arvore....................................................................
  printf("Primeira arvore: "); 
  inserirAVL(10, &ptraiz, &h);
  inserirAVL(20, &ptraiz, &h);
  inserirAVL(30, &ptraiz, &h);
  inserirAVL(40, &ptraiz, &h);
  inserirAVL(50, &ptraiz, &h);
  inserirAVL(25, &ptraiz, &h);
  preordem(ptraiz);
  printf("\nqtd nos = %d\n",qtd_nos(ptraiz));
  printf("altura = %d\n",altura(ptraiz));
  printf("flag (AVL) = %d\n",verificar_AVL(ptraiz));
   
  */

  
  /* 
  
  // Segunda arvore.....................................................................
  ptraiz= NULL;
  printf("\nSegunda arvore: "); 
  inserirAVL(30, &ptraiz, &h);
  inserirAVL(5, &ptraiz, &h);
  inserirAVL(32, &ptraiz, &h);
  inserirAVL(45, &ptraiz, &h);
  inserirAVL(40, &ptraiz, &h);
  inserirAVL(35, &ptraiz, &h);
  preordem(ptraiz);
  printf("\nqtd nos = %d\n",qtd_nos(ptraiz));
  printf("altura = %d\n",altura(ptraiz));
  printf("flag (AVL) =  %d\n",verificar_AVL(ptraiz)); 
  
  removerAVL(30,&ptraiz,&h);
  removerAVL(40,&ptraiz,&h);
  removerAVL(45,&ptraiz,&h);
  removerAVL(35,&ptraiz,&h);
  removerAVL(5,&ptraiz,&h);
  removerAVL(32,&ptraiz,&h);
  printf("altura = %d\n",altura(ptraiz));
  printf("\nqtd nos = %d\n",qtd_nos(ptraiz));
  preordem(ptraiz);
  
  */
  
  /*
  // Terceira arvore....................................................................
  ptraiz = NULL;
  printf("\nTerceira arvore: ");
  inserirAVL(10, &ptraiz, &h);
  inserirAVL(4, &ptraiz, &h);
  inserirAVL(89, &ptraiz, &h);
  inserirAVL(12, &ptraiz, &h);
  inserirAVL(3, &ptraiz, &h);
  inserirAVL(7, &ptraiz, &h);
  inserirAVL(20, &ptraiz, &h);
  inserirAVL(30, &ptraiz, &h);
  inserirAVL(40, &ptraiz, &h);
  inserirAVL(17, &ptraiz, &h);
  inserirAVL(9, &ptraiz, &h);
  inserirAVL(1, &ptraiz, &h);
  inserirAVL(15, &ptraiz, &h);
  inserirAVL(8, &ptraiz, &h);
  preordem(ptraiz);
  printf("\nqtd nos = %d\n",qtd_nos(ptraiz));
  printf("altura = %d\n",altura(ptraiz));
  printf("flag (AVL) =  %d\n",verificar_AVL(ptraiz)); 
  
  removerAVL(1,&ptraiz,&h);
  removerAVL(89,&ptraiz,&h);
  removerAVL(9,&ptraiz,&h);
  removerAVL(12,&ptraiz,&h);
  removerAVL(7,&ptraiz,&h);
  removerAVL(17,&ptraiz,&h);
  removerAVL(30,&ptraiz,&h);
  removerAVL(10,&ptraiz,&h);
  removerAVL(4,&ptraiz,&h);
  printf("altura = %d\n",altura(ptraiz));
  printf("\nqtd nos = %d\n",qtd_nos(ptraiz));
  preordem(ptraiz);
  */

  /*
  
  // Quarta arvore......................................................................
  printf("\nQuarta arvore: "); 
  ptraiz = NULL;
  inserirAVL(8, &ptraiz, &h);
  inserirAVL(14, &ptraiz, &h);
  inserirAVL(1, &ptraiz, &h);
  inserirAVL(67, &ptraiz, &h);
  inserirAVL(68, &ptraiz, &h);
  inserirAVL(59, &ptraiz, &h);
  inserirAVL(2, &ptraiz, &h);
  inserirAVL(34, &ptraiz, &h);
  inserirAVL(69, &ptraiz, &h);
  inserirAVL(20, &ptraiz, &h);
  inserirAVL(10, &ptraiz, &h);
  inserirAVL(2, &ptraiz, &h);
  inserirAVL(16, &ptraiz, &h);
  inserirAVL(17, &ptraiz, &h);
  preordem(ptraiz);
  printf("\nqtd nos = %d\n",qtd_nos(ptraiz));
  printf("altura = %d\n",altura(ptraiz));
  printf("flag (AVL) =  %d\n",verificar_AVL(ptraiz)); 
  
  
  // Verificando se eh AVL (Arvore nao balanceada):.....................................
  printf("\n\n");
  printf("Quinta arvore: ");

  No A, B, C, D, E, F;
  No *raiz = NULL;
  raiz = &A;
  
  A.chave = 10;
  B.chave = 12;
  C.chave = 7;
  D.chave = 4;
  E.chave = 8;
  F.chave = 9;
  
  A.bal = -2;
  B.bal = 0; 
  C.bal = 1;
  D.bal = 0;
  E.bal = 1;
  F.bal = 0;

  A.esq = &C;
  A.dir = &B; 
  B.dir = B.esq = NULL;
  C.esq = &D;
  C.dir = &E;
  D.dir = D.esq = NULL;
  E.esq = NULL;
  E.dir = &F;
  F.dir = F.esq = NULL;
  */

  /*                 
   *  AVL nao balanceada:
   *
   *                 10
   *                /   \
   *               7     12
   *              / \
   *             4   8
   *                  \
   *                   9
   *
  */
  
  /* 
  preordem(raiz);
  printf("flag (AVL) =  %d\n",verificar_AVL(raiz)); 
  
  printf("\n\n\n\n\n");
  ptraiz = NULL;
  
  inserirAVL(5, &ptraiz, &h);
  inserirAVL(10, &ptraiz, &h);
  inserirAVL(15, &ptraiz, &h);
  inserirAVL(25, &ptraiz, &h);
  inserirAVL(30, &ptraiz, &h);
  inserirAVL(14, &ptraiz, &h);
  inserirAVL(23, &ptraiz, &h);
  inserirAVL(6, &ptraiz, &h);
  inserirAVL(24, &ptraiz, &h);
  inserirAVL(3, &ptraiz, &h);
  preordem(ptraiz);
  removerAVL(10, &ptraiz, &h);
  removerAVL(15, &ptraiz, &h);
  removerAVL(14, &ptraiz, &h);
  removerAVL(24, &ptraiz, &h);
  removerAVL(5, &ptraiz, &h);
  removerAVL(6, &ptraiz, &h);
  removerAVL(23, &ptraiz, &h);
  removerAVL(30, &ptraiz, &h);
  removerAVL(3, &ptraiz, &h);
  removerAVL(25, &ptraiz, &h);
  printf("\n\n");
  preordem(ptraiz);
  //preordem(ptraiz);
  


  removerAVL(14, &ptraiz, &h);
  
  removerAVL(24, &ptraiz, &h);
  removerAVL(30, &ptraiz, &h);
  removerAVL(5, &ptraiz, &h);
  removerAVL(25, &ptraiz, &h);
  
  removerAVL(3, &ptraiz, &h);
  removerAVL(15, &ptraiz, &h);

  */
  return 0;
}
