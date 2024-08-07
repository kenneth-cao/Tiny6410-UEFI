/** @file

 Copyright (c) 2011, ARM Ltd. All rights reserved.<BR>
 This program and the accompanying materials
 are licensed and made available under the terms and conditions of the BSD License
 which accompanies this distribution.  The full text of the license may be found at
 http://opensource.org/licenses/bsd-license.php

 THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
 WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

 **/

#include <PiDxe.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DevicePathLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/MemoryAllocationLib.h>

#include <Guid/GlobalVariable.h>

#include "LcdGraphicsOutputDxe.h"


//
// Function Definitions
//

STATIC
EFI_STATUS
VideoCopyNoHorizontalOverlap (
  IN UINTN          BitsPerPixel,
  IN volatile VOID  *FrameBufferBase,
  IN UINT32         HorizontalResolution,
  IN UINTN          SourceX,
  IN UINTN          SourceY,
  IN UINTN          DestinationX,
  IN UINTN          DestinationY,
  IN UINTN          Width,
  IN UINTN          Height
  )
{
  EFI_STATUS    Status = EFI_SUCCESS;
  UINTN         SourceLine;
  UINTN         DestinationLine;
  UINTN         WidthInBytes;
  UINTN         LineCount;
  INTN          Step;
  VOID          *SourceAddr;
  VOID          *DestinationAddr;

  if( DestinationY <= SourceY ) {
    // scrolling up (or horizontally but without overlap)
    SourceLine       = SourceY;
    DestinationLine  = DestinationY;
    Step             = 1;
  } else {
    // scrolling down
    SourceLine       = SourceY + Height;
    DestinationLine  = DestinationY + Height;
    Step             = -1;
  }

  WidthInBytes = Width * 4;

  for( LineCount = 0; LineCount < Height; LineCount++ ) {
    // Update the start addresses of source & destination using 32bit pointer arithmetic
    SourceAddr      = (VOID *)((UINT32 *)FrameBufferBase + SourceLine      * HorizontalResolution + SourceX     );
    DestinationAddr = (VOID *)((UINT32 *)FrameBufferBase + DestinationLine * HorizontalResolution + DestinationX);

    // Copy the entire line Y from video ram to the temp buffer
    CopyMem( DestinationAddr, SourceAddr, WidthInBytes);

    // Update the line numbers
    SourceLine      += Step;
    DestinationLine += Step;
  }

  return Status;
}

STATIC
EFI_STATUS
VideoCopyHorizontalOverlap (
  IN UINTN          BitsPerPixel,
  IN volatile VOID  *FrameBufferBase,
  UINT32            HorizontalResolution,
  IN UINTN          SourceX,
  IN UINTN          SourceY,
  IN UINTN          DestinationX,
  IN UINTN          DestinationY,
  IN UINTN          Width,
  IN UINTN          Height
  )
{
  EFI_STATUS      Status = EFI_SUCCESS;

  UINT32 *PixelBuffer32bit;
  UINT32 *SourcePixel32bit;
  UINT32 *DestinationPixel32bit;

  UINT32          SourcePixelY;
  UINT32          DestinationPixelY;
  UINTN           SizeIn32bits;

  // Allocate a temporary buffer
  PixelBuffer32bit = (UINT32 *) AllocatePool((Height * Width) * 4);

  if (PixelBuffer32bit == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto EXIT;
  }

  // Access each pixel inside the source area of the Video Memory and copy it to the temp buffer

  SizeIn32bits = Width * 4;

  for (SourcePixelY = SourceY, DestinationPixel32bit = PixelBuffer32bit;
       SourcePixelY < SourceY + Height;
       SourcePixelY++, DestinationPixel32bit += Width)
  {
    // Calculate the source address:
    SourcePixel32bit = (UINT32 *)FrameBufferBase + SourcePixelY * HorizontalResolution + SourceX;

    // Copy the entire line Y from Video to the temp buffer
    CopyMem( (VOID *)DestinationPixel32bit, (CONST VOID *)SourcePixel32bit, SizeIn32bits);
  }

  // Copy from the temp buffer into the destination area of the Video Memory

  for (DestinationPixelY = DestinationY, SourcePixel32bit = PixelBuffer32bit;
       DestinationPixelY < DestinationY + Height;
       DestinationPixelY++, SourcePixel32bit += Width)
  {
    // Calculate the target address:
    DestinationPixel32bit = (UINT32 *)FrameBufferBase + (DestinationPixelY * HorizontalResolution + DestinationX);

    // Copy the entire line Y from the temp buffer to Video
    CopyMem( (VOID *)DestinationPixel32bit, (CONST VOID *)SourcePixel32bit, SizeIn32bits);
  }

  // Free the allocated memory
  FreePool((VOID *) PixelBuffer32bit);


EXIT:
  return Status;
}

