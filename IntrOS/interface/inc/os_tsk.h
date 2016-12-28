/******************************************************************************

    @file    IntrOS: os_tsk.h
    @author  Rajmund Szymanski
    @date    28.12.2016
    @brief   This file contains definitions for IntrOS.

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

#ifndef __INTROS_TSK_H
#define __INTROS_TSK_H

#include <oskernel.h>

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************************************************************************
 *                                                                                                                    *
 * Name              : task (thread)                                                                                  *
 *                                                                                                                    *
 **********************************************************************************************************************/

struct __tsk
{
	unsigned id;    // inherited from timer
	tsk_id   prev;  // inherited from timer
	tsk_id   next;  // inherited from timer
	unsigned event; // wakeup event

	fun_id   state; // inherited from timer
	unsigned start; // inherited from timer
	unsigned delay; // inherited from timer
	unsigned period;// inherited from timer

	void    *sp;    // stack pointer
	void    *top;   // top of stack
#ifdef __cplusplus
	~__tsk( void ) { assert(id == ID_STOPPED); }
#endif
};

/**********************************************************************************************************************
 *                                                                                                                    *
 * Name              : _TSK_INIT                                                                                      *
 *                                                                                                                    *
 * Description       : create and initilize a task object                                                             *
 *                                                                                                                    *
 * Parameters                                                                                                         *
 *   state           : task state (initial task function) doesn't have to be noreturn-type                            *
 *                     it will be executed into an infinite system-implemented loop                                   *
 *   top             : top of task's private stack storage                                                            *
 *                                                                                                                    *
 * Return            : task object                                                                                    *
 *                                                                                                                    *
 * Note              : for internal use                                                                               *
 *                                                                                                                    *
 **********************************************************************************************************************/

#define               _TSK_INIT( state, top ) { 0, 0, 0, 0, state, 0, 0, 0, 0, top }

/**********************************************************************************************************************
 *                                                                                                                    *
 * Name              : _TSK_STACK                                                                                     *
 *                                                                                                                    *
 * Description       : create task's private stack storage                                                            *
 *                                                                                                                    *
 * Parameters                                                                                                         *
 *   size            : size of task's private stack storage (in bytes)                                                *
 *                                                                                                                    *
 * Return            : top of task's private stack storage                                                            *
 *                                                                                                                    *
 * Note              : for internal use                                                                               *
 *                                                                                                                    *
 **********************************************************************************************************************/

#define               _TSK_STACK( size ) (stk_t[ASIZE(size)]){ 0 } + ASIZE(size)

/**********************************************************************************************************************
 *                                                                                                                    *
 * Name              : OS_WRK                                                                                         *
 *                                                                                                                    *
 * Description       : define and initilize complete work area for task object                                        *
 *                                                                                                                    *
 * Parameters                                                                                                         *
 *   tsk             : name of a pointer to task object                                                               *
 *   state           : task state (initial task function) doesn't have to be noreturn-type                            *
 *                     it will be executed into an infinite system-implemented loop                                   *
 *   size            : size of task private stack (in bytes)                                                          *
 *                                                                                                                    *
 **********************************************************************************************************************/

#define             OS_WRK( tsk, state, size )                                        \
                       stk_t tsk##__stk[ASIZE( size )];                                \
                       tsk_t tsk##__tsk = _TSK_INIT( state, tsk##__stk+ASIZE( size ) ); \
                       tsk_id tsk = & tsk##__tsk

/**********************************************************************************************************************
 *                                                                                                                    *
 * Name              : OS_TSK                                                                                         *
 *                                                                                                                    *
 * Description       : define and initilize complete work area for task obj. with stack size defined by OS_STACK_SIZE *
 *                                                                                                                    *
 * Parameters                                                                                                         *
 *   tsk             : name of a pointer to task object                                                               *
 *   state           : task state (initial task function) doesn't have to be noreturn-type                            *
 *                     it will be executed into an infinite system-implemented loop                                   *
 *                                                                                                                    *
 **********************************************************************************************************************/

