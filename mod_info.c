/*
 * Copyright (C) 2013 Roman I. Volkov
 *
 *  This driver is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License, version 2.
 *
 *  This driver is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this driver; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 *
 */

#include "mod_info.h"

PLDR_DATA_TABLE_ENTRY mod_info_find_first(IN PDRIVER_OBJECT DriverObject)
{
	return (PLDR_DATA_TABLE_ENTRY)DriverObject->DriverSection;
}

PLDR_DATA_TABLE_ENTRY mod_info_get_next(IN PLDR_DATA_TABLE_ENTRY First, IN PLDR_DATA_TABLE_ENTRY Prev)
{
	PLDR_DATA_TABLE_ENTRY next;
	next = CONTAINING_RECORD(Prev->LoadOrder.Blink, LDR_DATA_TABLE_ENTRY, LoadOrder);
	if (next == First) /* don't loop the ring again */
		return NULL;
	return next;
}