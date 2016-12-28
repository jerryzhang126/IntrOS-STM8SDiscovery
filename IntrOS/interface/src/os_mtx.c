/******************************************************************************

    @file    IntrOS: os_mtx.c
    @author  Rajmund Szymanski
    @date    28.12.2016
    @brief   This file provides set of functions for IntrOS.

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

#include <os.h>

/* -------------------------------------------------------------------------- */
unsigned mtx_take( mtx_id mtx )
/* -------------------------------------------------------------------------- */
{
	unsigned event = E_FAILURE;

	assert(mtx);

	if (mtx->owner == 0)
	{
		mtx->owner = Current;
	}

	if (mtx->owner == Current)
	{
		event = E_SUCCESS;
	}
	
	return event;
}

/* -------------------------------------------------------------------------- */
void mtx_wait( mtx_id mtx )
/* -------------------------------------------------------------------------- */
{
	while (mtx_take(mtx) != E_SUCCESS) tsk_yield();
}

/* -------------------------------------------------------------------------- */
unsigned mtx_give( mtx_id mtx )
/* -------------------------------------------------------------------------- */
{
	unsigned event = E_FAILURE;
	
	assert(mtx);

	if (mtx->owner == Current)
	{
	    mtx->owner = 0;

		event = E_SUCCESS;
	}

	return event;
}

/* -------------------------------------------------------------------------- */
