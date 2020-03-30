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
#include "S3c6410PadConfiguration.h"
#include "S3c6410Gpmc.h"
#include "S3c6410Dma.h"

//CONTROL_PBIAS_LITE
#define CONTROL_PBIAS_LITE    0x48002520
#define PBIASLITEVMODE0       BIT0
#define PBIASLITEPWRDNZ0      BIT1
#define PBIASSPEEDCTRL0       BIT2
#define PBIASLITEVMODE1       BIT8
#define PBIASLITEWRDNZ1       BIT9

#define REG(x) (*(volatile UINT32*)(x))

// Device Copy Function Pointer
#define GLOBAL_BLOCK_CNT    0x0C003FFC
#define COPY_MMC_TO_MEM     0x0C004008
#define BL1_SIG_RESERV_CNT  18

#define PERIPHERAL_PORT_REMAP_BASE     0x70000000

// Watchdog timer
#define WTCON 0x7E004000

// Interrupt
#define VIC0_BASE 0x71200000
#define VIC1_BASE 0x71300000

#define INTENCLEAR_OFFSET 0x14

#define VIC0INTENCLEAR 0x71200014
#define VIC1INTENCLEAR 0x71300014

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


//　GPIO
#define GPKCON0 0x7f008800
#define GPKCON1 0x7F008804
#define GPKDAT  0x7F008808
#define GPKPUD  0x7F00880C

// SDMMC
#define CONTROL4_0 0x7C20008C

// Bus Matrix
#define ELFIN_MEM_SYS_CFG	0x7e00f120

// SDRAM Controller
#define ELFIN_DMC0_BASE		0x7e000000
#define ELFIN_DMC1_BASE		0x7e001000

#define INDEX_DMC_MEMC_STATUS   (0x00)
#define INDEX_DMC_MEMC_CMD      (0x04)
#define INDEX_DMC_DIRECT_CMD    (0x08)
#define INDEX_DMC_MEMORY_CFG    (0x0C)
#define INDEX_DMC_REFRESH_PRD   (0x10)
#define INDEX_DMC_CAS_LATENCY   (0x14)
#define INDEX_DMC_T_DQSS        (0x18)
#define INDEX_DMC_T_MRD         (0x1C)
#define INDEX_DMC_T_RAS         (0x20)
#define INDEX_DMC_T_RC          (0x24)
#define INDEX_DMC_T_RCD         (0x28)
#define INDEX_DMC_T_RFC         (0x2C)
#define INDEX_DMC_T_RP          (0x30)
#define INDEX_DMC_T_RRD         (0x34)
#define INDEX_DMC_T_WR          (0x38)
#define INDEX_DMC_T_WTR         (0x3C)
#define INDEX_DMC_T_XP          (0x40)
#define INDEX_DMC_T_XSR         (0x44)
#define INDEX_DMC_T_ESR         (0x48)
#define INDEX_DMC_MEMORY_CFG2	(0x4C)
#define INDEX_DMC_CHIP_0_CFG    (0x200)
#define INDEX_DMC_CHIP_1_CFG    (0x204)
#define INDEX_DMC_CHIP_2_CFG    (0x208)
#define INDEX_DMC_CHIP_3_CFG    (0x20C)
#define INDEX_DMC_USER_STATUS	(0x300)
#define INDEX_DMC_USER_CONFIG	(0x304)

// Memory Chip direct command
#define DMC_NOP0 			0x0c0000
#define DMC_NOP1			0x1c0000
#define DMC_PA0 			0x000000	//Precharge all
#define DMC_PA1 			0x100000
#define DMC_AR0 			0x040000	//Autorefresh
#define DMC_AR1 			0x140000
#define DMC_SDR_MR0			0x080032	//MRS, CAS 3,  Burst Length 4
#define DMC_SDR_MR1			0x180032
#define DMC_DDR_MR0			0x080162
#define DMC_DDR_MR1			0x180162
#define DMC_mDDR_MR0			0x080032	//CAS 3, Burst Length 4
#define DMC_mDDR_MR1			0x180032
#define DMC_mSDR_EMR0			0x0a0000	//EMRS, DS:Full, PASR:Full Array
#define DMC_mSDR_EMR1			0x1a0000
#define DMC_DDR_EMR0			0x090000
#define DMC_DDR_EMR1			0x190000
#define DMC_mDDR_EMR0			0x0a0000	// DS:Full, PASR:Full Array
#define DMC_mDDR_EMR1			0x1a0000


#endif //__S3C6410_H__
