/******************************************************************************

    @file    IntrOS: os_tmr.h
    @author  Rajmund Szymanski
    @date    30.03.2017
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

#ifndef __INTROS_TMR_H
#define __INTROS_TMR_H

#include <oskernel.h>

#ifdef __cplusplus
extern "C" {
#endif

/**********************************************************************************************************************
 *                                                                                                                    *
 * Name              : timer                                                                                          *
 *                                                                                                                    *
 **********************************************************************************************************************/

struct __tmr
{
	unsigned id;    // inherited from object
	tmr_t  * prev;  // inherited from object
	tmr_t  * next;  // inherited from object
	unsigned signal;

	fun_t  * state; // callback procedure
	unsigned start;
	unsigned delay;
	unsigned period;
};

/**********************************************************************************************************************
 *                                                                                                                    *
 * Name              : _TMR_INIT                                                                                      *
 *                                                                                                                    *
 * Description       : create and initilize a timer object                                                            *
 *                                                                                                                    *
 * Parameters                                                                                                         *
 *   state           : callback procedure                                                                             *
 *                     0: no callback                                                                                 *
 *                                                                                                                    *
 * Return            : timer object                                                                                   *
 *                                                                                                                    *
 * Note              : for internal use                                                                               *
 *                                                                                                                    *
 **********************************************************************************************************************/

#define               _TMR_INIT( _state ) { 0, 0, 0, 0, _state, 0, 0, 0 }

/**********************************************************************************************************************
 *                                                                                                                    *
 * Name              : OS_TMR                                                                                         *
 *                                                                                                                    *
 * Description       : define and initilize a timer object                                                            *
 *                                                                                                                    *
 * Parameters                                                                                                         *
 *   tmr             : name of a pointer to timer object                                                              *
 *   state           : callback procedure                                                                             *
 *                     0: no callback                                                                                 *
 *                                                                                                                    *
 **********************************************************************************************************************/

#define             OS_TMR( tmr, state )                    \
                       void state( void );                   \
                       tmr_t tmr##__tmr = _TMR_INIT( state ); \
                       tmr_t * const tmr = & tmr##__tmr

/**********************************************************************************************************************
 *                                                                                                                    *
 * Name              : static_TMR                                                                                     *
 *                                                                                                                    *
 * Description       : define and initilize a static timer object                                                     *
 *                                                                                                                    *
 * Parameters                                                                                                         *
 *   tmr             : name of a pointer to timer object                                                              *
 *   state           : callback procedure                                                                             *
 *                     0: no callback                                                                                 *
 *                                                                                                                    *
 **********************************************************************************************************************/

#define         static_TMR( tmr, state )                    \
                static void state( void );                   \
                static tmr_t tmr##__tmr = _TMR_INIT( state ); \
                static tmr_t * const tmr = & tmr##__tmr

/**********************************************************************************************************************
 *                                                                                                                    *
 * Name              : TMR_INIT                                                                                       *
 *                                                                                                                    *
 * Description       : create and initilize a timer object                                                            *
 *                                                                                                                    *
 * Parameters                                                                                                         *
 *   state           : callback procedure                                                                             *
 *                     0: no callback                                                                                 *
 *                                                                                                                    *
 * Return            : timer object                                                                                   *
 *                                                                                                                    *
 * Note              : use only in 'C' code                                                                           *
 *                                                                                                                    *
 **********************************************************************************************************************/

#ifndef __cplusplus
#define                TMR_INIT( state ) \
                      _TMR_INIT( state )
#endif

/**********************************************************************************************************************
 *                                                                                                                    *
 * Name              : TMR_CREATE                                                                                     *
 *                                                                                                                    *
 * Description       : create and initilize a timer object                                                            *
 *                                                                                                                    *
 * Parameters                                                                                                         *
 *   state           : callback procedure                                                                             *
 *                     0: no callback                                                                                 *
 *                                                                                                                    *
 * Return            : pointer to timer object                                                                        *
 *                                                                                                                    *
 * Note              : use only in 'C' code                                                                           *
 *                                                                                                                    *
 **********************************************************************************************************************/

#ifndef __cplusplus
#define                TMR_CREATE( state ) \
             & (tmr_t) TMR_INIT( state )
#endif

