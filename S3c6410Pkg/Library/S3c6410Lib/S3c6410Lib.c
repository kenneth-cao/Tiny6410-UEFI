/** @file

  Copyright (c) 2008 - 2009, Apple Inc. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <Base.h>
#include <Library/DebugLib.h>
#include <Library/S3c6410Lib.h>
#include <S3c6410/S3c6410.h>

UINT32
GpioBase (
  IN  CHAR8 Port
  )
{
  switch (Port) {
  case 'A':  return GPIOA_BASE;
  case 'B':  return GPIOB_BASE;
  case 'C':  return GPIOC_BASE;
  case 'D':  return GPIOD_BASE;
  case 'E':  return GPIOE_BASE;
  case 'F':  return GPIOF_BASE;
  case 'G':  return GPIOG_BASE;
  case 'H':  return GPIOH_BASE;
  case 'I':  return GPIOI_BASE;
  case 'J':  return GPIOJ_BASE;
  case 'K':  return GPIOK_BASE;
  case 'L':  return GPIOL_BASE;
  case 'M':  return GPIOM_BASE;
  case 'N':  return GPION_BASE;
  case 'O':  return GPIOO_BASE;
  case 'P':  return GPIOP_BASE;
  case 'Q':  return GPIOQ_BASE;
  default: ASSERT(FALSE); return 0;
  }
}

UINT32
TimerBase (
  IN  UINTN Timer
  )
{
  switch (Timer) {
  case  0: return TIMER0_BASE;
  case  1: return TIMER1_BASE;
  case  2: return TIMER2_BASE;
  case  3: return TIMER3_BASE;
  case  4: return TIMER4_BASE;
  default: return 0;
  }
}

UINTN
InterruptVectorForTimer (
  IN  UINTN Timer
  )
{
  if (Timer > 4) {
    ASSERT(FALSE);
    return 0xFFFFFFFF;
  }

  return 36 + Timer;
}

UINT32
UartBase (
  IN  UINTN Uart
  )
{
  switch (Uart) {
  case 0:  return UART0_BASE;
  case 1:  return UART1_BASE;
  case 2:  return UART2_BASE;
  case 3:  return UART3_BASE;
  default: ASSERT(FALSE); return 0;
  }
}

