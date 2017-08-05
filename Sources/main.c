/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdlib.h>
#include <stdio.h>
#include "dma.h"
#include "MKL25Z4.h"
#include "memory.h"
#include "profiling.h"

static int i = 0;
uint32_t stat,dummy,count1=0,count2=0,diff_count=0;
volatile uint32_t count_cycle=0;

int8_t result1 = 5, result2 = 5, result3 = 5, result4 = 5;

/*Uncomment the type of profiling you would like to do*/

#define MOVE      0
#define SET       1
//#define STD_LIB_VERSION
#define NON_DMA_O0_O3
//#define MY_DMA


int main(void)
{

	result1 = test_memmove1();
//	result2 = test_memmove2();
//	result3 = test_memmove3();
//	result4 = test_memset();

	while(1)
	{

	}
	return 0;
}

//For 8 bytes transfers
int8_t test_memmove1()
{
  clock_t start, end;
  double elapsed_time;
  uint32_t i;
  int8_t ret = DMA_TEST_NO_ERROR;
  uint8_t * set;
  uint8_t * ptra;
  uint8_t * ptrb;
  uint8_t value = 0x06;

  // NO OVERLAP
  set = (uint8_t*) malloc(DMA_BUFF_SIZE*(sizeof(uint8_t)));

  if (! set )
  {
    return DMA_TEST_ERROR;
  }
  ptra = &set[0];
  ptrb = &set[DMA_TRANS_BYTE_SIZE];
  /* Initialize the set to test values */
//  for(i = 0; i < DMA_BUFF_SIZE; i++)
//  {
//	      set[i] = i;
//  }

#ifdef STD_LIB_VERSION
  	TPM_init();
  	TPM2_SC |= TPM_SC_CMOD(1);
  	count1 = TPM2_BASE_PTR->CNT;
	#if(MOVE)
    		memmove(ptrb,ptra,DMA_TRANS_BYTE_SIZE);
	#elif(SET)
    		memset(ptrb,7,DMA_TRANS_BYTE_SIZE);
	#endif
    count2 = TPM2_BASE_PTR->CNT;
    TPM_deinit();
#endif

#ifdef NON_DMA_O0_O3
  	TPM_init();
  	TPM2_SC |= TPM_SC_CMOD(1);
  	count1 = TPM2_BASE_PTR->CNT;
	#if(MOVE)
    		my_memmove(ptra,ptrb,DMA_TRANS_BYTE_SIZE);
	#elif(SET)
    		my_memset(ptra,DMA_TRANS_BYTE_SIZE,7);
	#endif
    count2 = TPM2_BASE_PTR->CNT;
 	TPM_deinit();
#endif

#ifdef MY_DMA
  	TPM_init();
  	TPM2_SC |= TPM_SC_CMOD(1);
  	count1 = TPM2_BASE_PTR->CNT;
	#if(MOVE)
  		memmove_DMA(ptra,ptrb,DMA_TRANS_BYTE_SIZE);
	#elif(SET)
  		memset_DMA(ptra,DMA_TRANS_BYTE_SIZE,&value);
	#endif
  	count2 = TPM2_BASE_PTR->CNT;
  	TPM_deinit();

#endif

//  for (i = 0; i < DMA_TRANS_BYTE_SIZE; i++)
//  {
//    if (set[i + DMA_TRANS_BYTE_SIZE] != i)
//    {
//      ret = DMA_TEST_ERROR;
//    }
//  }
//  for (i = 0; i < DMA_TRANS_BYTE_SIZE; i++)
//  {
//  	    if (set[i] != value)
//  	    {
//  	      ret  = DMA_TEST_ERROR;
//  	    }
//  }

  free(set);
  return ret;
}

