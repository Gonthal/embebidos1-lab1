#pragma once
#include <stdint.h>
#include "structures.h"
#define CIRCULAR_MAX_SIZE 10

typedef struct Circular_Queue {
    Interruption interruptions[CIRCULAR_MAX_SIZE];
    int8_t front;
    int8_t rear;
} Circular_Queue;

void init_circular_queue(Circular_Queue* q);
uint8_t is_full_circular(Circular_Queue* q);
uint8_t is_empty_circular(Circular_Queue* q);
void enqueue_circular(Circular_Queue* q, Interruption interruption);
Interruption dequeue_circular(Circular_Queue* q);
void print_circular_queue(Circular_Queue* q);
void populate_circular_queue(Circular_Queue* q);
int8_t take_circular_action(Interruption interruption);