#define             OS_TSK( tsk, state ) \
                    OS_WRK( tsk, state, OS_STACK_SIZE )

/**********************************************************************************************************************
 *                                                                                                                    *
 * Name              : OS_DEF                                                                                         *
 *                                                                                                                    *
 * Description       : define and initilize complete work area for task obj. with stack size defined by OS_STACK_SIZE *
 *                     task state (function body) must be defined immediately below                                   *
 *                                                                                                                    *
 * Parameters                                                                                                         *
 *   tsk             : name of a pointer to task object                                                               *
 *                                                                                                                    *
 **********************************************************************************************************************/

#define             OS_DEF( tsk )              \
                       void tsk##__fun( void ); \
                    OS_TSK( tsk, tsk##__fun );   \
                       void tsk##__fun( void )

/**********************************************************************************************************************
 *                                                                                                                    *
 * Name              : static_WRK                                                                                     *
 *                                                                                                                    *
 * Description       : define and initilize static work area for task object                                          *
 *                                                                                                                    *
 * Parameters                                                                                                         *
 *   tsk             : name of a pointer to task object                                                               *
 *   state           : task state (initial task function) doesn't have to be noreturn-type                            *
 *                     it will be executed into an infinite system-implemented loop                                   *
 *   size            : size of task private stack (in bytes)                                                          *
 *                                                                                                                    *
 **********************************************************************************************************************/

#define         static_WRK( tsk, state, size )                                        \
                static stk_t tsk##__stk[ASIZE( size )];                                \
                static tsk_t tsk##__tsk = _TSK_INIT( state, tsk##__stk+ASIZE( size ) ); \
                static tsk_id tsk = & tsk##__tsk

/**********************************************************************************************************************
 *                                                                                                                    *
 * Name              : static_TSK                                                                                     *
 *                                                                                                                    *
 * Description       : define and initilize static work area for task object with stack size defined by OS_STACK_SIZE *
 *                                                                                                                    *
 * Parameters                                                                                                         *
 *   tsk             : name of a pointer to task object                                                               *
 *   state           : task state (initial task function) doesn't have to be noreturn-type                            *
 *                     it will be executed into an infinite system-implemented loop                                   *
 *                                                                                                                    *
 **********************************************************************************************************************/

#define         static_TSK( tsk, state ) \
                static_WRK( tsk, state, OS_STACK_SIZE )

/**********************************************************************************************************************
 *                                                                                                                    *
 * Name              : static_DEF                                                                                     *
 *                                                                                                                    *
 * Description       : define and initilize static work area for task object with stack size defined by OS_STACK_SIZE *
 *                     task state (function body) must be defined immediately below                                   *
 *                                                                                                                    *
 * Parameters                                                                                                         *
 *   tsk             : name of a pointer to task object                                                               *
 *                                                                                                                    *
 **********************************************************************************************************************/

#define         static_DEF( tsk )              \
                static void tsk##__fun( void ); \
                static_TSK( tsk, tsk##__fun );   \
                static void tsk##__fun( void )

/**********************************************************************************************************************
 *                                                                                                                    *
 * Name              : WRK_INIT                                                                                       *
 *                                                                                                                    *
 * Description       : create and initilize complete work area for task object                                        *
 *                                                                                                                    *
 * Parameters                                                                                                         *
 *   state           : task state (initial task function) doesn't have to be noreturn-type                            *
 *                     it will be executed into an infinite system-implemented loop                                   *
 *   size            : size of task private stack (in bytes)                                                          *
 *                                                                                                                    *
 * Return            : task object                                                                                    *
 *                                                                                                                    *
 * Note              : use only in 'C' code                                                                           *
 *                                                                                                                    *
 **********************************************************************************************************************/

#define                WRK_INIT( state, size ) \
                      _TSK_INIT( state, _TSK_STACK( size ) )

/**********************************************************************************************************************
 *                                                                                                                    *
 * Name              : WRK_CREATE                                                                                     *
 *                                                                                                                    *
 * Description       : create and initilize complete work area for task object                                        *
 *                                                                                                                    *
 * Parameters                                                                                                         *
 *   state           : task state (initial task function) doesn't have to be noreturn-type                            *
 *                     it will be executed into an infinite system-implemented loop                                   *
 *   size            : size of task private stack (in bytes)                                                          *
 *                                                                                                                    *
 * Return            : pointer to task object                                                                         *
 *                                                                                                                    *
 * Note              : use only in 'C' code                                                                           *
 *                                                                                                                    *
 **********************************************************************************************************************/

#ifndef __cplusplus
#define                WRK_CREATE( state, size ) \
               &(tsk_t)WRK_INIT( state, size )
#endif

/**********************************************************************************************************************
 *                                                                                                                    *
 * Name              : TSK_INIT                                                                                       *
 *                                                                                                                    *
 * Description       : create and initilize complete work area for task obj. with stack size defined by OS_STACK_SIZE *
 *                                                                                                                    *
 * Parameters                                                                                                         *
 *   state           : task state (initial task function) doesn't have to be noreturn-type                            *
 *                     it will be executed into an infinite system-implemented loop                                   *
 *                                                                                                                    *
 * Return            : task object                                                                                    *
 *                                                                                                                    *
 * Note              : use only in 'C' code                                                                           *
 *                                                                                                                    *
 **********************************************************************************************************************/

#define                TSK_INIT( state ) \
                       WRK_INIT( state, OS_STACK_SIZE )

/**********************************************************************************************************************
 *                                                                                                                    *
 * Name              : TSK_CREATE                                                                                     *
 *                                                                                                                    *
 * Description       : create and initilize complete work area for task obj. with stack size defined by OS_STACK_SIZE *
 *                                                                                                                    *
 * Parameters                                                                                                         *
 *   state           : task state (initial task function) doesn't have to be noreturn-type                            *
 *                     it will be executed into an infinite system-implemented loop                                   *
 *                                                                                                                    *
 * Return            : pointer to task object                                                                         *
 *                                                                                                                    *
 * Note              : use only in 'C' code                                                                           *
 *                                                                                                                    *
 **********************************************************************************************************************/

#ifndef __cplusplus
#define                TSK_CREATE( state ) \
                       WRK_CREATE( state, OS_STACK_SIZE )
#endif

/**********************************************************************************************************************
 *                                                                                                                    *
 * Name              : tsk_start                                                                                      *
 *                                                                                                                    *
 * Description       : start previously defined/created/stopped task object                                           *
 *                                                                                                                    *
 * Parameters                                                                                                         *
 *   tsk             : pointer to task object                                                                         *
 *                                                                                                                    *
 * Return            : none                                                                                           *
 *                                                                                                                    *
 **********************************************************************************************************************/

              void     tsk_start( tsk_id tsk );

/**********************************************************************************************************************
 *                                                                                                                    *
 * Name              : tsk_startFrom                                                                                  *
 *                                                                                                                    *
 * Description       : start previously defined/created/stopped task object                                           *
 *                                                                                                                    *
 * Parameters                                                                                                         *
 *   tsk             : pointer to task object                                                                         *
 *   state           : task state (initial task function) doesn't have to be noreturn-type                            *
 *                     it will be executed into an infinite system-implemented loop                                   *
 *                                                                                                                    *
 * Return            : none                                                                                           *
 *                                                                                                                    *
 **********************************************************************************************************************/

              void     tsk_startFrom( tsk_id tsk, fun_id state );

/**********************************************************************************************************************
 *                                                                                                                    *
 * Name              : tsk_stop                                                                                       *
 *                                                                                                                    *
 * Description       : stop current task and remove it from READY queue                                               *
 *                                                                                                                    *
 * Parameters                                                                                                         *
 *   tsk             : pointer to current task object                                                                 *
 *                                                                                                                    *
 * Return            : none                                                                                           *
 *                                                                                                                    *
 **********************************************************************************************************************/

              void     tsk_stop( void ) __NORETURN;

/**********************************************************************************************************************
 *                                                                                                                    *
 * Name              : tsk_join                                                                                       *
 *                                                                                                                    *
 * Description       : delay execution of current task until termination of given task                                *
 *                                                                                                                    *
 * Parameters                                                                                                         *
 *   tsk             : pointer to task object                                                                         *
 *                                                                                                                    *
 * Return            : none                                                                                           *
 *                                                                                                                    *
 **********************************************************************************************************************/

              void     tsk_join( tsk_id tsk );

/**********************************************************************************************************************
 *                                                                                                                    *
 * Name              : tsk_yield                                                                                      *
 *                                                                                                                    *
 * Description       : yield system control to the next task                                                          *
 *                                                                                                                    *
 * Parameters        : none                                                                                           *
 *                                                                                                                    *
 * Return            : none                                                                                           *
 *                                                                                                                    *
 **********************************************************************************************************************/

static inline void     tsk_yield( void ) { core_ctx_switch(); }

/**********************************************************************************************************************
 *                                                                                                                    *
 * Name              : tsk_pass                                                                                       *
 *                                                                                                                    *
 * Description       : the same as tsk_yield (force context switch)                                                   *
 *                                                                                                                    *
 * Parameters        : none                                                                                           *
 *                                                                                                                    *
 * Return            : none                                                                                           *
 *                                                                                                                    *
 **********************************************************************************************************************/

static inline void     tsk_pass ( void ) { core_ctx_switch(); }

/**********************************************************************************************************************
 *                                                                                                                    *
 * Name              : tsk_flip                                                                                       *
 *                                                                                                                    *
 * Description       : restart current task with the new state (task function)                                        *
 *                                                                                                                    *
 * Parameters                                                                                                         *
 *   proc            : new task state (task function)                                                                 *
 *                                                                                                                    *
 * Return            : none                                                                                           *
 *                                                                                                                    *
 **********************************************************************************************************************/

              void     tsk_flip( fun_id state ) __NORETURN;

/**********************************************************************************************************************
 *                                                                                                                    *
 * Name              : tsk_wait                                                                                       *
 *                                                                                                                    *
 * Description       : delay indefinitly execution of current task and wait for flags or message                      *
 *                                                                                                                    *
 * Parameters                                                                                                         *
 *   flags           : all flags to wait                                                                              *
 *                                                                                                                    *
 * Return            : none                                                                                           *
 *                                                                                                                    *
 **********************************************************************************************************************/

              void     tsk_wait( unsigned flags );

/**********************************************************************************************************************
 *                                                                                                                    *
 * Name              : tsk_give                                                                                       *
 *                                                                                                                    *
 * Description       : set given flags in waiting task (tsk_wait)                                                     *
 *                                                                                                                    *
 * Parameters                                                                                                         *
 *   tsk             : pointer to delayed task object                                                                 *
 *   flags           : flags or message transfered to the task                                                        *
 *                                                                                                                    *
 * Return            : none                                                                                           *
 *                                                                                                                    *
 **********************************************************************************************************************/

              void     tsk_give( tsk_id tsk, unsigned flags );

/**********************************************************************************************************************
 *                                                                                                                    *
 * Name              : tsk_sleepUntil                                                                                 *
 *                                                                                                                    *
 * Description       : delay execution of current task until given timepoint                                          *
 *                                                                                                                    *
 * Parameters                                                                                                         *
 *   time            : timepoint value                                                                                *
 *                                                                                                                    *
 * Return                                                                                                             *
 *   E_SUCCESS       : task object successfully finished countdown                                                    *
 *   'another'       : task was resumed with 'another' event value                                                    *
 *                                                                                                                    *
 **********************************************************************************************************************/

              unsigned tsk_sleepUntil( unsigned time );

/**********************************************************************************************************************
 *                                                                                                                    *
 * Name              : tsk_sleepFor                                                                                   *
 *                                                                                                                    *
 * Description       : delay execution of current task for given duration of time                                     *
 *                                                                                                                    *
 * Parameters                                                                                                         *
 *   delay           : duration of time (maximum number of ticks to delay execution of current task)                  *
 *                     IMMEDIATE: don't delay execution of current task                                               *
 *                     INFINITE:  delay indefinitly execution of current task                                         *
 *                                                                                                                    *
 * Return                                                                                                             *
 *   E_SUCCESS       : task object successfully finished countdown                                                    *
 *   'another'       : task was resumed with 'another' event value                                                    *
 *                                                                                                                    *
 **********************************************************************************************************************/

              unsigned tsk_sleepFor( unsigned delay );

/**********************************************************************************************************************
 *                                                                                                                    *
 * Name              : tsk_sleep                                                                                      *
 *                                                                                                                    *
 * Description       : delay indefinitly execution of current task                                                    *
 *                                                                                                                    *
 * Parameters        : none                                                                                           *
 *                                                                                                                    *
 * Return                                                                                                             *
 *   'another'       : task was resumed with 'another' event value                                                    *
 *                                                                                                                    *
 **********************************************************************************************************************/

static inline unsigned tsk_sleep( void ) { return tsk_sleepFor(INFINITE); }

/**********************************************************************************************************************
 *                                                                                                                    *
 * Name              : tsk_delay                                                                                      *
 *                                                                                                                    *
 * Description       : the same as tsk_sleepFor, delay execution of current task for given duration of time           *
 *                                                                                                                    *
 * Parameters                                                                                                         *
 *   delay           : duration of time (maximum number of ticks to delay execution of current task)                  *
 *                     IMMEDIATE: don't delay execution of current task                                               *
 *                     INFINITE:  delay indefinitly execution of current task                                         *
 *                                                                                                                    *
 * Return                                                                                                             *
 *   E_SUCCESS       : task object successfully finished countdown                                                    *
 *   'another'       : task was resumed with 'another' event value                                                    *
 *                                                                                                                    *
 **********************************************************************************************************************/

static inline unsigned tsk_delay( unsigned delay ) { return tsk_sleepFor(delay); }

/**********************************************************************************************************************
 *                                                                                                                    *
 * Name              : tsk_suspend                                                                                    *
 *                                                                                                                    *
 * Description       : the same as tsk_sleep, delay indefinitly execution of current task                             *
 *                                                                                                                    *
 * Parameters        : none                                                                                           *
 *                                                                                                                    *
 * Return                                                                                                             *
 *   'another'       : task was resumed with 'another' event value                                                    *
 *                                                                                                                    *
 **********************************************************************************************************************/

static inline unsigned tsk_suspend( void ) { return tsk_sleep(); }

/**********************************************************************************************************************
 *                                                                                                                    *
 * Name              : tsk_resume                                                                                     *
 *                                                                                                                    *
 * Description       : resume execution of given delayed task                                                         *
 *                                                                                                                    *
 * Parameters                                                                                                         *
 *   tsk             : pointer to delayed task object                                                                 *
 *   event           : the value at which the given task is woken up                                                  *
 *                                                                                                                    *
 * Return            : none                                                                                           *
 *                                                                                                                    *
 **********************************************************************************************************************/

              void     tsk_resume( tsk_id tsk, unsigned event );

#ifdef __cplusplus
}
#endif

/* -------------------------------------------------------------------------- */

#ifdef __cplusplus

/**********************************************************************************************************************
 *                                                                                                                    *
 * Namespace         : ThisTask                                                                                       *
 *                                                                                                                    *
 * Description       : provide set of functions for Current Task                                                      *
 *                                                                                                                    *
 **********************************************************************************************************************/

namespace ThisTask
{
	void     pass      ( void )            {        tsk_pass      ();             }
	void     yield     ( void )            {        tsk_yield     ();             }
	void     flip      ( fun_id   _state ) {        tsk_flip      (_state);       }
	void     stop      ( void )            {        tsk_stop      ();             }

	unsigned sleepUntil( unsigned _time )  { return tsk_sleepUntil(_time);        }
	unsigned sleepFor  ( unsigned _delay ) { return tsk_sleepFor  (_delay);       }
	unsigned sleep     ( void )            { return tsk_sleep     ();             }
	unsigned delay     ( unsigned _delay ) { return tsk_delay     (_delay);       }
	unsigned suspend   ( void )            { return tsk_suspend   ();             }
}

/**********************************************************************************************************************
 *                                                                                                                    *
 * Class             : TaskT<>                                                                                        *
 *                                                                                                                    *
 * Description       : create and initilize complete work area for task object                                        *
 *                                                                                                                    *
 * Constructor parameters                                                                                             *
 *   size            : size of task private stack (in bytes)                                                          *
 *   prio            : initial task priority (any unsigned int value)                                                 *
 *   state           : task state (initial task function) doesn't have to be noreturn-type                            *
 *                     it will be executed into an infinite system-implemented loop                                   *
 *                                                                                                                    *
 **********************************************************************************************************************/

template<unsigned _size>
class TaskT : public __tsk
{
	stk_t _stack[ASIZE(_size)];

public:

	explicit
	TaskT( const fun_id _state ): __tsk _TSK_INIT(0, _stack+ASIZE(_size)) { state = _state; }

	void     join      ( void )            {        tsk_join      (this);         }
	void     start     ( void )            {        tsk_start     (this);         }
	void     startFrom ( fun_id   _state ) {        tsk_startFrom (this, _state); }
	void     resume    ( unsigned _event ) {        tsk_resume    (this, _event); }

	bool     operator! ( void )            { return __tsk::id == ID_STOPPED;      }
};

/**********************************************************************************************************************
 *                                                                                                                    *
 * Class             : Task                                                                                           *
 *                                                                                                                    *
 * Description       : create and initilize complete work area for task object with default stack size                *
 *                                                                                                                    *
 * Constructor parameters                                                                                             *
 *   prio            : initial task priority (any unsigned int value)                                                 *
 *   state           : task state (initial task function) doesn't have to be noreturn-type                            *
 *                     it will be executed into an infinite system-implemented loop                                   *
 *                                                                                                                    *
 **********************************************************************************************************************/

class Task: public TaskT<OS_STACK_SIZE>
{
public:

	explicit
	Task( const fun_id _state ): TaskT<OS_STACK_SIZE>(_state) {}
};

/**********************************************************************************************************************
 *                                                                                                                    *
 * Class             : startTaskT<>                                                                                   *
 *                                                                                                                    *
 * Description       : create and initilize complete work area for task object                                        *
 *                     and start task object                                                                          *
 *                                                                                                                    *
 * Constructor parameters                                                                                             *
 *   size            : size of task private stack (in bytes)                                                          *
 *   prio            : initial task priority (any unsigned int value)                                                 *
 *   state           : task state (initial task function) doesn't have to be noreturn-type                            *
 *                     it will be executed into an infinite system-implemented loop                                   *
 *                                                                                                                    *
 **********************************************************************************************************************/

template<unsigned _size>
class startTaskT : public TaskT<_size>
{
public:

	explicit
	startTaskT( const fun_id _state ): TaskT<_size>(_state) { tsk_start(this); }
};

/**********************************************************************************************************************
 *                                                                                                                    *
 * Class             : startTask                                                                                      *
 *                                                                                                                    *
 * Description       : create and initilize complete work area for task object with default stack size                *
 *                     and start task object                                                                          *
 *                                                                                                                    *
 * Constructor parameters                                                                                             *
 *   prio            : initial task priority (any unsigned int value)                                                 *
 *   state           : task state (initial task function) doesn't have to be noreturn-type                            *
 *                     it will be executed into an infinite system-implemented loop                                   *
 *                                                                                                                    *
 **********************************************************************************************************************/

class startTask : public startTaskT<OS_STACK_SIZE>
{
public:

	explicit
	startTask( const fun_id _state ): startTaskT<OS_STACK_SIZE>(_state) {}
};

#endif

/* -------------------------------------------------------------------------- */

#endif//__INTROS_TSK_H