STATIC
EFI_STATUS
BltVideoFill (
  IN EFI_GRAPHICS_OUTPUT_PROTOCOL        *This,
  IN OUT EFI_GRAPHICS_OUTPUT_BLT_PIXEL   *EfiSourcePixel,     OPTIONAL
  IN UINTN                               SourceX,
  IN UINTN                               SourceY,
  IN UINTN                               DestinationX,
  IN UINTN                               DestinationY,
  IN UINTN                               Width,
  IN UINTN                               Height,
  IN UINTN                               Delta           OPTIONAL   // Number of BYTES in a row of the BltBuffer
  )
{
  EFI_PIXEL_BITMASK*  PixelInformation;
  EFI_STATUS          Status;
  UINT32              HorizontalResolution;
  VOID                *FrameBufferBase;
  UINT32              *DestinationPixel32bit;
  UINT32              Pixel32bit;
  UINT32              DestinationPixelX;
  UINT32              DestinationLine;

  Status           = EFI_SUCCESS;
  PixelInformation = &This->Mode->Info->PixelInformation;
  FrameBufferBase = (UINTN *)((UINTN)(This->Mode->FrameBufferBase));
  HorizontalResolution = This->Mode->Info->HorizontalResolution;

  // Convert the EFI pixel at the start of the BltBuffer(0,0) into a video display pixel
  Pixel32bit = (UINT32) (
      ( (EfiSourcePixel->Red      <<  12) & PixelInformation->RedMask      )
    | ( (EfiSourcePixel->Green    <<  6 )  & PixelInformation->GreenMask   )
    | ( (EfiSourcePixel->Blue     <<  0 )  & PixelInformation->BlueMask    )
   );

  // Copy the SourcePixel into every pixel inside the target rectangle
  for (DestinationLine = DestinationY;
       DestinationLine < DestinationY + Height;
       DestinationLine++)
  {
    for (DestinationPixelX = DestinationX;
         DestinationPixelX < DestinationX + Width;
         DestinationPixelX++)
    {
      // Calculate the target address:
      DestinationPixel32bit =  (UINT32 *)FrameBufferBase + DestinationLine * HorizontalResolution  + DestinationPixelX;

      // Copy the pixel into the new target
      *DestinationPixel32bit = Pixel32bit;
    }
  }


  return Status;
}

