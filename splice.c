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

#include "splice.h"

// Clears CPU WP flag (Exception when write read-only pages) and disables interrupts 
__declspec(naked) void __stdcall clear_wp(PSPLICE_BACKUP backup)
{
	_asm
	{	//
		mov edi,edi
		push ebp
		mov ebp,esp
		//
		mov edx,[ebp+8]
		mov eax,cr0
		mov dword ptr [edx].cr0,eax
		and eax,0xfffeffff
		mov cr0,eax
		lahf
		mov dword ptr [edx].flags,eax
		cld
		//
		mov esp,ebp
		pop ebp
		retn 4
	}
}

// Restores original CPU flags after disabling WP and interrupts
__declspec(naked) void __stdcall restore_wp(PSPLICE_BACKUP backup)
{
	_asm
	{
		//
		mov edi,edi
		push ebp
		mov ebp,esp
		//
		mov edx,[ebp+8]
		mov eax,dword ptr [edx].cr0
		mov cr0,eax
		mov eax,dword ptr [edx].flags
		sahf
		//
		mov esp,ebp
		pop ebp
		retn 4
	}
}

void splice_install(PSPLICE_BACKUP Backup, PVOID Function, PVOID Hook)
{
	clear_wp(Backup);

	// Save old code
	Backup->ptr = Function;
	Backup->olddata1 = ((ULONG*)Function)[0];
	Backup->olddata2 = ((ULONG*)Function)[1];

	// Patch the code to jump to our hook function
	*(USHORT*)(((ULONG)Function) + 0) = 0xfeeb;	// temporary spinlock jmp $ if multithreaded
	*(UCHAR*)(((ULONG)Function) + 2) = 0x68;	// push
	*(VOID**)(((ULONG)Function) + 3) = Hook;	// linear address of function
	*(UCHAR*)(((ULONG)Function) + 7) = 0xc3;	// ret
	*(USHORT*)(((ULONG)Function) + 0) = 0x9090;	// nop nop - remove spinlock

	restore_wp(Backup);
}

void splice_uninstall(PSPLICE_BACKUP Backup)
{
	clear_wp(Backup);

	// Restore the original function's body
	((ULONG*)Backup->ptr)[0] = Backup->olddata1;
	((ULONG*)Backup->ptr)[1] = Backup->olddata2;

	restore_wp(Backup);
}