/** @file

  Copyright (c) 2008 - 2009, Apple Inc. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef __S3C6410TIMER_H__
#define __S3C6410TIMER_H__

#define TIMER_CFG0         0x7F006000
#define TIMER_CFG1         0x7F006004
#define TIMER_CON          0x7F006008
#define TIMER_INT_CSTAT    0x7F006044

#define TIMER0_BASE        0x7F00600C
#define TIMER1_BASE        0x7F006018
#define TIMER2_BASE        0x7F006024
#define TIMER3_BASE        0x7F006030
#define TIMER4_BASE        0x7F00603c

#define TIMER_CNTB         0x0
#define TIMER_COMPB        0x4
#define TIMER_CONTO        0x8

#define TCFG0_PRESCALER    4

#endif // __S3C6410TIMER_H__