STATIC
EFI_STATUS
BltVideoToBltBuffer (
  IN EFI_GRAPHICS_OUTPUT_PROTOCOL        *This,
  IN OUT EFI_GRAPHICS_OUTPUT_BLT_PIXEL   *BltBuffer,     OPTIONAL
  IN UINTN                               SourceX,
  IN UINTN                               SourceY,
  IN UINTN                               DestinationX,
  IN UINTN                               DestinationY,
  IN UINTN                               Width,
  IN UINTN                               Height,
  IN UINTN                               Delta           OPTIONAL   // Number of BYTES in a row of the BltBuffer
  )
{
  EFI_STATUS         Status;
  UINT32             HorizontalResolution;
  EFI_PIXEL_BITMASK  *PixelInformation;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL *EfiDestinationPixel;
  VOID               *FrameBufferBase;
  UINT32             *SourcePixel32bit;
  UINT32             Pixel32bit;
  UINT32             SourcePixelX;
  UINT32             SourceLine;
  UINT32             DestinationPixelX;
  UINT32             DestinationLine;
  UINT32             BltBufferHorizontalResolution;

  Status = EFI_SUCCESS;
  PixelInformation = &This->Mode->Info->PixelInformation;
  HorizontalResolution = This->Mode->Info->HorizontalResolution;
  FrameBufferBase = (UINTN *)((UINTN)(This->Mode->FrameBufferBase));

  if(( Delta != 0 ) && ( Delta != Width * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL))) {
    // Delta is not zero and it is different from the width.
    // Divide it by the size of a pixel to find out the buffer's horizontal resolution.
    BltBufferHorizontalResolution = (UINT32) (Delta / sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL));
  } else {
    BltBufferHorizontalResolution = Width;
  }

  // Access each pixel inside the Video Memory
  for (SourceLine = SourceY, DestinationLine = DestinationY;
       SourceLine < SourceY + Height;
       SourceLine++, DestinationLine++)
  {
    for (SourcePixelX = SourceX, DestinationPixelX = DestinationX;
         SourcePixelX < SourceX + Width;
         SourcePixelX++, DestinationPixelX++)
    {
      // Calculate the source and target addresses:
      SourcePixel32bit = (UINT32 *)FrameBufferBase + SourceLine * HorizontalResolution + SourcePixelX;
      EfiDestinationPixel = BltBuffer + DestinationLine * BltBufferHorizontalResolution + DestinationPixelX;

      // Snapshot the pixel from the video buffer once, to speed up the operation.
      // If we were dereferencing the pointer, as it is volatile, we would perform 3 memory read operations.
      Pixel32bit = *SourcePixel32bit;

      // Copy the pixel into the new target
      EfiDestinationPixel->Red      = (UINT8) ( (Pixel32bit & PixelInformation->RedMask     ) >>  12 );
      EfiDestinationPixel->Green    = (UINT8) ( (Pixel32bit & PixelInformation->GreenMask   ) >>  6  );
      EfiDestinationPixel->Blue     = (UINT8) ( (Pixel32bit & PixelInformation->BlueMask    ) >>  0  );
    }
  }

  return Status;
}

STATIC
EFI_STATUS
BltBufferToVideo (
  IN EFI_GRAPHICS_OUTPUT_PROTOCOL        *This,
  IN OUT EFI_GRAPHICS_OUTPUT_BLT_PIXEL   *BltBuffer,     OPTIONAL
  IN UINTN                               SourceX,
  IN UINTN                               SourceY,
  IN UINTN                               DestinationX,
  IN UINTN                               DestinationY,
  IN UINTN                               Width,
  IN UINTN                               Height,
  IN UINTN                               Delta           OPTIONAL   // Number of BYTES in a row of the BltBuffer
  )
{
  EFI_STATUS         Status;
  UINT32             HorizontalResolution;
  EFI_PIXEL_BITMASK  *PixelInformation;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL *EfiSourcePixel;
  VOID               *FrameBufferBase;
  UINT32             *DestinationPixel32bit;
  UINT32             SourcePixelX;
  UINT32             SourceLine;
  UINT32             DestinationPixelX;
  UINT32             DestinationLine;
  UINT32             BltBufferHorizontalResolution;

  Status = EFI_SUCCESS;
  PixelInformation = &This->Mode->Info->PixelInformation;
  HorizontalResolution = This->Mode->Info->HorizontalResolution;
  FrameBufferBase = (UINTN *)((UINTN)(This->Mode->FrameBufferBase));

  if(( Delta != 0 ) && ( Delta != Width * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL))) {
    // Delta is not zero and it is different from the width.
    // Divide it by the size of a pixel to find out the buffer's horizontal resolution.
    BltBufferHorizontalResolution = (UINT32) (Delta / sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL));
  } else {
    BltBufferHorizontalResolution = Width;
  }

  // Access each pixel inside the BltBuffer Memory
  for (SourceLine = SourceY, DestinationLine = DestinationY;
       SourceLine < SourceY + Height;
       SourceLine++, DestinationLine++) {

    for (SourcePixelX = SourceX, DestinationPixelX = DestinationX;
         SourcePixelX < SourceX + Width;
         SourcePixelX++, DestinationPixelX++)
    {
      // Calculate the source and target addresses:
      EfiSourcePixel  = BltBuffer + SourceLine * BltBufferHorizontalResolution + SourcePixelX;
      DestinationPixel32bit = (UINT32 *)FrameBufferBase + DestinationLine * HorizontalResolution + DestinationPixelX;

      // Copy the pixel into the new target
      // Only the most significant bits will be copied across:
      // To convert from 8 bits to 5 bits per pixel we throw away the 3 least significant bits
        *DestinationPixel32bit = (UINT32) (
              ( (EfiSourcePixel->Red      <<  12) & PixelInformation->RedMask      )
            | ( (EfiSourcePixel->Green    <<  6 )  & PixelInformation->GreenMask   )
            | ( (EfiSourcePixel->Blue     <<  0 )  & PixelInformation->BlueMask    )
            );
      }
    }

  return Status;
}