/**********************************************************************************************************************
 *                                                                                                                    *
 * Name              : tmr_startUntil                                                                                 *
 *                                                                                                                    *
 * Description       : start/restart one-shot timer until given timepoint and then launch the callback procedure      *
 *                                                                                                                    *
 * Parameters                                                                                                         *
 *   tmr             : pointer to timer object                                                                        *
 *   time            : timepoint value                                                                                *
 *                                                                                                                    *
 * Return            : none                                                                                           *
 *                                                                                                                    *
 **********************************************************************************************************************/

void tmr_startUntil( tmr_t *tmr, unsigned time );

/**********************************************************************************************************************
 *                                                                                                                    *
 * Name              : tmr_start                                                                                      *
 *                                                                                                                    *
 * Description       : start/restart periodic timer for given duration of time and then launch the callback procedure *
 *                     do this periodically if period > 0                                                             *
 *                                                                                                                    *
 * Parameters                                                                                                         *
 *   tmr             : pointer to timer object                                                                        *
 *   delay           : duration of time (maximum number of ticks to countdown) for first expiration                   *
 *                     IMMEDIATE: don't countdown                                                                     *
 *                     INFINITE:  countdown indefinitly                                                               *
 *   period          : duration of time (maximum number of ticks to countdown) for all next expirations               *
 *                     IMMEDIATE: don't countdown                                                                     *
 *                     INFINITE:  countdown indefinitly                                                               *
 *                                                                                                                    *
 * Return            : none                                                                                           *
 *                                                                                                                    *
 **********************************************************************************************************************/

void tmr_start( tmr_t *tmr, unsigned delay, unsigned period );

/**********************************************************************************************************************
 *                                                                                                                    *
 * Name              : tmr_startFor                                                                                   *
 *                                                                                                                    *
 * Description       : start/restart one-shot timer for given duration of time and then launch the callback procedure *
 *                                                                                                                    *
 * Parameters                                                                                                         *
 *   tmr             : pointer to timer object                                                                        *
 *   delay           : duration of time (maximum number of ticks to countdown)                                        *
 *                     IMMEDIATE: don't countdown                                                                     *
 *                     INFINITE:  countdown indefinitly                                                               *
 *                                                                                                                    *
 * Return            : none                                                                                           *
 *                                                                                                                    *
 **********************************************************************************************************************/

__STATIC_INLINE
void tmr_startFor( tmr_t *tmr, unsigned delay ) { tmr_start(tmr, delay, 0); }

/**********************************************************************************************************************
 *                                                                                                                    *
 * Name              : tmr_startPeriodic                                                                              *
 *                                                                                                                    *
 * Description       : start/restart periodic timer for given duration of time and then launch the callback procedure *
 *                     do this periodically                                                                           *
 *                                                                                                                    *
 * Parameters                                                                                                         *
 *   tmr             : pointer to timer object                                                                        *
 *   period          : duration of time (maximum number of ticks to countdown)                                        *
 *                     IMMEDIATE: don't countdown                                                                     *
 *                     INFINITE:  countdown indefinitly                                                               *
 *                                                                                                                    *
 * Return            : none                                                                                           *
 *                                                                                                                    *
 **********************************************************************************************************************/

__STATIC_INLINE
void tmr_startPeriodic( tmr_t *tmr, unsigned period ) { tmr_start(tmr, period, period); }

/**********************************************************************************************************************
 *                                                                                                                    *
 * Name              : tmr_startFrom                                                                                  *
 *                                                                                                                    *
 * Description       : start/restart periodic timer for given duration of time and then launch the callback procedure *
 *                     do this periodically if period > 0                                                             *
 *                                                                                                                    *
 * Parameters                                                                                                         *
 *   tmr             : pointer to timer object                                                                        *
 *   delay           : duration of time (maximum number of ticks to countdown) for first expiration                   *
 *                     IMMEDIATE: don't countdown                                                                     *
 *                     INFINITE:  countdown indefinitly                                                               *
 *   period          : duration of time (maximum number of ticks to countdown) for all next expirations               *
 *                     IMMEDIATE: don't countdown                                                                     *
 *                     INFINITE:  countdown indefinitly                                                               *
 *   proc            : callback procedure                                                                             *
 *                     0: no callback                                                                                 *
 *                                                                                                                    *
 * Return            : none                                                                                           *
 *                                                                                                                    *
 **********************************************************************************************************************/

