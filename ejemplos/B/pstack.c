#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "pstack.h"

struct s_pstack {
    unsigned int size; 
    struct s_node *front;
};

struct s_node {
    pstack_elem elem;
    float average_grade;//promedio de notas actual
    unsigned int approved_courses;//cantidad de materias aprobadas
    float priority;
    struct s_node *next;
};


static float calculate_priority(float average_grade,
                                unsigned int approved_courses){
    float priority = 0.0;
    priority = 0.3 * (average_grade/MAX_GRADE) + 0.7 * (approved_courses/TOTAL_COURSES);
    return priority;
}



static struct s_node *create_node(pstack_elem e,
                                  float average_grade,
                                  unsigned int approved_courses){
    struct s_node *new_node = NULL;

    float priority = calculate_priority(average_grade, approved_courses);    
    new_node = malloc(sizeof(struct s_node));
    assert(new_node != NULL);
   
   new_node->elem = e;
   new_node->average_grade = average_grade;
   new_node->approved_courses = approved_courses;
   new_node->priority = 0.0;
   new_node->next = NULL;

    return new_node;
}

static struct s_node *destroy_node(struct s_node *node){
    assert(node != NULL);

    free(node);
    node = NULL;

    assert(node == NULL);
    return node;
}


//la creamos para asegurar el invariante de representacion, el orden de pstack.
bool pstack_ord(pstack s){
    bool ord = true;
    struct s_node *p = s->front;
    while (p != NULL && ord && p->next != NULL){
          ord &= p->priority <= (p->next)->priority;
          p = p->next;
    }
    return ord;
}


static bool invrep(pstack s){
    bool inv = s!= NULL ? pstack_ord(s) : true; 
    return inv;
}

pstack pstack_empty(void){
    pstack s=NULL;
    s = malloc(sizeof(struct s_pstack));
    s->front = NULL;
    s->size = 0;
    return s;
}

pstack pstack_push(pstack s,
                   pstack_elem e,
                   float average_grade,
                   unsigned int approved_courses)
{
    assert(invrep(s));
    struct s_node *new_node = create_node(e, average_grade, approved_courses);
    ++s->size;

    struct s_node *p = s->front;
    struct s_node *r = NULL;

    while (p != NULL && new_node->priority <= p->priority)
    {
        r = p;
        p = p->next;
    }
    if (r == NULL){
        new_node->next = p;
        s->front = new_node;
    }else{
        r->next = new_node;
        new_node->next = p;
    }
    return s;
}

bool pstack_is_empty(pstack s){
    assert(invrep(s));
    return s->size == 0;
}

pstack_elem pstack_top(pstack s){
    assert(invrep(s) && !pstack_is_empty(s));
    return s->front->elem;
}

float pstack_top_average_grade(pstack s) {
    assert(invrep(s) && !pstack_is_empty(s));
    return s->front->average_grade;
}

unsigned int pstack_top_approved_courses(pstack s) {
    assert(invrep(s) && !pstack_is_empty(s));
    return s->front->approved_courses;
}

float pstack_top_priority(pstack s) {
    assert(invrep(s) && !pstack_is_empty(s));
    return s->front->priority;
}

unsigned int pstack_size(pstack s){
    assert(invrep(s));
    return s->size;
}

pstack pstack_copy(pstack s){
    assert(invrep(s));
    pstack copy_s = pstack_empty();
    struct s_node *p = s->front;
    
    if(p != NULL){
        copy_s->front = p;
        copy_s->front->next = p->next; 
    }else{
        copy_s->front = p->next;
    }
    return copy_s;
}

pstack pstack_pop(pstack s) {
    assert(invrep(s) && !pstack_is_empty(s));

    struct s_node *killme = s->front;
    s->front = s->front->next;
    destroy_node(killme);
    --s->size;

    assert(invrep(s));
    return s;
}

pstack pstack_destroy(pstack s) {
    assert(invrep(s));
    struct s_node *node = s->front;
    while (node != NULL){
          struct s_node *killme = node;
          node = node->next;
          killme = destroy_node(killme);
    }
    free(s);
    s = NULL;

    assert(s == NULL);
    return s;
}
