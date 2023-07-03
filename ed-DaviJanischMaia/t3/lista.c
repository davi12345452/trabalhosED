#include "lista.h"
#include <stdlib.h>

typedef struct _node {
  void *data;
  struct _node *prev;
  struct _node *next;
} Node;

struct _lista {
  Node *head;
  Node *tail;
  int ndados;
  int pos_percurso;
};

Lista lista_cria(void)
{
  Lista self = malloc(sizeof(*self));
  if (self == NULL) return NULL;
  self->head = NULL;
  self->tail = NULL;
  self->ndados = 0;
  self->pos_percurso = 0;
  return self;
}

void lista_destroi(Lista self)
{
  if (self != NULL) {
    Node *current = self->head;
    while (current != NULL) {
      Node *next = current->next;
      free(current);
      current = next;
    }
    free(self);
  }
}

int lista_nelem(Lista self)
{
  if (self == NULL) return 0;
  return self->ndados;
}

void *lista_elem_pos(Lista self, int pos)
{
  if (self == NULL) return NULL;
  if (pos < 0) pos = self->ndados + pos;
  if (pos < 0 || pos >= self->ndados) return NULL;

  Node *current;
  if (pos < self->ndados / 2) {
    current = self->head;
    for (int i = 0; i < pos; i++) {
      current = current->next;
    }
  } else {
    current = self->tail;
    for (int i = self->ndados - 1; i > pos; i--) {
      current = current->prev;
    }
  }

  return current->data;
}

void lista_insere_pos(Lista self, void *dado, int pos)
{
  if (self == NULL) return;
  if (pos < 0) pos = self->ndados + pos + 1;
  if (pos < 0 || pos > self->ndados) return;

  Node *newNode = malloc(sizeof(Node));
  if (newNode == NULL) return;

  newNode->data = dado;

  if (pos == 0) {
    newNode->prev = NULL;
    newNode->next = self->head;
    if (self->head != NULL) {
      self->head->prev = newNode;
    }
    self->head = newNode;
    if (self->tail == NULL) {
      self->tail = newNode;
    }
  } else if (pos == self->ndados) {
    newNode->prev = self->tail;
    newNode->next = NULL;
    self->tail->next = newNode;
    self->tail = newNode;
  } else {
    Node *current;
    if (pos < self->ndados / 2) {
      current = self->head;
      for (int i = 0; i < pos; i++) {
        current = current->next;
      }
    } else {
      current = self->tail;
      for (int i = self->ndados - 1; i > pos; i--) {
        current = current->prev;
      }
    }
    newNode->prev = current->prev;
    newNode->next = current;
    current->prev->next = newNode;
    current->prev = newNode;
  }

  self->ndados++;
}

void lista_insere_inicio(Lista self, void *dado)
{
  lista_insere_pos(self, dado, 0);
}

void lista_insere_fim(Lista self, void *dado)
{
  lista_insere_pos(self, dado, -1);
}

void *lista_remove_pos(Lista self, int pos)
{
  if (self == NULL) return NULL;
  if (pos < 0) pos = self->ndados + pos;
  if (pos < 0 || pos >= self->ndados) return NULL;

  Node *current;
  if (pos == 0) {
    current = self->head;
    self->head = current->next;
    if (self->head != NULL) {
      self->head->prev = NULL;
    }
    if (self->tail == current) {
      self->tail = NULL;
    }
  } else if (pos == self->ndados - 1) {
    current = self->tail;
    self->tail = current->prev;
    self->tail->next = NULL;
  } else {
    if (pos < self->ndados / 2) {
      current = self->head;
      for (int i = 0; i < pos; i++) {
        current = current->next;
      }
    } else {
      current = self->tail;
      for (int i = self->ndados - 1; i > pos; i--) {
        current = current->prev;
      }
    }
    current->prev->next = current->next;
    current->next->prev = current->prev;
  }

  void *dado = current->data;
  free(current);
  self->ndados--;

  return dado;
}

void *lista_remove_inicio(Lista self)
{
  return lista_remove_pos(self, 0);
}

void *lista_remove_fim(Lista self)
{
  return lista_remove_pos(self, -1);
}

void *lista_proximo(Lista self)
{
  if (self == NULL) return NULL;
  if (self->pos_percurso >= self->ndados) return NULL;

  Node *current = self->head;
  for (int i = 0; i < self->pos_percurso; i++) {
    current = current->next;
  }

  self->pos_percurso++;
  return current->data;
}

void *lista_percorre(Lista self)
{
  if (self == NULL) return NULL;
  self->pos_percurso = 0;
  return lista_proximo(self);
}
