#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

// 1.1: GPIO and SPI pins
// 1.2: Registers and Instructions (items for the queue)
// 1.3: Interruptions
#include "structures.h"
// LINEAR QUEUE structures and prototypes
#include "queue_linear.h"
// CIRCULAR QUEUE structures and prototypes
#include "queue_circular.h"

/*
This structure is made in order to contain
and configure all the importante registers
in a single variable.
This is not necessary, but I wanted to include it.
*/
typedef struct System {
    DEVICE_DEFINITION device_definition;
    GRAVITY_L gravity_l;
    GRAVITY_H gravity_h;
    INTERRUPT_CONFIGURE interrupt_configure;
} System;
System admin; // global system variable

void system_config(System* system);
void print_system_config(System system);
int get_random_num(int min, int max);
Instruction instruction_init(ACCEL_REGISTER reg);
Interruption interruption_init(void);

int main (void) {
    GPIO_pins gpio_pin;         // initialize GPIO pins
    SPI_pins spi_pin;           // initialize SPI pins
    system_config(&admin);      // initialize admin system
    print_system_config(admin); // print the system configuration


    /* LINEAR QUEUE CODE SEGMENT
    Linear_Queue linear_q;            // Create a linear queue
    init_linear_q(&linear_q);         // Initialize it as empty
    populate_linear_queue(&linear_q); // Fill it with instructions
    print_linear_queue(&linear_q);    // Print it

    // Instruction processing
    while(!is_empty_linear(&linear_q)) {
        take_linear_action(peek_linear(&linear_q)); // Do something based on the instruction
        dequeue_linear(&linear_q);                  // Take the instruction out of the queue
        print_linear_queue(&linear_q);              // Print the queue
    }
    */

    ///* CIRCULAR QUEUE CODE SEGMENT
    Circular_Queue circular_q;            // Same process as with the linear queue
    init_circular_queue(&circular_q);
    populate_circular_queue(&circular_q);
    print_circular_queue(&circular_q);

    // Interruption processing
    while (!is_empty_circular(&circular_q)) {
        take_circular_action(dequeue_circular(&circular_q)); // Do something based on the interruption
        print_circular_queue(&circular_q);                   // Print the queue
    }
    //*/

    return 0;
}

// Function to simulate the init of the system
void system_config(System* system) {
    system->device_definition.mem_address = 0xFF;
    system->device_definition.is_writable = 0;
    system->gravity_l.mem_address = 0x10;
    system->gravity_l.is_writable = 0;
    system->gravity_h.mem_address = 0x11;
    system->gravity_h.is_writable = 0;
    system->interrupt_configure.mem_address = 0x20;
    system->interrupt_configure.is_writable = 1;
}

// It prints the system configuration
void print_system_config(System system) {
    printf("%d, %d, %d, %d, %d, %d, %d and %d\n",
            system.device_definition.mem_address,
            system.device_definition.is_writable,
            system.gravity_l.mem_address,
            system.gravity_l.is_writable,
            system.gravity_h.mem_address,
            system.gravity_h.is_writable,
            system.interrupt_configure.mem_address,
            system.interrupt_configure.is_writable);
}

// LINEAR QUEUE FUNCTIONS
//Initializes the queue as empty
void init_linear_q(Linear_Queue* q) {
    q->front = -1;
    q->rear = 0;
}

//Checks wether the queue is empty
uint8_t is_empty_linear(Linear_Queue* q) {
    return (q->front == q->rear - 1);
}

// Checks wether the queue is full
uint8_t is_full_linear(Linear_Queue* q) {
    return (q->rear == LINEAR_MAX_SIZE);
}

// To add an instruction to the linear queue
void enqueue_linear(Linear_Queue* q, Instruction instruction) {
    if (is_full_linear(q)) {
        printf("Linear Queue is full\n");
        return;
    }
    q->instructions[q->rear] = instruction; // add it to the rear of the queue
    q->rear++; // next position
}

// To take out an element from the queue
void dequeue_linear(Linear_Queue* q) {
    if (is_empty_linear(q)) {
        printf("Queue is empty\n");
        return;
    }
    q->front++; // just move the front of the queue to the next element
}

