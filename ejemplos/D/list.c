#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "list.h"

struct _node_t {
  unsigned int key;
  list_value value;
  struct _node_t *next;
};

typedef struct _node_t * node_t;

struct _list_t {
  unsigned int size;
  node_t elems;
};

static bool invrep(list_t list) {
  assert(list != NULL);
  if (list->elems == NULL && list->size == 0) {
    return true;
  }
  node_t check = list->elems;
  while (check->next != NULL){
    if (check->key >= check->next->key) {
      return false;
    }
    check = check->next;
  }
  return true;
}


static struct _node_t * create_node(unsigned int key, list_value value) {
  node_t new_node = malloc(sizeof(struct _node_t));
  new_node->key = key;
  new_node->value = value;
  new_node->next = NULL;
  assert(new_node != NULL);
  return new_node;
}

static struct _node_t * destroy_node(struct _node_t *node) {
  free(node);
  return NULL;
}

list_t list_empty(void) {
    list_t list = NULL;
  list = malloc(sizeof(struct _list_t));
  
  if (list == NULL) {
    exit(EXIT_FAILURE);
  }
  
  list->elems = NULL;
  list->size = 0;
  
  assert(invrep(list));
  return list;
}

list_t list_add(list_t list, unsigned int key, list_value value) {
  assert(invrep(list));
  node_t node = create_node(key, value);
  assert(node != NULL);

  if (list->size == 0) {
    list->elems = node;
  } else if (list_exists(list, key)) {
    node_t replace = list->elems;
    while (replace->key != key) {
      replace = replace->next;
    }
    replace->value = value;  //Actualizamos el valor
    destroy_node(node);      //destruimos el nodo creado
  } else {
    node_t replace = list->elems;
    node_t prev = NULL;
    while (replace != NULL && replace->key < key) {
      prev = replace;
      replace = replace->next;
    }
    if (prev == NULL) {
      // Insertar al principio de la lista
      node->next = list->elems;
      list->elems = node;
    } else {
      // Insertar en otro lugar de la lista
      prev->next = node;
      node->next = replace;
    }
  }
  list->size++;
  assert(invrep(list) && value == list_search(list, key));
  return list;
}

unsigned int list_length(list_t list) {
  assert(invrep(list));
  return list->size;
}

bool list_is_empty(list_t list) {
  assert(invrep(list));
  return (list->size == 0);
}

bool list_exists(list_t list, unsigned int key) {
  assert(invrep(list));
  node_t cursor = list->elems;
  while (cursor != NULL && cursor->key < key) {
    cursor = cursor->next;
  }
  return cursor != NULL && cursor->key == key;
}


char list_search(list_t list, unsigned int key) {
  assert(invrep(list));
  assert(list_exists(list, key));
  node_t search = list->elems;
  while (search->key != key){
    search = search->next;
  }
  list_value res = search->value;
  //assert ((search->value != NULL) == list_exists(list, key));
  //no supe como hacer el assert con el resultado, pero deberia poder chequearse correctamente
  return res;
}

list_t list_remove(list_t list, unsigned int key) {
  assert(invrep(list));
  if (!list_exists(list, key)) {
    return list;
  }

  node_t search = list->elems;
  node_t prev = NULL;

  while (search != NULL && search->key != key) {
    prev = search;
    search = search->next;
  }

  if (prev == NULL) {
    list->elems = search->next;
  } else {
    prev->next = search->next;
  }

  destroy_node(search);
  list->size--;

  assert(invrep(list) && !list_exists(list, key));
  return list;
} 

list_t list_remove_all(list_t list) {
  node_t kill = list->elems;
  node_t next = NULL;

  while (kill != NULL) {
    next = kill->next;
    destroy_node(kill);
    kill = next;
  }

  list->elems = NULL;
  list->size = 0;

  assert(invrep(list) && list_length(list) == 0);
  return list;
}

list_value* list_to_array(list_t list) {
  assert(invrep(list));
  list_value *array = calloc(list->size, sizeof(list_value));
  node_t dump = list->elems;
  unsigned int length = list_length(list);
  for (unsigned int i = 0; (dump != NULL) && (i<=length); i++){
    array[i] = dump->value;
    dump = dump->next;
  }
  assert(invrep(list) && !list_is_empty(list));
  return array;
}

list_t list_destroy(list_t list) {
  list_remove_all(list);
  free(list);
  return NULL;
}
