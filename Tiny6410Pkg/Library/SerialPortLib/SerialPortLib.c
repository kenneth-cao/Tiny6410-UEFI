/** @file
  Serial I/O Port library functions with no library constructor/destructor


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
#include <Library/SerialPortLib.h>
#include <Library/PcdLib.h>
#include <Library/IoLib.h>
#include <Library/S3c6410Lib.h>
#include <S3c6410.h>

/*

  Programmed hardware of Serial port.

  @return    Always return EFI_UNSUPPORTED.

**/
RETURN_STATUS
EFIAPI
SerialPortInitialize (
  VOID
  )
{
  UINT32 Register = CLK_POWER_BASE + CLK_DIV2;
  // CLKUART(=66.5Mhz) = CLKUART_input(532/2=266Mhz) / (UART_RATIO(3)+1)
  MmioWrite32(Register, (MmioRead32(Register) & ~(7 << 16)) | (3 << 16));
  MmioOr32(Register, 1 << 13);

  // Set GPIO to enable UART
  MmioWrite32(GPIOA_BASE + GPIO_CON, 0x22222222);
  Register = GPIOB_BASE + GPIO_CON;
  MmioWrite32(Register, (MmioRead32(Register) & 0xFFFF0000) | 0x2222);

  // Port Init
  Register = UartBase(PcdGet32(PcdTiny6410ConsoleUart));
  MmioWrite32(Register + UART_LCON, 0x3);
  MmioWrite32(Register + UART_FCON, 0x0);
  MmioWrite32(Register + UART_MCON, 0x0);
  MmioWrite32(Register + UART_CON, 0x245);
  MmioWrite32(Register + UART_BRDIV, 0x23);
  MmioWrite32(Register + UART_DIVSLOT, 0x0080);

  return RETURN_SUCCESS;
}

/**
  Write data to serial device.

  @param  Buffer           Point of data buffer which need to be writed.
  @param  NumberOfBytes    Number of output bytes which are cached in Buffer.

  @retval 0                Write data failed.
  @retval !0               Actual number of bytes writed to serial device.

**/
UINTN
EFIAPI
SerialPortWrite (
  IN UINT8     *Buffer,
  IN UINTN     NumberOfBytes
)
{
  UINT32  UTRSTAT = UartBase(PcdGet32(PcdTiny6410ConsoleUart)) + UART_TRSTAT;
  UINT32  UTXH = UartBase(PcdGet32(PcdTiny6410ConsoleUart)) + UART_TXH;
  UINTN   Count;

  for (Count = 0; Count < NumberOfBytes; Count++, Buffer++) {
    while (!(MmioRead8(UTRSTAT) & BIT2));
    MmioWrite8(UTXH, *Buffer);
  }

  return NumberOfBytes;
}


/**
  Read data from serial device and save the datas in buffer.

  @param  Buffer           Point of data buffer which need to be writed.
  @param  NumberOfBytes    Number of output bytes which are cached in Buffer.

  @retval 0                Read data failed.
  @retval !0               Aactual number of bytes read from serial device.

**/
UINTN
EFIAPI
SerialPortRead (
  OUT UINT8     *Buffer,
  IN  UINTN     NumberOfBytes
)
{
  UINT32  UTRSTAT = UartBase(PcdGet32(PcdTiny6410ConsoleUart)) + UART_TRSTAT;
  UINT32  URXH = UartBase(PcdGet32(PcdTiny6410ConsoleUart)) + UART_RXH;
  UINTN   Count;

  for (Count = 0; Count < NumberOfBytes; Count++, Buffer++) {
    while (!(MmioRead8(UTRSTAT) & BIT0));
    *Buffer = MmioRead8(URXH);
  }

  return NumberOfBytes;
}


/**
  Check to see if any data is avaiable to be read from the debug device.

  @retval EFI_SUCCESS       At least one byte of data is avaiable to be read
  @retval EFI_NOT_READY     No data is avaiable to be read
  @retval EFI_DEVICE_ERROR  The serial device is not functioning properly

**/
BOOLEAN
EFIAPI
SerialPortPoll (
  VOID
  )
{
  UINT32  UTRSTAT = UartBase(PcdGet32(PcdTiny6410ConsoleUart)) + UART_TRSTAT;

  if (MmioRead8(UTRSTAT) & BIT0) {
    return TRUE;
  } else {
    return FALSE;
  }
}

