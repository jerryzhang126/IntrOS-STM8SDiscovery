/******************************************************************************

    @file    IntrOS: oscore.h
    @author  Rajmund Szymanski
    @date    21.04.2017
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

#ifndef __INTROSCORE_H
#define __INTROSCORE_H

#include <osbase.h>

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------------------- */

// task context

typedef struct __ctx ctx_t;

#if   defined(__CSMC__)

struct __ctx
{
	void   * sp;
	fun_t  * pc;

};

#elif defined(__SDCC)

struct __ctx
{
	fun_t  * pc;
	void   * sp;

};

#endif

#define _CTX_INIT() { 0, 0 }

/* -------------------------------------------------------------------------- */

__STATIC_INLINE
void port_ctx_init( ctx_t *ctx, stk_t *sp, fun_t *pc )
{
	ctx->sp = sp - 1;
	ctx->pc = pc;
}

/* -------------------------------------------------------------------------- */

#if   defined(__CSMC__)

#define _get_SP()    (void *)_asm("ldw x, sp")
#define _set_SP(sp)  (void)  _asm("ldw sp, x", (void *)(sp))

#define _get_CC()    (char)  _asm("push cc""\n""pop a")
#define _set_CC(cc)  (void)  _asm("push a""\n""pop cc", (char)(cc))

#elif defined(__SDCC)

char _get_CC( void );
void _set_CC( char cc);

#endif

/* -------------------------------------------------------------------------- */

#define  port_get_lock()     _get_CC()
#define  port_put_lock(lck)  _set_CC(lck)

#define  port_set_lock()      disableInterrupts()
#define  port_clr_lock()      enableInterrupts()

#define  port_sys_lock()      do { char __LOCK = port_get_lock(); port_set_lock()
#define  port_sys_unlock()         port_put_lock(__LOCK); } while(0)

#define  port_isr_lock()      do { port_set_lock()
#define  port_isr_unlock()         port_clr_lock(); } while(0)

/* -------------------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif//__INTROSCORE_H