int8_t test_memmove2() {
  uint32_t i;
  int8_t ret = DMA_TEST_NO_ERROR;
  uint8_t * set;
  uint8_t * ptra;
  uint8_t * ptrb;

  // printf("test_memmove2() -OVERLAP END OF SRC BEGINNING OF DST\n");
  set = (uint8_t*) malloc(DMA_BUFF_SIZE*(sizeof(uint8_t)));

  if (! set )
  {
    return DMA_TEST_ERROR;
  }
  ptra = &set[0];
  ptrb = &set[DMA_TRANS_BYTE_SIZE/2];

  /* Initialize the set to test values */
  for(i = 0; i < DMA_BUFF_SIZE; i++)
  {
	  set[i] = i;
  }

  memmove_DMA(ptra, ptrb, DMA_TRANS_BYTE_SIZE);

  for (i = 0; i < DMA_TRANS_BYTE_SIZE; i++)
  {
    if (set[i + (DMA_TRANS_BYTE_SIZE/2)] != i)
    {
      ret = DMA_TEST_ERROR;
    }
  }
//while(1)
//{
//
//}
  free(set);
  return ret;
}

int8_t test_memmove3() {
  uint32_t i;
  int8_t ret = DMA_TEST_NO_ERROR;
  uint8_t * set;
  uint8_t * ptra;
  uint8_t * ptrb;

  set = (uint8_t*) malloc(DMA_BUFF_SIZE*(sizeof(uint8_t)));

  if (! set )
  {
    return DMA_TEST_ERROR;
  }
  ptra = &set[(DMA_TRANS_BYTE_SIZE/2)];
  ptrb = &set[0];

  /* Initialize the set to test values */
  for( i = 0; i < DMA_BUFF_SIZE; i++)
  {
    set[i] = i;
  }

  memmove_DMA(ptra, ptrb,DMA_TRANS_BYTE_SIZE);

  for (i = 0; i < DMA_TRANS_BYTE_SIZE; i++)
  {
    if (set[i] != (i + (DMA_TRANS_BYTE_SIZE/2)))
    {
      ret = DMA_TEST_ERROR;
    }
  }
//  while(1)
//  {
//
//  }
  free(set);
  return ret;

}

int8_t test_memset()
{
  uint8_t i;
  uint8_t ret = DMA_TEST_NO_ERROR;
  uint8_t * set;
  uint8_t * ptra;
  uint8_t value = 0x06;

  set = (uint8_t*) malloc(DMA_BUFF_SIZE*(sizeof(uint8_t)));

  if (! set )
  {
	  return DMA_TEST_ERROR;
  }
  ptra = &set[0];

  /* Initialize the set to test values */
  for( i = 0; i < DMA_BUFF_SIZE; i++)
   {
     set[i] = i;
   }

  memset_DMA(ptra, DMA_TRANS_BYTE_SIZE, &value);


  /* Validate Set & Zero Functionality */
  for (i = 0; i < DMA_TRANS_BYTE_SIZE; i++)
  {
    if (set[i] != value)
    {
      ret  = DMA_TEST_ERROR;
    }
  }

  free(set);
  return ret;
}

void TPM_init()
{
	SIM_BASE_PTR->SCGC6 |= SIM_SCGC6_TPM2_MASK;        	//TPM2 USED
	SIM_BASE_PTR->SOPT2 |= SIM_SOPT2_TPMSRC(1);        	//Clock SRC 11- MCGIRCLK
	TPM2_BASE_PTR->SC = TPM_SC_PS(6);	 				// Prescalar of 1 to get smallest possible resolution
	TPM2_SC |= TPM_SC_TOIE_MASK;						// Enable the counter & interrupt
	TPM2_BASE_PTR->MOD = 0xFFFF;						//Load for high Counter value
	NVIC_EnableIRQ(TPM2_IRQn);							//Enable TPM2 interrupt in NVIC
}

void TPM_deinit()
{
	TPM2_SC |= TPM_SC_CMOD(0);
	TPM2_SC &= ~TPM_SC_TOIE_MASK;						// Enable the counter & interrupt
}


void TPM2_IRQHandler()
{
	TPM2_BASE_PTR->SC |= TPM_SC_TOF_MASK;              //Clearing TOF bit
	count_cycle++;
}
