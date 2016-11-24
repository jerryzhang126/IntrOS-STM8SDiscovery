/******************************************************************************

    @file    IntrOS: osport.c
    @author  Rajmund Szymanski
    @date    24.11.2016
    @brief   IntrOS port file for STM8 uC.

 ******************************************************************************

    IntrOS - Copyright (C) 2013 Rajmund Szymanski.

    This file is part of IntrOS distribution.

    IntrOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published
    by the Free Software Foundation; either version 3 of the License,
    or (at your option) any later version.

    IntrOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

 ******************************************************************************/

#include <oskernel.h>

/* -------------------------------------------------------------------------- */

void port_sys_init( void )
{
/******************************************************************************
 Put here configuration of system timer

 Example for TIM1 (STM32F4):

	NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 0xFF);
	NVIC_EnableIRQ  (TIM1_UP_TIM10_IRQn);
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;

	TIM1->PSC  = CPU_FREQUENCY / 10000 - 1;
	TIM1->ARR  = 10000 /  OS_FREQUENCY - 1;
	TIM1->EGR  = TIM_EGR_UG;
	TIM1->CR1  = TIM_CR1_CEN;
	TIM1->DIER = TIM_DIER_UIE;

*******************************************************************************/

	CLK->CKDIVR = 0;
	CLK->SWCR  |= CLK_SWCR_SWEN; // Enable switching the master clock to the source defined in the CLK_SWR register
	CLK->SWR    = 0xB4;          // HSE selected as master clock source

	TIM4->PSCR  = 6;             // Prescaler:      16.000.000 /  64 = 250.000
	TIM4->ARR   = 249;           // Auto-reload value: 250.000 / 250 =   1.000
	TIM4->IER  |= TIM4_IER_UIE;  // Enable interrupt
	TIM4->CR1  |= TIM4_CR1_CEN;  // Enable timer

	enableInterrupts();
}

/* -------------------------------------------------------------------------- */

/******************************************************************************
 Put here the procedure of interrupt handler of system timer

 Example for TIM1 (STM32F4):

void TIM1_UP_TIM10_IRQHandler( void )
{
	TIM1->SR = ~TIM_SR_UIF; // clear timer's status register

	Counter++;
}

*******************************************************************************/

INTERRUPT_HANDLER(TIM4_UPD_OVF_IRQHandler, 23)
{
	TIM4->SR1 = ~TIM4_SR1_UIF; // clear timer's status register

	Counter++;
}

/* -------------------------------------------------------------------------- */
