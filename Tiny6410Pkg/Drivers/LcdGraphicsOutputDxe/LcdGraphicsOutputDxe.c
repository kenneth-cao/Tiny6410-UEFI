/** @file

 Copyright (c) 2011-2014, ARM Ltd. All rights reserved.<BR>
 This program and the accompanying materials
 are licensed and made available under the terms and conditions of the BSD License
 which accompanies this distribution.  The full text of the license may be found at
 http://opensource.org/licenses/bsd-license.php

 THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
 WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "LcdGraphicsOutputDxe.h"


LCD_MODE LcdModes[] = {
  {
    0, 480, 272,
    6, 4, 45,
    2, 2, 3
  },
};

LCD_INSTANCE mLcdTemplate = {
  LCD_INSTANCE_SIGNATURE,
  NULL, // Handle
  { // ModeInfo
    0, // Version
    0, // HorizontalResolution
    0, // VerticalResolution
    PixelBltOnly, // PixelFormat
    {
      0x3F000, //RedMask;
      0xFC0, //GreenMask;
      0x3F, //BlueMask;
      0x0//ReservedMask
    }, // PixelInformation
    0, // PixelsPerScanLine
  },
  { // Mode
    1, // MaxMode;
    0, // Mode;
    NULL, // Info;
    0, // SizeOfInfo;
    0, // FrameBufferBase;
    0 // FrameBufferSize;
  },
  { // Gop
    LcdGraphicsQueryMode,  // QueryMode
    LcdGraphicsSetMode,    // SetMode
    LcdGraphicsBlt,        // Blt
    NULL                     // *Mode
  },
  { // DevicePath
    {
      {
        HARDWARE_DEVICE_PATH, HW_VENDOR_DP,
        { (UINT8) (sizeof(VENDOR_DEVICE_PATH)), (UINT8) ((sizeof(VENDOR_DEVICE_PATH)) >> 8) },
      },
      // Hardware Device Path for Lcd
      EFI_CALLER_ID_GUID // Use the driver's GUID
    },
    {
      END_DEVICE_PATH_TYPE,
      END_ENTIRE_DEVICE_PATH_SUBTYPE,
      { sizeof(EFI_DEVICE_PATH_PROTOCOL), 0}
    }
  }
};

EFI_STATUS
LcdInstanceContructor (
  OUT LCD_INSTANCE** NewInstance
  )
{
  LCD_INSTANCE* Instance;

  Instance = AllocateCopyPool (sizeof(LCD_INSTANCE), &mLcdTemplate);
  if (Instance == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  Instance->Gop.Mode          = &Instance->Mode;
  Instance->Mode.Info         = &Instance->ModeInfo;

  *NewInstance = Instance;
  return EFI_SUCCESS;
}

EFI_STATUS
LcdPlatformGetVram (
  OUT EFI_PHYSICAL_ADDRESS*  VramBaseAddress,
  OUT UINTN*                 VramSize
  )
{
  EFI_STATUS             Status;
  EFI_CPU_ARCH_PROTOCOL  *Cpu;
  UINTN                  MaxSize;

  MaxSize = LcdModes[0].HorizontalResolution * LcdModes[0].VerticalResolution * 4;

  // Allocate VRAM from DRAM
  Status = gBS->AllocatePages (AllocateAnyPages, EfiBootServicesData, EFI_SIZE_TO_PAGES((MaxSize)), VramBaseAddress);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  // Ensure the Cpu architectural protocol is already installed
  Status = gBS->LocateProtocol (&gEfiCpuArchProtocolGuid, NULL, (VOID **)&Cpu);
  ASSERT_EFI_ERROR(Status);

  // Mark the VRAM as un-cacheable. The VRAM is inside the DRAM, which is cacheable.
  Status = Cpu->SetMemoryAttributes (Cpu, *VramBaseAddress, MaxSize, EFI_MEMORY_UC);
  if (EFI_ERROR(Status)) {
    gBS->FreePool (VramBaseAddress);
    return Status;
  }

  *VramSize = MaxSize;

  return EFI_SUCCESS;
}

EFI_STATUS
HwInitializeDisplay (
  UINT32 VramBaseAddress
  )
{

  // GPIO Init
  MmioWrite32(GPIOI_BASE + GPIO_CON, 0xaaaaaaaa);
  MmioWrite32(GPIOJ_BASE + GPIO_CON, 0xaaaaaa);

  // MOFPCON: SEL_BYPASS[3] value @ 0x7410800C must be set as ‘0’(normal mode) instead of ‘1’(by-pass mode)
  MmioAnd32(MOFPCON, ~(1 << 3));

  // SPCON: LCD_SEL[1:0] value @ 0x7F0081A0 must be set as ‘00’ to use Host I/F Style or as ‘01’ to use RGB I/F Style
  MmioAndThenOr32(SPCON, 0xfffffffc, 1);

  // VIDCON0: configure Video output format and display enable/disable
  MmioAndThenOr32(VIDCON0, ~(0xff << 6 | 3 << 2), (N43_CLKVAL << 6) | 0x13);

  // VIDCON1: RGB I/F control signal.
  MmioOr32(VIDCON1, 1 << 7); // Rise edge

  // VIDTCONx: configure Video output Timing and determine the size of display
  MmioWrite32 (VIDTCON0,
               ( (LcdModes[0].VSync - 1)
               | ((LcdModes[0].VFrontPorch - 1) << 8)
               | ((LcdModes[0].VBackPorch - 1) << 16))
               );
  MmioWrite32 (VIDTCON1,
               ( (LcdModes[0].HSync - 1)
               | ((LcdModes[0].HFrontPorch - 1) << 8)
               | ((LcdModes[0].HBackPorch - 1) << 16))
               );
  MmioWrite32 (VIDTCON2, ((LcdModes[0].VerticalResolution - 1) << 11) | (LcdModes[0].HorizontalResolution - 1));
  
  // WINCONx: each window format setting
   MmioAndThenOr32(WINCON0, ~(0xf << 2), (N43_BPP << 2) | 1);

  // VIDOSDxA, VIDOSDxB: Window position setting
  MmioWrite32(VIDOSD0A, 0);
  MmioWrite32(VIDOSD0B, (LcdModes[0].HorizontalResolution << 11) | LcdModes[0].VerticalResolution);
  MmioWrite32(VIDOSD0C, LcdModes[0].HorizontalResolution * LcdModes[0].VerticalResolution * 4 / 2);

  MmioWrite32(VIDW00ADD0B0, VramBaseAddress);
  MmioWrite32(VIDW00ADD1B0, (VramBaseAddress + LcdModes[0].HorizontalResolution * LcdModes[0].VerticalResolution * 4) & 0xffffff);
  MmioWrite32(VIDW00ADD2, LcdModes[0].HorizontalResolution * 4);

  return EFI_SUCCESS;

}

EFI_STATUS
InitializeDisplay (
  IN LCD_INSTANCE* Instance
  )
{
  EFI_STATUS           Status;
  UINTN                VramSize;
  EFI_PHYSICAL_ADDRESS VramBaseAddress;

  Status = LcdPlatformGetVram (&VramBaseAddress, &VramSize);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Instance->Mode.FrameBufferBase = VramBaseAddress;
  Instance->Mode.FrameBufferSize = VramSize;

  Status = HwInitializeDisplay((UINT32)VramBaseAddress);
  if (EFI_ERROR (Status)) {
    gBS->FreePool ((void*)VramBaseAddress);
    return Status;
  }

  Instance->ModeInfo.HorizontalResolution = LcdModes[0].HorizontalResolution;
  Instance->ModeInfo.VerticalResolution = LcdModes[0].VerticalResolution;

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
LcdGraphicsQueryMode (
  IN EFI_GRAPHICS_OUTPUT_PROTOCOL            *This,
  IN UINT32                                  ModeNumber,
  OUT UINTN                                  *SizeOfInfo,
  OUT EFI_GRAPHICS_OUTPUT_MODE_INFORMATION   **Info
  )
{
  LCD_INSTANCE  *Instance;

  Instance = LCD_INSTANCE_FROM_GOP_THIS(This);

  // Error checking
  if ( (This == NULL) || (Info == NULL) || (SizeOfInfo == NULL) || (ModeNumber >= This->Mode->MaxMode) ) {
    DEBUG((DEBUG_ERROR, "LcdGraphicsQueryMode: ERROR - For mode number %d : Invalid Parameter.\n", ModeNumber ));
    return EFI_INVALID_PARAMETER;
  }

  *Info = AllocateCopyPool(sizeof (EFI_GRAPHICS_OUTPUT_MODE_INFORMATION), &Instance->ModeInfo);
  if (*Info == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  *SizeOfInfo = sizeof (EFI_GRAPHICS_OUTPUT_MODE_INFORMATION);

  (*Info)->Version = 0;
  (*Info)->HorizontalResolution = LcdModes[ModeNumber].HorizontalResolution;
  (*Info)->VerticalResolution = LcdModes[ModeNumber].VerticalResolution;
  (*Info)->PixelFormat = PixelBltOnly;

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
LcdGraphicsSetMode (
  IN EFI_GRAPHICS_OUTPUT_PROTOCOL   *This,
  IN UINT32                         ModeNumber
  )
{

  // Check if this mode is supported
  if( ModeNumber >= This->Mode->MaxMode ) {
    DEBUG((DEBUG_ERROR, "LcdGraphicsSetMode: ERROR - Unsupported mode number %d .\n", ModeNumber ));
    return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
LcdGraphicsOutputDxeInitialize (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  LCD_INSTANCE* Instance;

  Status = LcdInstanceContructor (&Instance);
  if (EFI_ERROR(Status)) {
    goto EXIT;
  }

  Status = InitializeDisplay (Instance);
  if (EFI_ERROR(Status)) {
    gBS->FreePool (Instance);
    goto EXIT;
  }

  // Install the Graphics Output Protocol and the Device Path
  Status = gBS->InstallMultipleProtocolInterfaces(
             &Instance->Handle,
             &gEfiGraphicsOutputProtocolGuid, &Instance->Gop,
             &gEfiDevicePathProtocolGuid,     &Instance->DevicePath,
             NULL
             );

  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "GraphicsOutputDxeInitialize: Can not install the protocol. Exit Status=%r\n", Status));
    goto EXIT;
  }

  // Register for an ExitBootServicesEvent
  // When ExitBootServices starts, this function here will make sure that the graphics driver will shut down properly,
  // i.e. it will free up all allocated memory and perform any necessary hardware re-configuration.
  /*Status = gBS->CreateEvent (
               EVT_SIGNAL_EXIT_BOOT_SERVICES,
               TPL_NOTIFY,
               LcdGraphicsExitBootServicesEvent, NULL,
               &Instance->ExitBootServicesEvent
               );

  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "GraphicsOutputDxeInitialize: Can not install the ExitBootServicesEvent handler. Exit Status=%r\n", Status));
    goto EXIT_ERROR_UNINSTALL_PROTOCOL;
  }*/

  // To get here, everything must be fine, so just exit
  // goto EXIT;

//EXIT_ERROR_UNINSTALL_PROTOCOL:
  /* The following function could return an error message,
   * however, to get here something must have gone wrong already,
   * so preserve the original error, i.e. don't change
   * the Status variable, even it fails to uninstall the protocol.
   */
  /*  gBS->UninstallMultipleProtocolInterfaces (
        Instance->Handle,
        &gEfiGraphicsOutputProtocolGuid, &Instance->Gop, // Uninstall Graphics Output protocol
        &gEfiDevicePathProtocolGuid,     &Instance->DevicePath,     // Uninstall device path
        NULL
        );*/

EXIT:
  return Status;

}