// To see what the current instruction is
Instruction peek_linear (Linear_Queue* q) {
    if (!is_empty_linear(q)) {
        return q->instructions[q->front + 1];   
    } else {
        printf("Queue is empty\n");
    }
}

// To populate the queue with instructions
void populate_linear_queue (Linear_Queue* q) {
    if (is_full_linear(q)) {
        printf("Queue is full\n");
        return;
    }
    // This is just a test set
    // Done by hand because you cannot randomize register selection with just the ACCEL_REGISTER enum
    // i.e., I did not want to do another step just to randomize register selection
    srand(time(NULL));
    enqueue_linear(q, instruction_init(DEVICE_DEFINITIONS_R));
    enqueue_linear(q, instruction_init(GRAVITY_L_R));
    enqueue_linear(q, instruction_init(GRAVITY_H_R));
    enqueue_linear(q, instruction_init(INTERRUPT_CONFIGURE_RW));
    enqueue_linear(q, instruction_init(INTERRUPT_CONFIGURE_RW));
}

// Prints the linear queue
void print_linear_queue (Linear_Queue* q) {
    if (is_empty_linear(q)) {
        printf("Linear Queue is empty\n");
        return;
    }
    printf("Current Queue: \n");
    for (int i = q->front + 1; i < q->rear; i++) {

        printf("%d. SPI Mode: %d, Accel register: %x, data: %x\n",
                i,
                q->instructions[i].mode,
                q->instructions[i].reg,
                q->instructions[i].data);
    }
}

// Generates a random int
int get_random_num(int min, int max) {
    int random_num = rand() % (max - min + 1) + min;
    return random_num;
}

// Create an instruction
Instruction instruction_init (ACCEL_REGISTER reg) {
    Instruction new_instruction = {
        get_random_num(0x00, 0x01), // it can be READ OR WRITE
        reg,                        // the register from the argument
        get_random_num(0x00, 0xFF)  // random data from 0 to 255
    };
    return new_instruction;     
}

// This functions take care of dealing with an instruction received as an argument
int8_t take_linear_action(Instruction instruction) {
    switch (instruction.reg) {
        case DEVICE_DEFINITIONS_R:
            if (instruction.mode == WRITE) { // user wants to write, but this register is not writable
                printf("[DEVICE_DEFINITIONS] You cannot write to this register.\n");
                return -1;  // indicates that something went wrong
            }
            return admin.device_definition.byte; // user wants to read, return the requested info
            break;
        
        case GRAVITY_L_R:
            if (instruction.mode == WRITE) { // user wants to write, but this register is not writable
                printf("[GRAVITY_L] You cannot write to this register.\n");
                return -1;
            }
            return admin.gravity_l.byte; // user wants to read, return the requested info
            break;

        case GRAVITY_H_R:
            if (instruction.mode == WRITE) { // user wants to write, but this register is not writable
                printf("[GRAVITY_H] You cannot write to this register.\n");
                return -1;
            }
            return admin.gravity_h.byte; // user wants to read, return the requested info
            break;
        
        case INTERRUPT_CONFIGURE_RW:
            if (instruction.mode == WRITE) { // user wants to write, this register is writable
                admin.interrupt_configure.byte = instruction.data; // write to the register
                printf("[INTERRUPT_CONFIGURE] Writing successful\n");
                return 1; // indicates that everything is ok
            } else if (instruction.mode == READ) { // user wants to read
                return admin.interrupt_configure.byte;  // return requested info
            }
            break;
        
        default: // Just a safeguard
            printf("You should not be here...\n");
            return -2;
    }
}

// CIRCULAR QUEUE FUNCTIONS
// Create an empty circular queue
void init_circular_queue(Circular_Queue* q) {
    q->front = -1;
    q->rear = -1;
}

// Checks wether it is full
uint8_t is_full_circular(Circular_Queue* q) {
    return (q->front == (q->rear + 1) % CIRCULAR_MAX_SIZE);
}

// Checks wether it is empty
uint8_t is_empty_circular(Circular_Queue* q) {
    return (q->front == -1);
}

