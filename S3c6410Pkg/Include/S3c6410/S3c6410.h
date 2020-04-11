/** @file

  Copyright (c) 2008 - 2009, Apple Inc. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef __S3C6410_H__
#define __S3C6410_H__

#include "S3c6410Gpio.h"
#include "S3c6410Interrupt.h"
#include "S3c6410Prcm.h"
#include "S3c6410Timer.h"
#include "S3c6410Uart.h"
#include "S3c6410Usb.h"
#include "S3c6410MMCHS.h"
#include "S3c6410I2c.h"
#include "S3c6410Gpmc.h"
#include "S3c6410Dma.h"
#include "S3c6410WatchDog.h"
#include "S3c6410ClkPower.h"
#include "S3c6410Dramc.h"

// Device Copy Function Pointer
#define GLOBAL_BLOCK_CNT    0x0C003FFC
#define COPY_MMC_TO_MEM     0x0C004008
#define BL1_SIG_RESERV_CNT  18

#define PERIPHERAL_PORT_REMAP_BASE     0x70000000

//CONTROL_PBIAS_LITE
#define CONTROL_PBIAS_LITE    0x48002520
#define PBIASLITEVMODE0       BIT0
#define PBIASLITEPWRDNZ0      BIT1
#define PBIASSPEEDCTRL0       BIT2
#define PBIASLITEVMODE1       BIT8
#define PBIASLITEWRDNZ1       BIT9

#endif // __S3C6410_H__
