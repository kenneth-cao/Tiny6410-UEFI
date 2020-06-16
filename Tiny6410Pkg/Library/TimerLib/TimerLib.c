/** @file

  Copyright (c) 2008 - 2009, Apple Inc. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <Uefi.h>

#include <Library/BaseLib.h>
#include <Library/TimerLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/IoLib.h>
#include <Library/S3c6410Lib.h>

#include <S3c6410.h>

RETURN_STATUS
EFIAPI
TimerConstructor (
  VOID
  )
{
  UINTN  TconBits      = 0;
  UINTN  Timer         = PcdGet32(PcdTiny6410FreeTimer);
  UINT32 TCNTB         = TimerBase(Timer) + TIMER_CNTB;

  if (Timer > 0) {
    TconBits = 4 * Timer + 4;
  }

  if ((MmioRead32(TIMER_CON) & (1 << TconBits)) == 0) {
    MmioAnd32(TIMER_CFG0, 0xFFFF0000);
    MmioOr32(TIMER_CFG0, (TCFG0_PRESCALER << 8) | TCFG0_PRESCALER);
    MmioAnd32(TIMER_CFG1, ~(0xF << 4 * Timer));
    // Disable interrupts
    MmioAnd32(TIMER_INT_CSTAT, ~(1 << Timer));
    MmioWrite32(TCNTB, (UINT32)-1);
    // Enable auto-reload, set manual update
    if (Timer < 4) {
      MmioOr32(TIMER_CON, 10 << TconBits);
    } else {
      MmioOr32(TIMER_CON, 6 << TconBits);
    }
    // Clear manual update and Start Timer
    MmioAnd32(TIMER_CON, ~(3 << TconBits));
    MmioOr32(TIMER_CON, 1 << TconBits);
  }
  return EFI_SUCCESS;
}

UINTN
EFIAPI
MicroSecondDelay (
  IN  UINTN MicroSeconds
  )
{
  UINT64  NanoSeconds;

  NanoSeconds = MultU64x32(MicroSeconds, 1000);

  while (NanoSeconds > (UINTN)-1) {
    NanoSecondDelay((UINTN)-1);
    NanoSeconds -= (UINTN)-1;
  }

  NanoSecondDelay(NanoSeconds);

  return MicroSeconds;
}

UINTN
EFIAPI
NanoSecondDelay (
  IN  UINTN NanoSeconds
  )
{
  UINT32  Delay;
  UINT32  StartTime;
  UINT32  CurrentTime;
  UINT32  ElapsedTime;
  UINT32  TimerCountRegister;

  Delay = (NanoSeconds / PcdGet32(PcdEmbeddedPerformanceCounterPeriodInNanoseconds)) + 1;

  TimerCountRegister = TimerBase(PcdGet32(PcdTiny6410FreeTimer)) + TIMER_CONTO;

  StartTime = MmioRead32(TimerCountRegister);

  do
  {
    CurrentTime = MmioRead32(TimerCountRegister);
    ElapsedTime = StartTime - CurrentTime;
  } while (ElapsedTime < Delay);

  NanoSeconds = ElapsedTime * PcdGet32(PcdEmbeddedPerformanceCounterPeriodInNanoseconds);

  return NanoSeconds;
}

UINT64
EFIAPI
GetPerformanceCounter (
  VOID
  )
{
  return (UINT64)MmioRead32 (TimerBase(PcdGet32(PcdTiny6410FreeTimer)) + TIMER_CONTO);
}

UINT64
EFIAPI
GetPerformanceCounterProperties (
  OUT UINT64  *StartValue,  OPTIONAL
  OUT UINT64  *EndValue     OPTIONAL
  )
{
  if (StartValue != NULL) {
    // Timer starts with the reload value
    *StartValue = (UINT64)MmioRead32(TimerBase(PcdGet32(PcdTiny6410FreeTimer)) + TIMER_CNTB);
  }

  if (EndValue != NULL) {
    // Timer counts down to 0
    *EndValue = 0;
  }

  return PcdGet64(PcdEmbeddedPerformanceCounterFrequencyInHz);
}
