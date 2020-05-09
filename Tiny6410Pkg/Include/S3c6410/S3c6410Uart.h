/** @file

  Copyright (c) 2008 - 2009, Apple Inc. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef __S3C6410UART_H__
#define __S3C6410UART_H__

#define UART0_BASE  0x7F005000
#define UART1_BASE  0x7F005400
#define UART2_BASE  0x7F005800
#define UART3_BASE  0x7F005C00

#define UART_LCON      0x00
#define UART_CON       0x04
#define UART_FCON      0x08
#define UART_MCON      0x0C
#define UART_TRSTAT    0x10
#define UART_ERSTAT    0x14
#define UART_FSTAT     0x18
#define UART_MSTAT     0x1C
#define UART_TXH       0x20
#define UART_RXH       0x24
#define UART_BRDIV     0x28
#define UART_DIVSLOT   0x2C
#define UART_INTP      0x30
#define UART_INTSP     0x34
#define UART_INTM      0x38

#define UART_DLL_REG  (0x0000)
#define UART_RBR_REG  (0x0000)
#define UART_THR_REG  (0x0000)
#define UART_DLH_REG  (0x0004)
#define UART_FCR_REG  (0x0008)
#define UART_LCR_REG  (0x000C)
#define UART_MCR_REG  (0x0010)
#define UART_LSR_REG  (0x0014)
#define UART_MDR1_REG (0x0020)

#define UART_FCR_TX_FIFO_CLEAR          BIT2
#define UART_FCR_RX_FIFO_CLEAR          BIT1
#define UART_FCR_FIFO_ENABLE            BIT0

#define UART_LCR_DIV_EN_ENABLE          BIT7
#define UART_LCR_DIV_EN_DISABLE         (0UL << 7)
#define UART_LCR_CHAR_LENGTH_8          (BIT1 | BIT0)

#define UART_MCR_RTS_FORCE_ACTIVE       BIT1
#define UART_MCR_DTR_FORCE_ACTIVE       BIT0

#define UART_LSR_TX_FIFO_E_MASK         BIT5
#define UART_LSR_TX_FIFO_E_NOT_EMPTY    (0UL << 5)
#define UART_LSR_TX_FIFO_E_EMPTY        BIT5
#define UART_LSR_RX_FIFO_E_MASK         BIT0
#define UART_LSR_RX_FIFO_E_NOT_EMPTY    BIT0
#define UART_LSR_RX_FIFO_E_EMPTY        (0UL << 0)

// BIT2:BIT0
#define UART_MDR1_MODE_SELECT_DISABLE   (7UL)
#define UART_MDR1_MODE_SELECT_UART_16X  (0UL)

#endif // __S3C6410UART_H__
