/** @file

  Copyright (c) 2008 - 2009, Apple Inc. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef __S3C6410GPIO_H__
#define __S3C6410GPIO_H__

#define GPIOA_BASE   0x7F008000
#define GPIOB_BASE   0x7F008020
#define GPIOC_BASE   0x7F008040
#define GPIOD_BASE   0x7F008060
#define GPIOE_BASE   0x7F008080
#define GPIOF_BASE   0x7F0080A0
#define GPIOG_BASE   0x7F0080C0
#define GPIOH_BASE   0x7F0080E0
#define GPIOI_BASE   0x7F008100
#define GPIOJ_BASE   0x7F008120
#define GPIOK_BASE   0x7F008800
#define GPIOL_BASE   0x7F008810
#define GPIOM_BASE   0x7F008820
#define GPION_BASE   0x7F008830
#define GPIOO_BASE   0x7F008140
#define GPIOP_BASE   0x7F008160
#define GPIOQ_BASE   0x7F008180

// GPIO Control Reg
#define GPIO_CON      0x00
#define GPIO_DAT      0x04
#define GPIO_PUD      0x08
#define GPIO_CONSLP   0x0C
#define GPIO_PUDSLP   0x10

// GPIO H, K, L
#define GPIOHKL_CON0  0x00
#define GPIOHKL_CON1  0x04
#define GPIOHKL_DAT   0x08
#define GPIOHKL_PUD   0x0C
#define GPIOH_CONSLP  0x10
#define GPIOH_PUDSLP  0x14


#define GPIO_DATAIN_MASK(x)               (1UL << (x))

#define IsGioHKL(Port) (Port == 'H' || Port == 'K' || Port == 'L')
#define Con2Bits(Port) (Port == 'F' || Port == 'I' || Port == 'J' || Port == 'N' || Port == 'O' || Port == 'P' || Port == 'Q')

#endif // __S3C6410GPIO_H__

