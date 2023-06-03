#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include "set.h"

struct s_node {
    set_elem elem;
    struct s_node *next;
};

typedef struct s_node * node_t;

struct s_set {
    unsigned int size;
    node_t first;
};
/*
static bool
invrep(set s) {
    bool result = true;
    if (s->first == NULL){
        result = true;
    }
    else 
    {
        node_t iterador = NULL;
        node_t posterior = NULL;
        iterador = s->first;
        while (iterador != NULL && result){
            posterior = iterador->next;
            if (iterador->elem > posterior->elem){
                result = false;
            }
            iterador = iterador->next;
        }
    }
    return result;
}
*/
static struct s_node *
create_node(set_elem e) {
    node_t new_node = NULL;
    new_node = malloc(sizeof(struct s_node *));
    new_node->elem = e;
    new_node->next = NULL;
    assert(new_node != NULL);
return new_node;
}

static struct s_node *
destroy_node(struct s_node *node) {
   free(node);
   node = NULL;
   assert(node == NULL);
return node;
}

/* CONSTRUCTORS */

set set_empty(void) {
    set new_set = NULL;
    new_set = malloc(sizeof(set));
    new_set->size = 0;
    new_set->first = NULL;
    assert(new_set != NULL);
return new_set;
}

set set_add(set s, set_elem e) {
    //assert(invrep(s));
    node_t new_node = NULL;
    new_node = create_node(e);
    s->size++;
    if (s->first == NULL)
    {
        s->first = new_node;
    }
    else if (set_member(e,s) != e){
        node_t iterador = NULL;
        node_t anterior = NULL;
        iterador = s->first;
        anterior = iterador;
        while (iterador != NULL && iterador->elem < e){
            anterior = iterador;
            iterador = iterador->next;
        }
        anterior->next = new_node;
        new_node->next = iterador;
    }
    else 
    {
        s->size--;
    }
    //assert(invrep(s));
return s;
}

/* OPERATIONS   */
unsigned int set_cardinal(set s) {
    //assert(invrep(s));
return (s->size);
}

bool set_is_empty(set s) {
    //assert(invrep(s));
return (s->size == 0);
}

bool set_member(set_elem e, set s) {
    //assert(invrep(s));
    node_t node=s->first;
    while (node!=NULL && node->elem < e) {
        node = node->next;
    }
    return node!=NULL && node->elem==e;
}

set set_elim(set s, set_elem e) {
    //assert(invrep(s));
    node_t iterador = NULL;
    iterador = s->first;
    if(set_member(e,s)){
        while(iterador != NULL && iterador->elem != e ){
            iterador = iterador->next;
        }
        iterador = destroy_node(iterador);
        s->size--;
    }
    //assert(invrep(s));
return s;    
}

set_elem set_get(set s) {
    //assert(invrep(s) && !set_is_empty(s));
    return s->first->elem;
}

set_elem*
set_to_array(set s) {
    //assert(invrep(s));
    node_t iterador = NULL;
    set_elem *array = calloc(s->size, sizeof(set_elem));
    unsigned int length = set_cardinal(s); 
    iterador = s->first;
    for (unsigned int i = 0; iterador != NULL && i <= length; i++){
         array[i] = iterador->elem;
         iterador = iterador->next;
    }
return array;    
}

set set_destroy(set s) {
    //assert(invrep(s));
    node_t iterador = NULL;
    node_t anterior = NULL;
    iterador = s->first;
    anterior = iterador;
    while (iterador->next != NULL)
    {
        anterior = iterador;
        anterior = destroy_node(anterior);
        iterador = iterador->next;
    }
    iterador = destroy_node(iterador);
    free(s);
    s->first = NULL;
    s = NULL;
return s;
}

