/** @file

  Copyright (c) 2008 - 2010, Apple Inc. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef __S3C6410LIB_H__
#define __S3C6410LIB_H__

UINT32
GpioBase (
  IN  UINT8 Port
  );

UINT32
EFIAPI
TimerBase (
  IN  UINTN Timer
  );

UINTN
EFIAPI
InterruptVectorForTimer (
  IN  UINTN TImer
  );

UINT32
EFIAPI
UartBase (
  IN  UINTN Uart
  );


#endif // __S3C6410LIB_H__