void tmr_startFrom( tmr_t *tmr, unsigned delay, unsigned period, fun_t *proc );

/**********************************************************************************************************************
 *                                                                                                                    *
 * Name              : tmr_wait                                                                                       *
 *                                                                                                                    *
 * Description       : wait indefinitly until the timer finishes countdown                                            *
 *                                                                                                                    *
 * Parameters                                                                                                         *
 *   tmr             : pointer to timer object                                                                        *
 *                                                                                                                    *
 * Return            : none                                                                                           *
 *                                                                                                                    *
 **********************************************************************************************************************/

void tmr_wait( tmr_t *tmr );

/**********************************************************************************************************************
 *                                                                                                                    *
 * Name              : tmr_take                                                                                       *
 *                                                                                                                    *
 * Description       : check if the timer finishes countdown                                                          *
 *                                                                                                                    *
 * Parameters                                                                                                         *
 *   tmr             : pointer to timer object                                                                        *
 *                                                                                                                    *
 * Return                                                                                                             *
 *   E_SUCCESS       : timer object successfully finished countdown                                                   *
 *   E_FAILURE       : timer object has not yet completed counting                                                    *
 *                                                                                                                    *
 **********************************************************************************************************************/

unsigned tmr_take( tmr_t *tmr );

/**********************************************************************************************************************
 *                                                                                                                    *
 * Name              : tmr_flip                                                                                       *
 *                                                                                                                    *
 * Description       : change callback procedure for current periodic timer (available in next period)                *
 *                                                                                                                    *
 * Parameters                                                                                                         *
 *   proc            : new callback procedure                                                                         *
 *                     0: no callback                                                                                 *
 *                                                                                                                    *
 * Return            : none                                                                                           *
 *                                                                                                                    *
 * Note              : use only in timer's callback procedure                                                         *
 *                                                                                                                    *
 **********************************************************************************************************************/

__STATIC_INLINE
void tmr_flip( fun_t *proc ) { ((tmr_t *)Current)->state = proc; }

/**********************************************************************************************************************
 *                                                                                                                    *
 * Name              : tmr_delay                                                                                      *
 *                                                                                                                    *
 * Description       : change delay time for current timer                                                            *
 *                                                                                                                    *
 * Parameters                                                                                                         *
 *   delay           : duration of time (maximum number of ticks to countdown)                                        *
 *                     0:         current timer is stopped even if it was periodic                                    *
 *                     otherwise: current timer is restarted even if it was one-shot                                  *
 *                                                                                                                    *
 * Return            : none                                                                                           *
 *                                                                                                                    *
 * Note              : use only in timer's callback procedure                                                         *
 *                                                                                                                    *
 **********************************************************************************************************************/

__STATIC_INLINE
void tmr_delay( unsigned delay ) { ((tmr_t *)Current)->delay = delay; }

#ifdef __cplusplus
}
#endif

/* -------------------------------------------------------------------------- */

#ifdef __cplusplus

/**********************************************************************************************************************
 *                                                                                                                    *
 * Namespace         : ThisTimer                                                                                      *
 *                                                                                                                    *
 * Description       : provide set of functions for Current Timer                                                     *
 *                                                                                                                    *
 **********************************************************************************************************************/

namespace ThisTimer
{
	void flip ( fun_t  * _state ) { tmr_flip (_state); }
	void delay( unsigned _delay ) { tmr_delay(_delay); }
}

/**********************************************************************************************************************
 *                                                                                                                    *
 * Class             : Timer                                                                                          *
 *                                                                                                                    *
 * Description       : create and initilize a timer object                                                            *
 *                                                                                                                    *
 * Constructor parameters                                                                                             *
 *                   : none                                                                                           *
 *                                                                                                                    *
 **********************************************************************************************************************/

struct Timer : public __tmr
{
	explicit
	Timer( fun_t *_state = nullptr ): __tmr _TMR_INIT(0) { state = _state; }

	void startUntil   ( unsigned _time )                                   {        tmr_startUntil   (this, _time);                   }
	void start        ( unsigned _delay, unsigned _period )                {        tmr_start        (this, _delay, _period);         }
	void startFor     ( unsigned _delay )                                  {        tmr_startFor     (this, _delay);                  }
	void startPeriodic( unsigned _period )                                 {        tmr_startPeriodic(this,         _period);         }
	void startFrom    ( unsigned _delay, unsigned _period, fun_t *_state ) {        tmr_startFrom    (this, _delay, _period, _state); }

