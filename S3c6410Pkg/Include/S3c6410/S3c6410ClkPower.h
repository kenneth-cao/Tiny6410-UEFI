/** @file

  Copyright (c) 2008 - 2009, Apple Inc. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef __S3C6410CLKPOWER_H__
#define __S3C6410CLKPOWER_H__

#define CLK_POWER_BASE 0x7e00f000

/* Clock & Power Controller for mDirac3*/
#define APLL_LOCK         0x00
#define MPLL_LOCK         0x04
#define EPLL_LOCK         0x08
#define APLL_CON          0x0C
#define MPLL_CON          0x10
#define EPLL_CON0         0x14
#define EPLL_CON1         0x18
#define CLK_SRC           0x1C
#define CLK_DIV0          0x20
#define CLK_DIV1          0x24
#define CLK_DIV2          0x28
#define CLK_OUT           0x2C
#define HCLK_GATE         0x30
#define PCLK_GATE         0x34
#define SCLK_GATE         0x38
#define AHB_CON0          0x100
#define AHB_CON1          0x104
#define AHB_CON2          0x108
#define SELECT_DMA        0x110
#define SW_RST            0x114
#define SYS_ID            0x118
#define MEM_SYS_CFG       0x120
#define QOS_OVERRIDE0     0x124
#define QOS_OVERRIDE1     0x128
#define MEM_CFG_STAT      0x12C
#define PWR_CFG           0x804
#define EINT_MASK         0x808
#define NOR_CFG           0x810
#define STOP_CFG          0x814
#define SLEEP_CFG         0x818
#define OSC_FREQ          0x820
#define OSC_STABLE        0x824
#define PWR_STABLE        0x828
#define FPC_STABLE        0x82C
#define MTC_STABLE        0x830
#define OTHERS            0x900
#define RST_STAT          0x904
#define WAKEUP_STAT       0x908
#define BLK_PWR_STAT      0x90C
#define INF_REG0          0xA00
#define INF_REG1          0xA04
#define INF_REG2          0xA08
#define INF_REG3          0xA0C
#define INF_REG4          0xA10
#define INF_REG5          0xA14
#define INF_REG6          0xA18
#define INF_REG7          0xA1C

#define OSC_CNT_VAL       0x824
#define PWR_CNT_VAL       0x828
#define FPC_CNT_VAL       0x82C
#define MTC_CNT_VAL       0x830

#endif // __S3C6410CLKPOWER_H__