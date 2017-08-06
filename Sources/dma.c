
/**
 * @file dma.c
 * @brief This file is to be used to project 3.
 *
 * @author Sowmya
 * @date Aug 2, 2017
 *
 */

#include "dma.h"
#include "MKL25Z4.h"
#include <math.h>

extern volatile bool g_Transfer_Done = false;

void DMA_config()
{
	SIM_SCGC6 |= SIM_SCGC6_DMAMUX_MASK;							//DMA Mux Clock Gate enabled
	SIM_SCGC7 |= SIM_SCGC7_DMA_MASK;							//DMA Clock Gate enabled
	DMAMUX0_CHCFG0 = 0x00;										// Disable DMA Mux channel first

	// Clear pending errors and/or the done bit
	if (((DMA_DSR_BCR0 & DMA_DSR_BCR_DONE_MASK) == DMA_DSR_BCR_DONE_MASK)
	  | ((DMA_DSR_BCR0 & DMA_DSR_BCR_BES_MASK) == DMA_DSR_BCR_BES_MASK)
	  | ((DMA_DSR_BCR0 & DMA_DSR_BCR_BED_MASK) == DMA_DSR_BCR_BED_MASK)
	  | ((DMA_DSR_BCR0 & DMA_DSR_BCR_CE_MASK) == DMA_DSR_BCR_CE_MASK))
	  	  DMA_DSR_BCR0 |= DMA_DSR_BCR_DONE_MASK;
}

void DMA_SINC_mask_set()
{
	DMA_DCR0 |= DMA_DCR_SINC_MASK;                          //Increment source
	uint8_t dummy = 0;
}

void DMA_maskset()
{
	DMA_DCR0 |= DMA_DCR_EINT_MASK;							//Enable interrupt after completion
//	DMA_DCR0 |= DMA_DCR_AA_MASK;							//Enable auto-alignment
	//	DMA_DCR0 |= DMA_DCR_ERQ_MASK;						//Since memory to memory peripheral request is ignored
	DMA_DCR0 |= DMA_DCR_EADREQ_MASK;		                //Enable Async. DMA Requests
	//	DMA_DCR0 |= DMA_DCR_CS_MASK;                        //Cycle steal mask disabled so that automatic BCR decrement occurs
	DMA_DCR0 &= ~DMA_DCR_SINC_MASK;
	DMA_DCR0 |= DMA_DCR_DINC_MASK;                          //Increment destination
	DMA_DCR0 |= DMA_DCR_SSIZE(DMA_NO_BYTES_TRANS_SIZE);		//1 byte transfer
	DMA_DCR0 |= DMA_DCR_DSIZE(DMA_NO_BYTES_TRANS_SIZE);     //1 byte transfer
	DMA_DCR0 |=	DMA_DCR_D_REQ_MASK;							//ERQ bit is cleared when the BCR is exhausted.
	DMAMUX0_CHCFG0 |= DMAMUX_CHCFG_ENBL_MASK | DMAMUX_CHCFG_SOURCE(60);
	NVIC_EnableIRQ(DMA0_IRQn);
	__enable_irq();


}

void DMA_start()
{
	DMA_DCR0 |= DMA_DCR_START_MASK;
	while(!(g_Transfer_Done = true))
	{

	}

}
void memmove_DMA(uint8_t * srcAddr, uint8_t * destAddr, uint32_t length)
{
	uint32_t diff1;
	bool ans1, ans2, overlap;

	diff1 = (abs(destAddr-srcAddr));                       		//Check of space available is greater than length
	ans1 = (diff1 >= length);
	ans2 = (srcAddr > destAddr);								//Check if src Address comes after destination address
	overlap = !(ans1 || ans2);

	DMA_config();

	if(overlap == 0)        									// Non problematic overlap condition is true
	{
		DMA_SAR0 = (uint32_t)&srcAddr[0];						//Set source addresss
		DMA_DAR0 = (uint32_t)&destAddr[0];  					//Set destination address
		DMA_DSR_BCR0 |= length;

		DMA_maskset();
		DMA_SINC_mask_set();
		DMA_start();


	}
	else														//Copy overlapping portion first
	{
		DMA_SAR0 = (uint32_t)&destAddr[0];						//Set source address, sinus buffer address
		DMA_DAR0 = (uint32_t)(&destAddr[0]+diff1);  			//Set destination address
		DMA_DSR_BCR0 |= diff1;

		DMA_maskset();
		DMA_SINC_mask_set();
		DMA_start();

		g_Transfer_Done = false;

		DMA_SAR0 = (uint32_t)&srcAddr[0];						//Set source address
		DMA_DAR0 = (uint32_t)&destAddr[0];  					//Set destination address
		DMA_DSR_BCR0 |= (length - diff1);
		DMA_maskset();
		DMA_SINC_mask_set();
		DMA_start();

	}
}

void memset_DMA(uint8_t * destAddr, uint32_t length, uint8_t * value)
{
	g_Transfer_Done = false;
	DMA_config();
	DMA_SAR0 = (uint32_t)value;							//Set source addresss
	DMA_DAR0 = (uint32_t)&destAddr[0];  				//Set destination address
	DMA_DSR_BCR0 = length;
	DMA_maskset();
	DMA_start();

	uint8_t i=0;
}

void DMA0_IRQHandler()
{
	DMA_DSR_BCR0 |= DMA_DSR_BCR_DONE(1);    //Clear DONE status flag so that we can detect next interrupt
	g_Transfer_Done = true;
}


