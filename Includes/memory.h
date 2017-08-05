/**
 * @file memory.h
 * @brief This file is the header files and function prototypes needed for compiling the memory.c file
 * This file contains standard include libraries and memory manipulation header files
 *
 * @author Sowmya Akella
 * @date June 25, 2017
 *
 */

#ifndef __MEMORY_H__
#define __MEMORY_H__

# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>

#define BUFF_LENGTH 4

typedef enum{
 MEM_STAT_SUCCESS = 0,
 MEM_STAT_NULL_ERROR
}Mem_Status;

typedef enum
{
    DMA_32bits = 0x0U, /*!< 32 bits are transferred for every read/write */
    DMA_8bits,         /*!< 8 bits are transferred for every read/write */
    DMA_16bits,         /*!< 16b its are transferred for every read/write */
} DMA_Tsize;

Mem_Status Mem_Stat;

/* 
This function copies byte by byte from source to destination
NOTE : my_memmove() checks the lengths of source and destination hence no memory is corrupted
Inputs - pointer to source, pointer to destination, length of the bytes to be copied
Returns - uint8_t error code
*/
uint8_t * my_memmove(uint8_t * src, uint8_t * dst, size_t length);

/* 
This function copies byte by byte from source to destination
NOTE : my_memcpy() doesn't check the lengths of source and destination hence some memory locations could get corrupted with unwanted data
Inputs - pointer to source, pointer to destination, length of the bytes to be copied
Returns - pointer to destination byte - uint8_t
*/
uint8_t * my_memcpy(uint8_t * src, uint8_t * dst, size_t length);

/* 
This function sets the source with value till a specified length
Inputs - Pointer to source, length of bytes to be changed, Value to be placed in the source
Returns - Pointer to the source, uint8_t
*/
uint8_t * my_memset(uint8_t * src, size_t length, uint8_t value);

/*
This function zero out all of the memory that a source pointer points to upto a given length 
Inputs - pointer to source, length
Returns - Pointer to source, uint8_t
*/
uint8_t * my_memzero(uint8_t * src, size_t length);

/*
This function reverses the contents of a memory location upto a given length
Input - Source pointer , length of bytes to be reversed
Returns - Pointer to the source, uint8_t
*/
uint8_t * my_reverse(uint8_t * src, size_t length);

/*
Reserves a given length of memory dynamically 
Input - Length of the dynamic memory needed
Returns - A pointer pointing to the allocated memory, uint32_t
*/
uint32_t * reserve_words(size_t length);  /* Changed to unsigned to match with free_words function*/

/*
Frees the dynamically allocated memory 
Input - Pointer to the source
Returns - None
*/
void free_words(uint32_t * src);

/*
Generic swap function to swap two bytes
Inputs - Two byte pointers a and b pointing to two bytes that need to be swapped
Returns - None
*/
void swap(uint8_t*, uint8_t*);

#endif /* __MEMORY_H__ */
