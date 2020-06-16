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

// Interrupt
#define VIC0_BASE           0x71200000
#define VIC1_BASE           0x71300000

#define INTMOD              0x00C    // VIC INT SELECT (IRQ or FIQ)
#define INTUNMSK            0x010    // VIC INT EN (Unmask by writing 1)
#define INTMSK              0x014    // VIC INT EN CLEAR (Mask by writing 1)
#define INTSUBMSK           0x01C    // VIC SOFT INT CLEAR
#define VECTADDR            0xF00    // VIC ADDRESS

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

#define INT_NROF_VECTORS    64
#define MAX_VECTOR          (INT_NROF_VECTORS - 1)


#define INTERRUPT_BASE (0x48200000)

#define INTCPS_SYSCONFIG      (INTERRUPT_BASE + 0x0010)
#define INTCPS_SYSSTATUS      (INTERRUPT_BASE + 0x0014)
#define INTCPS_SIR_IRQ        (INTERRUPT_BASE + 0x0040)
#define INTCPS_SIR_IFQ        (INTERRUPT_BASE + 0x0044)
#define INTCPS_CONTROL        (INTERRUPT_BASE + 0x0048)
#define INTCPS_PROTECTION     (INTERRUPT_BASE + 0x004C)
#define INTCPS_IDLE           (INTERRUPT_BASE + 0x0050)
#define INTCPS_IRQ_PRIORITY   (INTERRUPT_BASE + 0x0060)
#define INTCPS_FIQ_PRIORITY   (INTERRUPT_BASE + 0x0064)
#define INTCPS_THRESHOLD      (INTERRUPT_BASE + 0x0068)
#define INTCPS_ITR(n)         (INTERRUPT_BASE + 0x0080 + (0x20 * (n)))
#define INTCPS_MIR(n)         (INTERRUPT_BASE + 0x0084 + (0x20 * (n)))
#define INTCPS_MIR_CLEAR(n)   (INTERRUPT_BASE + 0x0088 + (0x20 * (n)))
#define INTCPS_MIR_SET(n)     (INTERRUPT_BASE + 0x008C + (0x20 * (n)))
#define INTCPS_ISR_SET(n)     (INTERRUPT_BASE + 0x0090 + (0x20 * (n)))
#define INTCPS_ISR_CLEAR(n)   (INTERRUPT_BASE + 0x0094 + (0x20 * (n)))
#define INTCPS_PENDING_IRQ(n) (INTERRUPT_BASE + 0x0098 + (0x20 * (n)))
#define INTCPS_PENDING_FIQ(n) (INTERRUPT_BASE + 0x009C + (0x20 * (n)))
#define INTCPS_ILR(m)         (INTERRUPT_BASE + 0x0100 + (0x04 * (m)))

#define INTCPS_ILR_FIQ            BIT0
#define INTCPS_SIR_IRQ_MASK       (0x7F)
#define INTCPS_CONTROL_NEWIRQAGR  BIT0
#define INTCPS_CONTROL_NEWFIQAGR  BIT1

#endif // __S3C6410INTERRUPT_H__

