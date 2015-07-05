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
#include <ntstrsafe.h>
#include <windef.h>
#include "hooks.h"
#include "mod_info.h"

#define PH_NT_DEVICE_NAME 		L"\\Device\\DRVSPY"
#define PH_DOS_DEVICE_NAME		L"\\DosDevices\\drvspy"
#define PH_MEMORY_TAG			(ULONG)'PHMM'
#define PH_TARGET_MODULE_NAME		L"cmudaxp.sys"

#define IOCTL_KFCM_MAKE_MODULES_LIST \
    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x900, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_KFCM_RETRIEVE_MODULES_LIST \
    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x901, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_KFCM_HOOK_FUNCTION \
    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x902, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_KFCM_UNHOOK_FUNCTION \
    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x903, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)

NTSTATUS DriverEntry(IN PDRIVER_OBJECT pDriverObject, IN PUNICODE_STRING pRegistryPath);
void DriverUnload(IN PDRIVER_OBJECT pDriverObject);
NTSTATUS CreateClose(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
NTSTATUS DeviceControl(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);