	void     wait     ( void )                                             {        tmr_wait         (this);                          }
	unsigned take     ( void )                                             { return tmr_take         (this);                          }

	bool     operator!( void )                                             { return __tmr::id == ID_STOPPED;                          }
};

/**********************************************************************************************************************
 *                                                                                                                    *
 * Class             : startTimerUntil                                                                                *
 *                                                                                                                    *
 * Description       : create and initilize a timer object                                                            *
 *                     and start one-shot timer until given timepoint and then launch the callback procedure          *
 *                                                                                                                    *
 * Constructor parameters                                                                                             *
 *   time            : timepoint value                                                                                *
 *   state           : callback procedure                                                                             *
 *                     nullptr: no callback                                                                           *
 *                                                                                                                    *
 **********************************************************************************************************************/

struct startTimerUntil : public Timer
{
	explicit
	startTimerUntil( const unsigned _time, fun_t *_state = nullptr ): Timer(_state) { tmr_startUntil(this, _time); }
};

/**********************************************************************************************************************
 *                                                                                                                    *
 * Class             : startTimer                                                                                     *
 *                                                                                                                    *
 * Description       : create and initilize a timer object                                                            *
 *                     and start periodic timer for given duration of time and then launch the callback procedure     *
 *                     do this periodically                                                                           *
 *                                                                                                                    *
 * Constructor parameters                                                                                             *
 *   delay           : duration of time (maximum number of ticks to countdown) for first expiration                   *
 *                     IMMEDIATE: don't countdown                                                                     *
 *                     INFINITE:  countdown indefinitly                                                               *
 *   period          : duration of time (maximum number of ticks to countdown) for all next expirations               *
 *                     IMMEDIATE: don't countdown                                                                     *
 *                     INFINITE:  countdown indefinitly                                                               *
 *   state           : callback procedure                                                                             *
 *                     nullptr: no callback                                                                           *
 *                                                                                                                    *
 **********************************************************************************************************************/

struct startTimer : public Timer
{
	explicit
	startTimer( const unsigned _delay, const unsigned _period, fun_t *_state = nullptr ): Timer(_state) { tmr_start(this, _delay, _period); }
};

/**********************************************************************************************************************
 *                                                                                                                    *
 * Class             : startTimerFor                                                                                  *
 *                                                                                                                    *
 * Description       : create and initilize a timer object                                                            *
 *                     and start one-shot timer for given duration of time and then launch the callback procedure     *
 *                                                                                                                    *
 * Constructor parameters                                                                                             *
 *   delay           : duration of time (maximum number of ticks to countdown)                                        *
 *                     IMMEDIATE: don't countdown                                                                     *
 *                     INFINITE:  countdown indefinitly                                                               *
 *   state           : callback procedure                                                                             *
 *                     nullptr: no callback                                                                           *
 *                                                                                                                    *
 **********************************************************************************************************************/

struct startTimerFor : public Timer
{
	explicit
	startTimerFor( const unsigned _delay, fun_t *_state = nullptr ): Timer(_state) { tmr_startFor(this, _delay); }
};

/**********************************************************************************************************************
 *                                                                                                                    *
 * Class             : startTimerPeriodic                                                                             *
 *                                                                                                                    *
 * Description       : create and initilize a timer object                                                            *
 *                     and start periodic timer for given duration of time and then launch the callback procedure     *
 *                     do this periodically                                                                           *
 *                                                                                                                    *
 * Constructor parameters                                                                                             *
 *   period          : duration of time (maximum number of ticks to countdown)                                        *
 *                     IMMEDIATE: don't countdown                                                                     *
 *                     INFINITE:  countdown indefinitly                                                               *
 *   state           : callback procedure                                                                             *
 *                     nullptr: no callback                                                                           *
 *                                                                                                                    *
 **********************************************************************************************************************/

struct startTimerPeriodic : public Timer
{
	explicit
	startTimerPeriodic( const unsigned _period, fun_t *_state = nullptr ): Timer(_state) { tmr_startPeriodic(this, _period); }
};

#endif//__cplusplus

/* -------------------------------------------------------------------------- */

#endif//__INTROS_TMR_H
