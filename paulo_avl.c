#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define AVLS 1000         // MIL AVL's
#define NOS 10000         // 10 MIL NÓS     
#define TAM_ARRAY 10000   // TAMANHO DO ARRAY PARA AS CHAVES
#define QTD_REMOCOES 1000 // QTD DE REMOCOES, MIL REMOCOES


/* Autor.....: Paulo Henrique Diniz de Lima Alencar
 * Matrícula.: 494837
 * Curso.....: CC
 *
 * AVL:
 * 
 * Implementações................:
 *
 * Inserir................... [ok]
 * Contar quantidade de nós.. [ok]
 * Encontrar Altura.......... [ok]
 * Verificar se é AVL........ [ok]
 * Remover................... [ok]
 *
 * Testes.......................:
 *
 * Criar 1.000 ALV's ............................................ [ok]
 * Inserir 10.000 nós em cada AVL com chaves entre 0 e 100.000 .. [ok]
 * Verificar se a AVL possui 10.000 nós ......................... [ok]
 * Verificar se árvore é AVL .................................... [ok]
 * Remover 1.000 nós ............................................ [ok]
 * Após as remoções, verificar se árvore continua sendo AVL ..... [ok]
 *
 * Apresentação.................:
 *
 * Vídeo de 7 minutos ->
 *  # dificuldades encontradas e como foram resolvidas ..................................................... [ok]
 *  # apresentação dos códigos de cada algoritmo (atenção especial no inserir e remover) ................... [ok]
 *  # demonstrar o funcionamento do programa em um caso pequeno (inserção de 100 nós e remoção de 10 nós) .. [ok] 
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

// Função: outra maneira de determinadar a qtd de nós presentes em uma árvore
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

// Função: dado dois elementos retorna o maior deles (utilizando operador ternario)
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

    if (fb == -1 || fb == 0 || fb == 1) flag = true;
    else {                                               
      flag = false;
      printf("No (%d) nao esta regulado, fb = %d\n",pt->chave, fb);
    }
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
  printf("chave = (%d)(bal=%d)\n",pt->chave,pt->bal); // chave (bal)
  if (pt->esq != NULL)
    preordem_(pt->esq);
  if (pt->dir != NULL)
    preordem_(pt->dir);
}

void preordem(No *pt) {
  printf("ALV (Pré-ordem):\n");
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
  if (ptu->bal >= 0) { // Rotacao Simples
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
  } else { // Rotacao Dupla
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
    printf("Elem. nao existe!\n"); *h = false;
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


// Procedimento: resposnsável por desalocar memória de toda AVL - Basta percorrer a AVL em Pós-Ordem
void liberar_memoriaAVL_(No* pt) {
  if (pt->esq != NULL)
    liberar_memoriaAVL_(pt->esq);
  if (pt->dir != NULL)
    liberar_memoriaAVL_(pt->dir);

  free(pt);
}

// Procedimento: apos desalocar memoria de todos os nos de cada AVL, deixo o ponteiro raiz receber NULL, para indicar que a AVL esta vazia
void liberar_memoriaAVL(No** pt) {
  liberar_memoriaAVL_(*pt);
  *pt = NULL;
}


// Procedimento: vai inserir 10 mil elementos distintos em um array com valores no range [0-100.000]
void numeros_randomicos_10K (int* array, int tam_array, int semente) {
  for (int i = 0; i < tam_array; i++) {
    if (i <= 2499) {
      array[i] = semente * i + 2; 
    } else if (i >= 2500 && i <= 5000) {
      array[i] = semente * i + 4;  
    } else if (i <= 7500) {
      array[i] = semente * i + 6; 
    } else {
      array[i] = semente * i + 8;
    }
  }
}

// Procedimento: vai inserir Mil elementos em um array com valores no range [0-10.000]
void numeros_randomicos_1K (int* array, int tam_array, int semente) { 
  for (int i = 0; i < tam_array; i++) {
    if (i <= 249) {
      array[i] = semente * i + 2; 
    } else if (i >= 250 && i <= 500) {
      array[i] = semente * i + 4;  
    } else if (i <= 750) {
      array[i] = semente * i + 6; 
    } else {
      array[i] = semente * i + 8;
    }
  }
}

// Função: responsável por fazer uma busca sequencial em um vetor (retorna true se o elemento existe, false em caso contrário)
bool busca_sequencial(int* array, int tam_array, int chave) {
  for (int i = 0; i < tam_array; i++) {
    if (array[i] == chave) return true;
  }
  return false;
}

// Procedimento: responsável por gerar 100 números distintos em um array
void numeros_randomicos_100 (int* array, int tam_array) {
  int chave;
  srand(time(NULL));
  for (int i = 0; i < tam_array; i++) {
    chave = 1 + rand() % 10000;
    while (busca_sequencial(array, tam_array, chave)) {
      chave = 1 + rand() % 10000;
    }
    array[i] = chave;
  }
}

// Procedimento: responsável por gerar 10 numeros distintos em um array
void numeros_randomicos_10 (int* array, int tam_array) {
  int chave;
  srand(time(NULL));
  for (int i = 0; i < tam_array; i++) {
    chave = rand() % 100;
    while (busca_sequencial(array, tam_array, chave)) {
      chave = rand() % 100;
    }
    array[i] = chave;
  }

}

// Realizando os testes propostos pelo professor.
void TESTES () {
  
  int *array = (int*) malloc (sizeof(int) * TAM_ARRAY);
  int *indices = (int*) malloc (sizeof(int) * QTD_REMOCOES);
  
  memset(array, 0, sizeof(int) * TAM_ARRAY);
  memset(indices, 0, sizeof(int) * QTD_REMOCOES);
  
  No* ptraiz = NULL;
  bool h;
     
  srand(time(NULL));

  // TESTES ENUMERADOS.
  // 1. Deve-se criar 1.000 AVL's 
  for (int i = 0; i < AVLS; i++) {
    numeros_randomicos_10K (array, TAM_ARRAY, 1 + rand() % 9);
    
    // 2. Deve-se inserir 10.000 nós onde cada nó está entre 0 e 100.000 (verificar se AVL possui 10.000 nós)
    printf("\nINSERCAO (AVL - %d): \n",i+1);
    for (int j = 0; j < NOS; j++) {
      inserirAVL(array[j], &ptraiz, &h);     
    }
    
    // 2. Verificando se AVL possui 10.000 nós
    if (qtd_nos(ptraiz) == NOS) printf("#. ALV possui 10 MIL nós! n = %d\n",qtd_nos(ptraiz));
    else                        printf("#. AVL não possui 10 MIL nós! n = %d\n",qtd_nos(ptraiz));
    
    // 3. Verificar se cada árvore é AVL
    if (verificar_AVL(ptraiz)) printf("#. Árvore é AVL!\n");
    else                       printf("#. Árvore não é AVL!\n");
    
    printf("\n\nREMOCAO (AVL - %d):\n",i+1);

    // 4. Remover 1.000 nós 
    numeros_randomicos_1K (indices, QTD_REMOCOES, 1 + rand() % 9);
    for (int j = 0; j < QTD_REMOCOES; j++) {
      removerAVL(array[indices[j]], &ptraiz, &h);
    }

    // 4. Verificar se a AVL possui 9.000 nós após a remoção
    if (qtd_nos(ptraiz) == NOS - QTD_REMOCOES) printf("#. AVL possui 9 MIL nós! n = %d\n",qtd_nos(ptraiz));
    else                                       printf("#. AVL não possui 9 MIL nós! n = %d\n",qtd_nos(ptraiz));

    // 5. Após a remoção verificar se AVL continua sendo AVL
    if (verificar_AVL(ptraiz)) printf("#. Após remoções: Árvore é AVL!\n");
    else                       printf("#. Após remoções: Árvore não é AVL!\n");

    liberar_memoriaAVL(&ptraiz);
  }
  free(array);
  free(indices);
}

// Função principal para testes.
int main (void) {
  
  // TESTES();
  
   /* 
  No* ptraiz = NULL;
  bool h;
  
  // Criando AVL com 100 nós....................
  int* array = (int*) malloc (sizeof(int) * 100);
  memset(array, 0, sizeof(int) * 100);

  numeros_randomicos_100 (array, 100);
  
  for (int i = 0; i < 100; i++) {
    inserirAVL(array[i], &ptraiz, &h);
  }

  preordem(ptraiz);
  
  printf("\nqtd nos = %d\n",qtd_nos(ptraiz));
  printf("altura = %d\n",altura(ptraiz));
  printf("flag (AVL) = %d\n",verificar_AVL(ptraiz));
  
  // Removendo 10 nós da AVL...................
  
  int* indices_removidos = (int*) malloc (sizeof(int) * 100);
  memset(indices_removidos, -1, sizeof(int) * 100);
  numeros_randomicos_10 (indices_removidos, 10);

  for (int i = 0; i < 10; i++) {
    removerAVL(array[indices_removidos[i]], &ptraiz, &h);
  }

  preordem(ptraiz);
  
  printf("\nqtd nos após remoções = %d\n",qtd_nos(ptraiz));
  printf("altura = %d\n",altura(ptraiz));
  printf("flag (AVL) = %d\n",verificar_AVL(ptraiz));
  
   */
  
  ///*
  
  // Árvores para testes: 

  No *ptraiz = NULL;
  bool h;
 
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
  
  printf("\n");
   
  removerAVL(50, &ptraiz, &h);
  removerAVL(40, &ptraiz, &h);
  preordem(ptraiz); 
  
  printf("\nqtd nos = %d\n",qtd_nos(ptraiz));
  printf("altura = %d\n",altura(ptraiz));
  printf("flag (AVL) = %d\n",verificar_AVL(ptraiz));
  
  liberar_memoriaAVL(&ptraiz);
 
  //*/

  
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
