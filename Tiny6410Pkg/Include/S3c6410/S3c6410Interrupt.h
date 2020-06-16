/** @file

  Copyright (c) 2008 - 2009, Apple Inc. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef __S3C6410INTERRUPT_H__
#define __S3C6410INTERRUPT_H__

#define VIC0_BASE           0x71200000
#define VIC1_BASE           0x71300000

#define VICxIRQSTATUS       0x000
#define VICxFIQSTATUS       0x004
#define VICxRAWINTR         0x008
#define VICxINTSELECT       0x00C
#define VICxINTENABLE       0x010
#define VICxINTENCLEAR      0x014
#define VICxSOFTINT         0x018
#define VICxSOFTINTCLEAR    0x01C
#define VICxPROTECTION      0x020
#define VICxSWPRIORITYMASK  0x024
#define VICxPRIORITYDAISY   0x028
#define VICxADDRESS         0xF00

#define VICxVECTADDR(x)     (0x100 + 4 * (x))
#define VICxVECPRIORITY(x)  (0x200 + 4 * (x))

#define MAX_VECTOR          63

#endif // __S3C6410INTERRUPT_H__

