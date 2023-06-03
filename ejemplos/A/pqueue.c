#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "pqueue.h"

struct s_pqueue
{
    unsigned int size;
    struct s_node *front;
};

struct s_node
{
    pqueue_elem elem;
    float average_grade;
    unsigned int approved_courses;
    float priority;
    struct s_node *next;
};

static float calculate_priority(float average_grade,
                                unsigned int approved_courses)
{
    float calc_priority = 0;
    calc_priority = 0.5 * (average_grade/MAX_GRADE) + 
                    0.5 * (approved_courses/TOTAL_COURSES);
    if (calc_priority > 1) {
        calc_priority = 1;
    }
    else if (calc_priority < 0) {
        calc_priority = 0;
    }
    return calc_priority;
}

static struct s_node *create_node(pqueue_elem e,
                                  float average_grade,
                                  unsigned int approved_courses)
{
    struct s_node *new_node = NULL;
    float priority = calculate_priority(average_grade, approved_courses);
    
    new_node = malloc(sizeof(struct s_node));
    assert(new_node != NULL);
    
    new_node->elem = e;
    new_node->average_grade = average_grade;
    new_node->approved_courses = approved_courses;
    new_node->priority = priority;
    new_node->next = NULL;

    return new_node;
}

static struct s_node *destroy_node(struct s_node *node)
{
    assert(node != NULL);
    free(node);
    node = NULL;
    assert(node == NULL);
    return node;
}

static bool invrep(pqueue q)
{
    bool inv_rep = false;
    unsigned int actual_size = 0;
    float priority_before = 1;
    struct s_node * temporal = q->front;

    inv_rep = (q != NULL); // the queue exists
    
    while (temporal != NULL) {
        inv_rep = inv_rep && (priority_before >= temporal->priority);
        temporal = temporal->next;
        actual_size = actual_size + 1;
    } // the queue has a priority order
    
    inv_rep = inv_rep && (q->size == actual_size); 
    // the queue size is correct

    return inv_rep;
}

pqueue pqueue_empty(void)
{
    pqueue q=NULL;
    
    q = malloc(sizeof(struct s_pqueue));
    q->size = 0;
    q->front = NULL;

    assert(invrep(q));
    return q;
}

pqueue pqueue_enqueue(pqueue q,
                      pqueue_elem e,
                      float average_grade,
                      unsigned int approved_courses)
{
    assert(invrep(q));
    struct s_node *new_node = create_node(e, average_grade, approved_courses);

    if (pqueue_is_empty(q)) { // first node
        q->front = new_node;
    }
    else {
        struct s_node * temporal_before = q->front;
        struct s_node * temporal_after = NULL;
        float priority = new_node->priority;

        if (q->front->priority < priority) {
            temporal_after = q->front;
            q->front = new_node;
            new_node->next = temporal_after;
        }
        else {
            while((temporal_before->next) != NULL && (temporal_before->next->priority >= priority)) {
                temporal_before = temporal_before->next;
            } // until priority position of the new node

            temporal_after = temporal_before->next;
            temporal_before->next = new_node;
            new_node->next = temporal_after;
        }
    }

    q->size = q->size +1;
    assert(invrep(q));
    return q;
}

bool pqueue_is_empty(pqueue q)
{
    assert(invrep(q));
    bool empty = (q->size == 0);
    return empty;
}

pqueue_elem pqueue_peek(pqueue q)
{
    assert(invrep(q) && !pqueue_is_empty(q));
    return q->front->elem;
}

float pqueue_peek_average_grade(pqueue q)
{
    assert(invrep(q) && !pqueue_is_empty(q));
    unsigned int average_grade = q->front->average_grade;

    return average_grade;
}

unsigned int pqueue_peek_approved_courses(pqueue q)
{
    assert(invrep(q) && !pqueue_is_empty(q));
    unsigned int approved_courses = q->front->approved_courses;

    return approved_courses;
}

float pqueue_peek_priority(pqueue q)
{
    assert(invrep(q) && !pqueue_is_empty(q));
    float priority = q->front->priority;

    return priority;
}

unsigned int pqueue_size(pqueue q)
{
    assert(invrep(q));
    unsigned int size = q->size;

    return size;
}

pqueue pqueue_copy(pqueue q)
{
    assert(invrep(q));
    struct s_pqueue * q_copy = pqueue_empty();
    q_copy->size = q->size;
    q_copy->front = NULL;
    
    if (!pqueue_is_empty(q)) {
        q_copy->front = create_node(q->front->elem, q->front->average_grade, q->front->approved_courses);
        // first node

        struct s_node * temporal = q->front;
        struct s_node * temporal_copy = q_copy->front;

        for (unsigned int i = 0; i < q_copy->size - 1; ++i) {
            temporal = temporal->next;
            temporal_copy->next = create_node(temporal->elem, temporal->average_grade, temporal->approved_courses);
            temporal_copy = temporal_copy->next;
        } // every other node
    }

    assert(invrep(q));
    assert(invrep(q_copy));
    return q_copy;
}

pqueue pqueue_dequeue(pqueue q)
{
    assert(invrep(q));
    if (!pqueue_is_empty(q)) { // if size = 0, do nothing
        struct s_node * killme = q->front;    
        q->front = q->front->next;
        q->size = q->size - 1;

        killme = destroy_node(killme);
        killme = NULL;
        assert(killme == NULL);
    }
    assert(invrep(q));
    return q;
}

pqueue pqueue_destroy(pqueue q)
{
    assert(invrep(q));
    while(q->size > 0) {
        q = pqueue_dequeue(q);
    }

    free(q);
    q = NULL;
    assert(q == NULL);
    return q;
}