// Adds an interrupt to the circular queue
void enqueue_circular(Circular_Queue* q, Interruption interruption) {
    if (is_full_circular(q)) {
        printf("Circular Queue is full.\n");
        return;
    }
    if (is_empty_circular(q)) {
        q->front = 0; // If empty, then we can add it at the beginning
    }
    // Add it to the rear
    q->rear = (q->rear + 1) % CIRCULAR_MAX_SIZE; // this wraps around when max size is reached
    q->interruptions[q->rear] = interruption;    // add the interrupt
}

// Take an interrupt out from the queue
Interruption dequeue_circular(Circular_Queue* q) {
    if (!is_empty_circular(q)) {
        Interruption current_interrupt = q->interruptions[q->front];

        if (q->front == q->rear) { // this would mean the queue is now empty
            q->front = q->rear = -1; // declare it as empty
        } else {
            q->front = (q->front + 1) % CIRCULAR_MAX_SIZE; // move the front, make it wrap around if necessary
        }
        return current_interrupt;
    } else {
        printf("Circular Queue is empty.\n");
    }
}

// Prints the circular queue
void print_circular_queue(Circular_Queue* q) {
    if (is_empty_circular(q)) {
        printf("Circular Queue is empty.\n");
        return;
    }
    printf("Circular Queue elements are: \n");
    int i = q->front;
    while (i != q->rear) {
        printf("%d. GPIO pin: %x, SPI pin: %x, data: %x\n",
            i,
            q->interruptions[i].gpio_pin,
            q->interruptions[i].spi_pin,
            q->interruptions[i].data);
        i = (i + 1) % CIRCULAR_MAX_SIZE;
    }
    // The loop only gets to max position - 1, this prints the last element
    printf("%d. GPIO pin: %x, SPI pin: %x, data: %x\n",
        i,
        q->interruptions[i].gpio_pin,
        q->interruptions[i].spi_pin,
        q->interruptions[i].data);
}

// Create a random interruption
Interruption interruption_init(void) {
    Interruption new_interrupt = {
        get_random_num(INT_1, INT_4), // random GPIO pin
        get_random_num(SERIAL_DATA_OUTPUT, SERIAL_DATA_INPUT), // random SPI pin
        get_random_num(0, 255) // random data from 0 to 255
    };
    return new_interrupt;
}

// Fill the circular queue with interrupts
void populate_circular_queue(Circular_Queue* q) {
    if (is_full_circular(q)) {
        printf("Circular Queue is full.\n");
        return;
    }

    srand(time(NULL));
    for (int i = 0; i < CIRCULAR_MAX_SIZE; i++) {
        enqueue_circular(q, interruption_init());
    }
}

/*
This functions takes care of the interruption passed.
It knows which interrupt it is based on the GPIO pin that received it
*/
int8_t take_circular_action(Interruption interruption) {
    switch (interruption.gpio_pin) {
        case INT_1:
            interruption.spi_pin == SERIAL_DATA_INPUT ? (
                printf("TOUCH DETECTED, %x received.\n", interruption.data) // microcontroller is receiving info
            ) : (
                printf("TOUCH DETECTED, sending %x.\n", interruption.data)  // microcontroller is sending info
            );
            return interruption.data; // return the data that you received or sent
            break;

        case INT_2:
            interruption.spi_pin == SERIAL_DATA_INPUT ? (
                printf("GRAVITY CHANGE, %x received.\n", interruption.data)
            ) : (
                printf("GRAVITY CHANGE, sending %x.\n", interruption.data)
            );
            return interruption.data;
            break;

        case INT_3:
            interruption.spi_pin == SERIAL_DATA_INPUT ? (
                printf("INACTIVITY MODE, %x received.\n", interruption.data)
            ) : (
                printf("INACTIVITY MODE, sending %x.\n", interruption.data)
            );
            return interruption.data;
            break;

        case INT_4:
            interruption.spi_pin == SERIAL_DATA_INPUT ? (
                printf("DOUBLE TAP DETECTED, %x received.\n", interruption.data)
            ) : (
                printf("DOUBLE TAP DETECTED, sending %x.\n", interruption.data)
            );
            return interruption.data;
            break;

        default:
            printf("You should not be here...\n");
            return -1;
    }
}