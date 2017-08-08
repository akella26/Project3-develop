/*
 * Main function that test DMA for various overlap scenarios and profiles time
 * using breakpoints
 */

#include <stdlib.h>
#include <stdio.h>
#include "dma.h"
#include "MKL25Z4.h"
#include "memory.h"
#include "profiling.h"

static int i = 0;
uint32_t stat,dummy,count1=0,count2=0,diff_count=0;

int8_t result1 = 5, result2 = 5, result3 = 5, result4 = 5;

/*Uncomment the type of profiling you would like to do*/

#define PROFILING 		0
#define DMA_TEST_ONLY 	1

#define MOVE      		0
#define SET       		1

//#define STD_LIB_VERSION
#define NON_DMA_O0_O3
//#define MY_DMA


int main(void)
{
	result1 = test_memmove1();

#if(DMA_TEST_ONLY)
	result2 = test_memmove2();
	result3 = test_memmove3();
	result4 = test_memset();
#endif

	while(1)
	{

	}
	return 0;
}

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

  set = (uint8_t*) malloc(DMA_BUFF_SIZE*(sizeof(uint8_t)));

  if (! set )
  {
    return DMA_TEST_ERROR;
  }
  ptra = &set[0];
  ptrb = &set[DMA_TRANS_BYTE_SIZE];

#if(DMA_TEST_ONLY)
  /* Initialize the set to test values */
  for(i = 0; i < DMA_BUFF_SIZE; i++)
  {
	      set[i] = i;
  }
#endif

#if(PROFILING)

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

#endif



#if(DMA_TEST_ONLY)
memmove_DMA(ptra,ptrb,DMA_TRANS_BYTE_SIZE);
  for (i = 0; i < DMA_TRANS_BYTE_SIZE; i++)
  {
    if (set[i + DMA_TRANS_BYTE_SIZE] != i)
    {
      ret = DMA_TEST_ERROR;
    }
  }
#endif

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

  free(set);
  return ret;

}

int8_t test_memset()
{
  uint32_t i;
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
  for(i = 0; i < DMA_BUFF_SIZE; i++)
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
//  while(1)
//  {
//
//  }
  free(set);
  return ret;
}


