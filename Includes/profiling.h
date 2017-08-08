/**
 * @file profiling.h
 * @brief This file is to be used to project 1.
 *
 * @author Sowmya Akela
 * @date Aug 2, 2017
 *
 */
#ifndef __PROFILING_H__
#define __PROFILING_H__

#include <stdint.h>
#include <stdio.h>
#include "dma.h"

/*
initializes TPM counter with required values to be used in profiling
Inputs - None
Returns - None
*/
void TPM_init();

/*
Deinitializes the TPM counter that's used to be stop profiling
Inputs - None
Returns - None
*/
void TPM_deinit();
#endif /* _PROFILING_H__ */
