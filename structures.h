#pragma once
#include <stdio.h>

// EJERCICIO 1.1
// GPIO pins, from 0xA0 to 0xA4
typedef enum GPIO_pins {
    CHIP_SELECT = 0xA0,
    INT_1,
    INT_2,
    INT_3,
    INT_4
} GPIO_pins;

// SPI pins, from 0XE0 to 0xE2
typedef enum SPI_pins {
    SERIAL_DATA_OUTPUT = 0xE0,
    SERIAL_DATA_INPUT,
    SERIAL_CLOCK,
} SPI_pins;

// EJERCICIO 1.2
// REGISTERS
/*
Each register has an address,
a variable that dictates if it is writable
and a byte, which can be accessed as a whole or bit by bit
*/
typedef struct DEVICE_DEFINITION {
    uint8_t mem_address;
    uint8_t is_writable;
    union {
        uint8_t byte;
        struct {
            uint8_t gravity_size_def: 2;
            uint8_t extra_interrupt_en: 1;
            uint8_t device_state: 1;
            uint8_t : 4;
        };
    };
} DEVICE_DEFINITION;

typedef struct GRAVITY_L {
    uint8_t mem_address;
    uint8_t is_writable;
    union {
        uint8_t byte;
        struct {
            uint8_t : 2;
            uint8_t measuremente_lsb: 6;
        };
    };
} GRAVITY_L;

typedef struct GRAVITY_H {
    uint8_t mem_address;
    uint8_t is_writable;
    union {
        uint8_t byte;
        struct {
            uint8_t tap_en: 1;
            uint8_t gravity_charge_en: 1;
            uint8_t inactivity_en: 1;
            uint8_t double_tap_en: 1;
            uint8_t : 4;
        };
    };
} GRAVITY_H;

typedef struct INTERRUPT_CONFIGURE {
    uint8_t mem_address;
    uint8_t is_writable;
    union {
        uint8_t byte;
        struct {
            uint8_t tap_en: 1;
            uint8_t gravity_change_en: 1;
            uint8_t inactivity_en: 1;
            uint8_t double_tap_en: 1;
            uint8_t : 4;
        };
    };
} INTERRUPT_CONFIGURE;

// SPI MODES: read or write
typedef enum SPI_MODE {
    READ = 0x00,
    WRITE = 0x01
} SPI_MODE;

// ACCELEROMETER REGISTERS
typedef enum ACCEL_REGISTER {
    DEVICE_DEFINITIONS_R = 0xFF,
    GRAVITY_L_R = 0x10,
    GRAVITY_H_R = 0x11,
    INTERRUPT_CONFIGURE_RW = 0x20
} ACCEL_REGISTER;
/*
Instructions for the linear queue
Each instruction has a read or write SPI mode,
an accelerometer register which will (or will try to) access
and data, in case it wants to write
*/
typedef struct Instruction {
    SPI_MODE mode;
    ACCEL_REGISTER reg;
    uint8_t data;
} Instruction;

// EJERCICIO 1.3
/*
An interruption has a GPIO pin associated to it,
an SPI pin through which information will be transmitted or received
and data, in case the interruption sent some data
*/
typedef struct Interruption {
    GPIO_pins gpio_pin;
    SPI_pins spi_pin;
    uint8_t data;
} Interruption;