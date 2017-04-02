/******************************************************************************

    @file    IntrOS: osport.c
    @author  Rajmund Szymanski
    @date    21.03.2017
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
*******************************************************************************/

	CLK->CKDIVR = 0;
	CLK->ECKR  |= CLK_ECKR_HSEEN; while ((CLK->ECKR & CLK_ECKR_HSERDY) == 0);
	CLK->SWCR  |= CLK_SWCR_SWEN;
	CLK->SWR    = 0xB4; /* HSE */ while ((CLK->SWCR & CLK_SWCR_SWBSY)  == 1);

	#define  CNT_(X)   ((X)>>0?(X)>>1?(X)>>2?(X)>>3?(X)>>4?(X)>>5?(X)>>6?(X)>>7?(X)>>8?(X)>>9?1/0:9:8:7:6:5:4:3:2:1:0)
	#define  PSC_ CNT_ ((CPU_FREQUENCY/OS_FREQUENCY-1)>>16)
	#define  ARR_     (((CPU_FREQUENCY/OS_FREQUENCY)>>PSC_)-1)

	TIM3->PSCR  = PSC_;
	TIM3->ARRH  = ARR_ >> 8;
	TIM3->ARRL  = ARR_;
	TIM3->IER  |= TIM3_IER_UIE;
	TIM3->CR1  |= TIM3_CR1_CEN;

	enableInterrupts();

/******************************************************************************
 End of configuration
*******************************************************************************/
}

/* -------------------------------------------------------------------------- */

#if OS_TIMER == 0

/******************************************************************************
 Put here the procedure of interrupt handler of system timer
*******************************************************************************/

INTERRUPT_HANDLER(TIM3_UPD_OVF_BRK_IRQHandler, 15)
{
	TIM3->SR1= (uint8_t) ~TIM3_SR1_UIF;

	System.cnt++;
}

/******************************************************************************
 End of the procedure of interrupt handler
*******************************************************************************/

#endif//OS_TIMER

/* -------------------------------------------------------------------------- */

/******************************************************************************
 Put here necessary lock / unlock procedures
*******************************************************************************/

#if defined(__SDCC)

char port_get_lock(void) __naked
{
	__asm

	push   cc
	pop    a
	ret

	__endasm;
}

void port_put_lock(char state) __naked
{
	(void) state;
	
	__asm

	push   a
	pop    cc
	ret

	__endasm;
}

#endif // __SDCC

/* -------------------------------------------------------------------------- */