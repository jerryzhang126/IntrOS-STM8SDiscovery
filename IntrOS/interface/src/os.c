/******************************************************************************

    @file    IntrOS: os.c
    @author  Rajmund Szymanski
    @date    06.07.2017
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
uint32_t sys_time( void )
/* -------------------------------------------------------------------------- */
{
	uint32_t cnt;

	port_cnt_lock();

	cnt = Counter;

	port_cnt_unlock();

	return cnt;
}

/* -------------------------------------------------------------------------- */
