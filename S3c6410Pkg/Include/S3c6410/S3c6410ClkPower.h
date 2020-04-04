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


// Clock and power management
#define CLK_POWER_BASE 0x7e00f000

/* Clock & Power Controller for mDirac3*/
#define APLL_LOCK_OFFSET	0x00
#define MPLL_LOCK_OFFSET	0x04
#define EPLL_LOCK_OFFSET	0x08
#define APLL_CON_OFFSET		0x0C
#define MPLL_CON_OFFSET		0x10
#define EPLL_CON0_OFFSET	0x14
#define EPLL_CON1_OFFSET	0x18
#define CLK_SRC_OFFSET		0x1C
#define CLK_DIV0_OFFSET		0x20
#define CLK_DIV1_OFFSET		0x24
#define CLK_DIV2_OFFSET		0x28
#define CLK_OUT_OFFSET		0x2C
#define HCLK_GATE_OFFSET	0x30
#define PCLK_GATE_OFFSET	0x34
#define SCLK_GATE_OFFSET	0x38
#define AHB_CON0_OFFSET		0x100
#define AHB_CON1_OFFSET		0x104
#define AHB_CON2_OFFSET		0x108
#define SELECT_DMA_OFFSET	0x110
#define SW_RST_OFFSET		0x114
#define SYS_ID_OFFSET		0x118
#define MEM_SYS_CFG_OFFSET	0x120
#define QOS_OVERRIDE0_OFFSET	0x124
#define QOS_OVERRIDE1_OFFSET	0x128
#define MEM_CFG_STAT_OFFSET	0x12C
#define PWR_CFG_OFFSET		0x804
#define EINT_MASK_OFFSET	0x808
#define NOR_CFG_OFFSET		0x810
#define STOP_CFG_OFFSET		0x814
#define SLEEP_CFG_OFFSET	0x818
#define OSC_FREQ_OFFSET		0x820
#define OSC_STABLE_OFFSET	0x824
#define PWR_STABLE_OFFSET	0x828
#define FPC_STABLE_OFFSET	0x82C
#define MTC_STABLE_OFFSET	0x830
#define OTHERS_OFFSET		0x900
#define RST_STAT_OFFSET		0x904
#define WAKEUP_STAT_OFFSET	0x908
#define BLK_PWR_STAT_OFFSET	0x90C
#define INF_REG0_OFFSET		0xA00
#define INF_REG1_OFFSET		0xA04
#define INF_REG2_OFFSET		0xA08
#define INF_REG3_OFFSET		0xA0C
#define INF_REG4_OFFSET		0xA10
#define INF_REG5_OFFSET		0xA14
#define INF_REG6_OFFSET		0xA18
#define INF_REG7_OFFSET		0xA1C

#define OSC_CNT_VAL_OFFSET	0x824
#define PWR_CNT_VAL_OFFSET	0x828
#define FPC_CNT_VAL_OFFSET	0x82C
#define MTC_CNT_VAL_OFFSET	0x830

#endif // __S3C6410CLKPOWER_H__