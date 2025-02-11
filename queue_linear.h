#pragma once
#define LINEAR_MAX_SIZE 5 // Max size for the linear queue

#include <stdint.h>
#include "structures.h"

typedef struct Liner_Queue {
    Instruction instructions[LINEAR_MAX_SIZE];
    int8_t front;
    uint8_t rear;
} Linear_Queue;

void init_linear_q(Linear_Queue* q);
uint8_t is_empty_linear(Linear_Queue* q);
uint8_t is_full_linear(Linear_Queue* q);
void enqueue_linear(Linear_Queue* q, Instruction instruction);
void dequeue_linear(Linear_Queue*q);
Instruction peek_linear(Linear_Queue* q);
void print_linear_queue(Linear_Queue* q);
void populate_linear_queue(Linear_Queue* q);
int8_t take_linear_action(Instruction instruction);