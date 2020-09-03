/** @file

 Copyright (c) 2011, ARM Ltd. All rights reserved.<BR>
 This program and the accompanying materials
 are licensed and made available under the terms and conditions of the BSD License
 which accompanies this distribution.  The full text of the license may be found at
 http://opensource.org/licenses/bsd-license.php

 THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
 WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef __S3C6410_GRAPHICS__
#define __S3C6410_GRAPHICS__

#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/IoLib.h>

#include <Protocol/DevicePathToText.h>
#include <Protocol/Cpu.h>

#include <Guid/GlobalVariable.h>

#include <S3c6410.h>


typedef struct {
  VENDOR_DEVICE_PATH            Guid;
  EFI_DEVICE_PATH_PROTOCOL      End;
} LCD_GRAPHICS_DEVICE_PATH;

typedef struct {
  UINTN                                 Signature;
  EFI_HANDLE                            Handle;
  EFI_GRAPHICS_OUTPUT_MODE_INFORMATION  ModeInfo;
  EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE     Mode;
  EFI_GRAPHICS_OUTPUT_PROTOCOL          Gop;
  LCD_GRAPHICS_DEVICE_PATH              DevicePath;
//  EFI_EVENT                             ExitBootServicesEvent;
} LCD_INSTANCE;

#define LCD_INSTANCE_SIGNATURE  SIGNATURE_32('l', 'c', 'd', '0')
#define LCD_INSTANCE_FROM_GOP_THIS(a)     CR (a, LCD_INSTANCE, Gop, LCD_INSTANCE_SIGNATURE)

typedef struct {
  UINTN             Mode;
  UINTN             HorizontalResolution;
  UINTN             VerticalResolution;

  UINT32            HSync;
  UINT32            HFrontPorch;
  UINT32            HBackPorch;

  UINT32            VSync;
  UINT32            VFrontPorch;
  UINT32            VBackPorch;
} LCD_MODE;

EFI_STATUS
InitializeDisplay (
  IN LCD_INSTANCE* Instance
);

EFI_STATUS
EFIAPI
LcdGraphicsQueryMode (
  IN  EFI_GRAPHICS_OUTPUT_PROTOCOL          *This,
  IN  UINT32                                ModeNumber,
  OUT UINTN                                 *SizeOfInfo,
  OUT EFI_GRAPHICS_OUTPUT_MODE_INFORMATION  **Info
);

EFI_STATUS
EFIAPI
LcdGraphicsSetMode (
  IN EFI_GRAPHICS_OUTPUT_PROTOCOL  *This,
  IN UINT32                        ModeNumber
);

EFI_STATUS
EFIAPI
LcdGraphicsBlt (
  IN EFI_GRAPHICS_OUTPUT_PROTOCOL        *This,
  IN OUT EFI_GRAPHICS_OUTPUT_BLT_PIXEL   *BltBuffer,     OPTIONAL
  IN EFI_GRAPHICS_OUTPUT_BLT_OPERATION   BltOperation,
  IN UINTN                               SourceX,
  IN UINTN                               SourceY,
  IN UINTN                               DestinationX,
  IN UINTN                               DestinationY,
  IN UINTN                               Width,
  IN UINTN                               Height,
  IN UINTN                               Delta           OPTIONAL   // Number of BYTES in a row of the BltBuffer
);

// HW registers
#define VIDCON0       0x77100000
#define VIDCON1       0x77100004

#define VIDTCON0      0x77100010
#define VIDTCON1      0x77100014
#define VIDTCON2      0x77100018

#define WINCON0       0x77100020

#define VIDOSD0A      0x77100040
#define VIDOSD0B      0x77100044
#define VIDOSD0C      0x77100048

#define VIDW00ADD0B0  0x771000A0
#define VIDW00ADD1B0  0x771000A4

#define VIDW00ADD2    0x77100100

#define MOFPCON       0x7410800C
#define SPCON         0x7F0081A0

// NEC LCD-N43i-1043
#define N43_CLKVAL  11
#define N43_BPP     8

#endif
