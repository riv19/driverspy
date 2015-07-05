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

#include "hooks.h"

SPLICE_BACKUP b_wpchar;
SPLICE_BACKUP b_wpshort;
SPLICE_BACKUP b_wplong;
ULONG fltCallerEntry;
ULONG fltCallerSize;

// standard prolog for stdcall and MS compiler:
// mov edi,edi
// push ebp
// mov ebp,esp
// So, caller address should be ebp+4

void __stdcall __FAKE_WRITE_PORT_UCHAR(PUCHAR Port, UCHAR Value)
{
	ULONG caller;
	
	_asm
	{
		mov eax,dword ptr [ebp+4]
		mov dword ptr [caller],eax
	}

	if ((caller >= fltCallerEntry) && (caller < (fltCallerEntry + fltCallerSize)))
		DbgPrint("__FAKE_WRITE_PORT_UCHAR: port %#x value %u", (ULONG)Port, (ULONG)Value);

	splice_uninstall(&b_wpchar);
	WRITE_PORT_UCHAR(Port, Value);
	splice_install(&b_wpchar, WRITE_PORT_UCHAR, __FAKE_WRITE_PORT_UCHAR);
}

void __stdcall __FAKE_WRITE_PORT_USHORT(PUSHORT Port, USHORT Value)
{
	ULONG caller;
	
	_asm
	{
		mov eax,dword ptr [ebp+4]
		mov dword ptr [caller],eax
	}

	if ((caller >= fltCallerEntry) && (caller < (fltCallerEntry + fltCallerSize)))
		DbgPrint("__FAKE_WRITE_PORT_USHORT: port %#x value %u", (ULONG)Port, (ULONG)Value);

	splice_uninstall(&b_wpshort);
	WRITE_PORT_USHORT(Port, Value);
	splice_install(&b_wpshort, WRITE_PORT_USHORT, __FAKE_WRITE_PORT_USHORT);
}

void __stdcall __FAKE_WRITE_PORT_ULONG(PULONG Port, ULONG Value)
{
	ULONG caller;
	
	_asm
	{
		mov eax,dword ptr [ebp+4]
		mov dword ptr [caller],eax
	}

	if ((caller >= fltCallerEntry) && (caller < (fltCallerEntry + fltCallerSize)))
		DbgPrint("__FAKE_WRITE_PORT_ULONG: port %#x value %u", (ULONG)Port, (ULONG)Value);

	splice_uninstall(&b_wplong);
	WRITE_PORT_ULONG(Port, Value);
	splice_install(&b_wplong, WRITE_PORT_ULONG, __FAKE_WRITE_PORT_ULONG);
}