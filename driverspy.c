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

#include "driverspy.h"

void DriverUnload(IN PDRIVER_OBJECT pDriverObject);
NTSTATUS DeviceControl(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
NTSTATUS CreateClose(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);

UNICODE_STRING devName;
UNICODE_STRING dosName;
PDEVICE_OBJECT devObj;

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING pRegistryPath) 
{
	NTSTATUS status;
	PLDR_DATA_TABLE_ENTRY first;
	PLDR_DATA_TABLE_ENTRY next;
	PLDR_DATA_TABLE_ENTRY found = NULL;
	UNICODE_STRING targetName;

	DbgPrint("PH::DriverEntry");

	DriverObject->DriverUnload = DriverUnload;
	DriverObject->MajorFunction[IRP_MJ_CREATE] = CreateClose;
	DriverObject->MajorFunction[IRP_MJ_CLOSE] = CreateClose;
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DeviceControl;

	RtlUnicodeStringInit(&devName, PH_NT_DEVICE_NAME);
	RtlUnicodeStringInit(&dosName, PH_DOS_DEVICE_NAME);

	status = IoCreateDevice(
		DriverObject, 0, &devName,
		FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN,
		FALSE, &devObj);

	if (!NT_SUCCESS(status))
	{
		DbgPrint("IoCreateDevice() failed with error %d", status);
		return status;
	}

	status = IoCreateSymbolicLink(&dosName, &devName);

	if (!NT_SUCCESS(status)) 
	{
		DbgPrint("IoCreateSymbolicLink() failed with error %d", status);
		IoDeleteDevice(devObj);
		return status;
	}

	// Find module
	RtlUnicodeStringInit(&targetName, PH_TARGET_MODULE_NAME);
	first = mod_info_find_first(DriverObject);
	next = first;
	while ((next = mod_info_get_next(first, next)) != NULL)
	{
		if (RtlEqualUnicodeString(&next->ModuleName, &targetName, TRUE))
			found = next;
		// seems module names are always null-terminated
		DbgPrint("PH:Module found: %S", next->ModuleName.Buffer);
	}
	//fltCallerEntry = 0;
	//fltCallerSize = 0xffffffff;
	if (found != NULL)
	{
		fltCallerEntry = (ULONG)found->ModuleBaseAddress;
		fltCallerSize = found->ModuleSize;
		if (fltCallerEntry != 0)
		{
			DbgPrint("PH:Module %S found, setting the hooks...", PH_TARGET_MODULE_NAME);
			splice_install(&b_wpchar, WRITE_PORT_UCHAR, __FAKE_WRITE_PORT_UCHAR);
			splice_install(&b_wpshort, WRITE_PORT_USHORT, __FAKE_WRITE_PORT_USHORT);
			splice_install(&b_wplong, WRITE_PORT_ULONG, __FAKE_WRITE_PORT_ULONG);
		}
	} else
	{
		DbgPrint("PH:Module %S not found, doing nothing...", PH_TARGET_MODULE_NAME);
		fltCallerEntry = 0;
	}

	return STATUS_SUCCESS;
}

void DriverUnload(IN PDRIVER_OBJECT pDriverObject)
{
	DbgPrint("PH::DriverUnload");

	if (fltCallerEntry != 0)
	{
		splice_uninstall(&b_wpchar);
		splice_uninstall(&b_wpshort);
		splice_uninstall(&b_wplong);
	}

	IoDeleteSymbolicLink(&dosName);
	IoDeleteDevice(devObj);
}

NTSTATUS DeviceControl(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp) 
{
	PIO_STACK_LOCATION iosl;

	DbgPrint("PH::DeviceControl() stub");
	iosl = IoGetCurrentIrpStackLocation(Irp);
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	Irp->IoStatus.Status = STATUS_SUCCESS;
	return Irp->IoStatus.Status;
}

NTSTATUS CreateClose(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
	PIO_STACK_LOCATION iosl;

	DbgPrint("PH::Create()\\Close() stub");
	iosl = IoGetCurrentIrpStackLocation(Irp);
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	Irp->IoStatus.Status = STATUS_SUCCESS;
	return Irp->IoStatus.Status;
}