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

#include <Library/IoLib.h>
#include <Library/S3c6410Lib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Protocol/Gpio.h>

#include <S3c6410.h>

EFI_STATUS
Get (
  IN  EMBEDDED_GPIO     *This,
  IN  EMBEDDED_GPIO_PIN Gpio,
  OUT UINTN               *Value
  )
{
  UINTN   Port;
  UINTN   Pin;
  UINT32  DataInRegister;

  if (Value == NULL)
  {
    return EFI_UNSUPPORTED;
  }

  Port    = GPIO_PORT(Gpio);
  Pin     = GPIO_PIN(Gpio);

  if (IsGioHKL(Port)) {
    DataInRegister = GpioBase((UINT8)Port) + GPIO_DAT;
  } else {
    DataInRegister = GpioBase((UINT8)Port) + GPIOHKL_DAT;
  }

  if (MmioRead32 (DataInRegister) & GPIO_DATAIN_MASK(Pin)) {
    *Value = 1;
  } else {
    *Value = 0;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
Set (
  IN  EMBEDDED_GPIO       *This,
  IN  EMBEDDED_GPIO_PIN   Gpio,
  IN  EMBEDDED_GPIO_MODE  Mode
  )
{
  UINTN   Port;
  UINTN   Pin;
  UINT32  GpCon;

  if (Mode > GPIO_MODE_SPECIAL_FUNCTION_6) {
    return EFI_UNSUPPORTED;
  }

  Port    = GPIO_PORT(Gpio);
  Pin     = GPIO_PIN(Gpio);
  GpCon   = GpioBase((UINT8)Port) + GPIO_CON;

  if (Con2Bits(Port)) {
    MmioAndThenOr32(GpCon, ~(0x3 << Pin * 2), Mode << Pin * 2);
  } else {
    if (Pin > 7 && IsGioHKL(Port)) {
      Pin -= 8;
      GpCon += 0x4;
    }
    MmioAndThenOr32(GpCon, ~(0xF << Pin * 4), Mode << Pin * 4);
  }

  return EFI_SUCCESS;
}

EFI_STATUS
GetMode (
  IN  EMBEDDED_GPIO       *This,
  IN  EMBEDDED_GPIO_PIN   Gpio,
  OUT EMBEDDED_GPIO_MODE  *Mode
  )
{
  return EFI_UNSUPPORTED;
}

EFI_STATUS
SetPull (
  IN  EMBEDDED_GPIO       *This,
  IN  EMBEDDED_GPIO_PIN   Gpio,
  IN  EMBEDDED_GPIO_PULL  Direction
  )
{
  UINTN   Port;
  UINTN   Pin;
  UINT32  GpPud;

  if (Direction > GPIO_PULL_UP) {
    return EFI_UNSUPPORTED;
  }

  Port    = GPIO_PORT(Gpio);
  Pin     = GPIO_PIN(Gpio);

  if (IsGioHKL(Port)) {
    GpPud = GpioBase((UINT8)Port) + GPIO_PUD;
  } else {
    GpPud = GpioBase((UINT8)Port) + GPIOHKL_PUD;
  }

  MmioAndThenOr32(GpPud, ~(0x3 << Pin * 2), Direction << Pin * 2);

  return EFI_SUCCESS;
}

EMBEDDED_GPIO Gpio = {
  Get,
  Set,
  GetMode,
  SetPull
};

EFI_STATUS
GpioInitialize (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS  Status;

  Status = gBS->InstallMultipleProtocolInterfaces(&ImageHandle, &gEmbeddedGpioProtocolGuid, &Gpio, NULL);
  return Status;
}
