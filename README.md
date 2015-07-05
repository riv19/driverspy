# driverspy
WRITE_PORT_UCHAR/USHORT/ULONG functions tracer for reverse engineers (Windows Kernel)

How to compile:

Use Microsoft DDK 2005 to compile this driver.
This driver hooks only WRITE_PORT_UCHAR\USHORT\ULONG functions, modify driver to hook other functions.
This driver hooks only "cmudaxp.sys" module. Change this to hook another module.
The compiler will make the driverspy.sys file for you.

How to use:

Use Windows XP Service Pack 3.
Use any driver loading\unloading utility like DrvLoader (http://www.codeproject.com/Articles/31905/A-C-class-wrapper-to-load-unload-device-drivers).
Use DbgView utility from Mark Russinovich to view any messages posted by the driver.
Both utilities are provided.

Visit http://v1ron.ru for more information.