STATIC
EFI_STATUS
BltVideoToVideo (
  IN EFI_GRAPHICS_OUTPUT_PROTOCOL        *This,
  IN OUT EFI_GRAPHICS_OUTPUT_BLT_PIXEL   *BltBuffer,     OPTIONAL
  IN UINTN                               SourceX,
  IN UINTN                               SourceY,
  IN UINTN                               DestinationX,
  IN UINTN                               DestinationY,
  IN UINTN                               Width,
  IN UINTN                               Height,
  IN UINTN                               Delta           OPTIONAL   // Number of BYTES in a row of the BltBuffer
  )
{
  EFI_STATUS         Status;
  UINT32             HorizontalResolution;
  UINTN              BitsPerPixel;
  VOID               *FrameBufferBase;

  BitsPerPixel = 32;

  HorizontalResolution = This->Mode->Info->HorizontalResolution;
  FrameBufferBase = (UINTN *)((UINTN)(This->Mode->FrameBufferBase));

  //
  // BltVideo to BltVideo:
  //
  //  Source is the Video Memory,
  //  Destination is the Video Memory

  FrameBufferBase = (UINTN *)((UINTN)(This->Mode->FrameBufferBase));

  // The UEFI spec currently states:
  // "There is no limitation on the overlapping of the source and destination rectangles"
  // Therefore, we must be careful to avoid overwriting the source data
  if( SourceY == DestinationY ) {
    // Copying within the same height, e.g. horizontal shift
    if( SourceX == DestinationX ) {
      // Nothing to do
      Status = EFI_SUCCESS;
    } else if( ((SourceX>DestinationX)?(SourceX - DestinationX):(DestinationX - SourceX)) < Width ) {
      // There is overlap
      Status = VideoCopyHorizontalOverlap (BitsPerPixel, FrameBufferBase, HorizontalResolution, SourceX, SourceY, DestinationX, DestinationY, Width, Height );
    } else {
      // No overlap
      Status = VideoCopyNoHorizontalOverlap (BitsPerPixel, FrameBufferBase, HorizontalResolution, SourceX, SourceY, DestinationX, DestinationY, Width, Height );
    }
  } else {
    // Copying from different heights
    Status = VideoCopyNoHorizontalOverlap (BitsPerPixel, FrameBufferBase, HorizontalResolution, SourceX, SourceY, DestinationX, DestinationY, Width, Height );
  }

  return Status;
}

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
  )
{
  EFI_STATUS    Status;

  switch (BltOperation) {
  case EfiBltVideoFill:
    Status = BltVideoFill (This, BltBuffer, SourceX, SourceY, DestinationX, DestinationY, Width, Height, Delta);
    break;

  case EfiBltVideoToBltBuffer:
    Status = BltVideoToBltBuffer (This, BltBuffer, SourceX, SourceY, DestinationX, DestinationY, Width, Height, Delta);
    break;

  case EfiBltBufferToVideo:
    Status = BltBufferToVideo (This, BltBuffer, SourceX, SourceY, DestinationX, DestinationY, Width, Height, Delta);
    break;

  case EfiBltVideoToVideo:
    Status = BltVideoToVideo (This, BltBuffer, SourceX, SourceY, DestinationX, DestinationY, Width, Height, Delta);
    break;

  case EfiGraphicsOutputBltOperationMax:
  default:
    DEBUG((DEBUG_ERROR, "LcdGraphicsBlt: Invalid Operation\n"));
    Status = EFI_INVALID_PARAMETER;
    break;
}

  return Status;
}
