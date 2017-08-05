/**
 * @file DMA.h
 * @brief This file is to be used to project 1.
 *
 * @author Sowmya Akela
 * @date Aug 2, 2017
 *
 */
#ifndef __DMA_H__
#define __DMA_H__

#include <stdint.h>
#include <stdio.h>
#include "memory.h"

#define DMA_BUFF_SIZE 10000
#define DMA_TRANS_BYTE_SIZE 5000
#define TRANS_SIZE 8

#if(TRANS_SIZE == 32)
#define DMA_NO_BYTES_TRANS_SIZE 0
#elif(TRANS_SIZE == 8)
#define DMA_NO_BYTES_TRANS_SIZE 1
#else
#define DMA_NO_BYTES_TRANS_SIZE 2
#endif

#define DMA_TEST_NO_ERROR 0
#define DMA_TEST_ERROR 1


/**
 * @brief function to run project1 memmove overlapped test
 *
 * This function calls the memmove routine with two sets of data that not
 * over lap. Overlap exists at the start of the source and the end of the
 * destination pointers. This function should print that a move worked correctly
 * for a move from source to destination regardless of overlap.
 *
 * @return void
 */
void memmove_DMA(uint8_t * srcAddr, uint8_t * destAddr, uint32_t length);

/**
 * @brief function to test the non-overlapped memmove operation
 *
 * This function calls the memmove routine with two sets of data that do not
 * over lap in anyway. This function should print that a move worked correctly
 * for a move from source to destination.
 *
 * @return void
 */
int8_t test_memmove1();

/**
 * @brief function to test an overlapped Memmove operation Part 1
 *
 * This function calls the memmove routine with two sets of data that not
 * over lap. Overlap exists at the start of the destination and the end of the
 * source pointers. This function should print that a move worked correctly
 * for a move from source to destination regardless of overlap.
 *
 * @return void
 */
int8_t test_memmove2();

/**
 * @brief function to run project1 memmove overlapped test
 *
 * This function calls the memmove routine with two sets of data that not
 * over lap. Overlap exists at the start of the source and the end of the
 * destination pointers. This function should print that a move worked correctly
 * for a move from source to destination regardless of overlap.
 *
 * @return void
 */
int8_t test_memmove3();

/**
 * @brief function to run project1 memmove overlapped test
 *
 * This function calls the memmove routine with two sets of data that not
 * over lap. Overlap exists at the start of the source and the end of the
 * destination pointers. This function should print that a move worked correctly
 * for a move from source to destination regardless of overlap.
 *
 * @return void
 */

int8_t test_memset();
/**
 * @brief function to run project1 memmove overlapped test
 *
 * This function calls the memmove routine with two sets of data that not
 * over lap. Overlap exists at the start of the source and the end of the
 * destination pointers. This function should print that a move worked correctly
 * for a move from source to destination regardless of overlap.
 *
 * @return void
 */
void DMA_config();

/**
 * @brief function to run project1 memmove overlapped test
 *
 * This function calls the memmove routine with two sets of data that not
 * over lap. Overlap exists at the start of the source and the end of the
 * destination pointers. This function should print that a move worked correctly
 * for a move from source to destination regardless of overlap.
 *
 * @return void
 */
void DMA_maskset_start();
/**
 * @brief function to run project1 memmove overlapped test
 *
 * This function calls the memmove routine with two sets of data that not
 * over lap. Overlap exists at the start of the source and the end of the
 * destination pointers. This function should print that a move worked correctly
 * for a move from source to destination regardless of overlap.
 *
 * @return void
 */

void DMA_SINC_mask_set();
/**
 * @brief function to run project1 memmove overlapped test
 *
 * This function calls the memmove routine with two sets of data that not
 * over lap. Overlap exists at the start of the source and the end of the
 * destination pointers. This function should print that a move worked correctly
 * for a move from source to destination regardless of overlap.
 *
 * @return void
 */
void memset_DMA(uint8_t * src, uint32_t length, uint8_t * value);

#endif /* __DMA_H__ */


