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

#include <ntddk.h>

typedef struct _SPLICE_BACKUP
{
	ULONG olddata1;
	ULONG olddata2;
	ULONG cr0;
	ULONG flags;
	PVOID ptr;
} SPLICE_BACKUP, *PSPLICE_BACKUP;

void splice_install(PSPLICE_BACKUP backup, PVOID Function, PVOID Hook);
void splice_uninstall(PSPLICE_BACKUP Backup);