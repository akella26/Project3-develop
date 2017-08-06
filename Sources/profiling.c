/**
 * @file profiling.c
 * @brief This file is to keep the timer
 *
 * @author Sowmya
 * @date Aug 2, 2017
 *
 */
#include "MKL25Z4.h"

extern volatile uint32_t count_cycle=0;

void TPM_init()
{
	SIM_BASE_PTR->SCGC6 |= SIM_SCGC6_TPM2_MASK;        	//TPM2 USED
	SIM_BASE_PTR->SOPT2 |= SIM_SOPT2_TPMSRC(1);        	//Clock SRC 11- MCGIRCLK
	TPM2_BASE_PTR->SC = TPM_SC_PS(6);	 				// Prescalar of 6
	TPM2_SC |= TPM_SC_TOIE_MASK;						// Enable the counter & interrupt
	TPM2_BASE_PTR->MOD = 0xFFFF;						//Load for high Counter value
	NVIC_EnableIRQ(TPM2_IRQn);							//Enable TPM2 interrupt in NVIC
}

void TPM_deinit()
{
	TPM2_SC |= TPM_SC_CMOD(0);
	TPM2_SC &= ~TPM_SC_TOIE_MASK;						// Disable the counter & interrupt
}

void TPM2_IRQHandler()
{
	TPM2_BASE_PTR->SC |= TPM_SC_TOF_MASK;              //Clearing TOF bit
	count_cycle++;
}
