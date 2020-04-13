//
//  Copyright (c) 2011-2015, ARM Limited. All rights reserved.
//
//  This program and the accompanying materials
//  are licensed and made available under the terms and conditions of the BSD License
//  which accompanies this distribution.  The full text of the license may be found at
//  http://opensource.org/licenses/bsd-license.php
//
//  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
//  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
//
//

#include <AsmMacroIoLib.h>
#include <Base.h>
#include <Library/PcdLib.h>
#include <AutoGen.h>
#include <Chipset/ArmV7.h>
#include <Tiny6410.h>
#include <S3c6410/S3c6410.h>

.text
.align 2

GCC_ASM_IMPORT(CEntryPoint)
GCC_ASM_EXPORT(_ModuleEntryPoint)
GCC_ASM_EXPORT(mSystemMemoryEnd)

ASM_PFX(_ModuleEntryPoint):
  b     _Reset

StartupAddr:       .word  CEntryPoint
mSystemMemoryEnd:  .word 0
_SystemMemoryEnd:  .word mSystemMemoryEnd

_Reset:
  // Enter SVC mode, Disable FIQ and IRQ
  mov   r1, #(CPSR_MODE_SVC | CPSR_IRQ | CPSR_FIQ)
  msr   CPSR_c, r1

  // Disable Cache and Mmu
  mcr   p15, 0, r0, c7, c7, 0
  mrc   p15, 0, r0, c1, c0, 0
  LoadConstantToReg (0x00001007, r1)
  bic   r0, r0, r1
  mcr   p15, 0, r0, c1, c0, 0

  // Disable Watchdog
  LoadConstantToReg (WTCON, r0)
  mov   r1, #0x0
  str   r1, [r0]

  // Disable Interrupt
  mvn   r1, #0x0
  LoadConstantToReg(VIC0INTENCLEAR, r0)
  str   r1, [r0]
  LoadConstantToReg(VIC1INTENCLEAR, r0)
  str   r1, [r0]

  // Peripheral Port Memory Remap
  LoadConstantToReg (PERIPHERAL_PORT_REMAP_BASE, r0)
  orr   r0, r0, #0x13
  mcr   p15,0,r0,c15,c2,4

  // System Clock Initialize
  bl    _ClockInit

  // System Memory Initialize
  bl    _MemoryInit

  // Copy SD/MMC Card Data to memory
  bl    _CopyMMCtoMem

// Check if we can install the stack at the top of the System Memory or if we need
// to install the stacks at the bottom of the Firmware Device (case the FD is located
// at the top of the DRAM)
_SystemMemoryEndInit:
  ldr   r3, _SystemMemoryEnd
  ldr   r1, [r3]

  // Is mSystemMemoryEnd initialized?
  cmp   r1, #0
  bne   _SetupStackPosition

  LoadConstantToReg (FixedPcdGet32(PcdSystemMemoryBase), r1)
  LoadConstantToReg (FixedPcdGet32(PcdSystemMemorySize), r2)
  sub   r2, r2, #1
  add   r1, r1, r2
  // Update the global variable
  str   r1, [r3]

_SetupStackPosition:
  // r1 = SystemMemoryTop
  // Calculate Top of the Firmware Device
  LoadConstantToReg (FixedPcdGet32(PcdFdBaseAddress), r2)
  LoadConstantToReg (FixedPcdGet32(PcdFdSize), r3)
  sub   r3, r3, #1
  add   r3, r3, r2      // r3 = FdTop = PcdFdBaseAddress + PcdFdSize

  // UEFI Memory Size (stacks are allocated in this region)
  LoadConstantToReg (FixedPcdGet32(PcdSystemMemoryUefiRegionSize), r4)

  //
  // Reserve the memory for the UEFI region (contain stacks on its top)
  //

  // Calculate how much space there is between the top of the Firmware and the Top of the System Memory
  subs  r0, r1, r3      // r0 = SystemMemoryTop - FdTop
  bmi   _SetupStack     // Jump if negative (FdTop > SystemMemoryTop). Case when the PrePi is in XIP memory outside of the DRAM
  cmp   r0, r4
  bge   _SetupStack

  // Case the top of stacks is the FdBaseAddress
  mov   r1, r2

_SetupStack:
  // r1 contains the top of the stack (and the UEFI Memory)

  // Because the 'push' instruction is equivalent to 'stmdb' (decrement before), we need to increment
  // one to the top of the stack. We check if incrementing one does not overflow (case of DRAM at the
  // top of the memory space)
  adds  r9, r1, #1
  bcs   _SetupOverflowStack

_SetupAlignedStack:
  mov   r1, r9
  b     _GetBaseUefiMemory

_SetupOverflowStack:
  // Case memory at the top of the address space. Ensure the top of the stack is EFI_PAGE_SIZE
  // aligned (4KB)
  LoadConstantToReg (EFI_PAGE_MASK, r9)
  and   r9, r9, r1
  sub   r1, r1, r9

_GetBaseUefiMemory:
  // Calculate the Base of the UEFI Memory
  sub   r9, r1, r4

_GetStackBase:
  // r1 = The top of the Mpcore Stacks
  // Stack for the primary core = PrimaryCoreStack
  LoadConstantToReg (FixedPcdGet32(PcdCPUCorePrimaryStackSize), r2)
  sub   r10, r1, r2

  // Stack for the secondary core = Number of Cores - 1
  LoadConstantToReg (FixedPcdGet32(PcdCoreCount), r0)
  sub   r0, r0, #1
  LoadConstantToReg (FixedPcdGet32(PcdCPUCoreSecondaryStackSize), r1)
  mul   r1, r1, r0
  sub   r10, r10, r1

  // r10 = The base of the MpCore Stacks (primary stack & secondary stacks)
  LoadConstantToReg (FixedPcdGet32(PcdCPUCorePrimaryStackSize), r1)
  LoadConstantToReg (FixedPcdGet32(PcdCPUCoreSecondaryStackSize), r2)
  // Add stack of primary stack to StackBase
  add   r0, r10, r1
  // Compute SecondaryCoresCount * SecondaryCoreStackSize
  LoadConstantToReg (FixedPcdGet32(PcdCoreCount), r1)
  sub   r1, #1
  mul   r2, r2, r1
  // Set Primary Stack ((StackBase + PrimaryStackSize) + (SecondaryCoresCount * SecondaryCoreStackSize))
  add   sp, r0, r2

_ReserveGlobalVariable:
  LoadConstantToReg (FixedPcdGet32(PcdPeiGlobalVariableSize), r0)
  // InitializePrimaryStack($GlobalVariableSize, $Tmp1)
  InitializePrimaryStack(r0, r1)

_PreiareArguments:
  mrc   p15, 0, r0, c0, c0, 0  // MpId
  mov   r1, r9
  mov   r2, r10
  mov   r3, sp

  // Move sec startup address into a data register
  // Ensure we're jumping to FV version of the code (not boot remapped alias)
  ldr   r4, StartupAddr

  // Jump to PrePiCore C code
  //    r0 = MpId
  //    r1 = UefiMemoryBase
  //    r2 = StacksBase
  //    r3 = GlobalVariableBase
  blx   r4

_NeverReturn:
  b _NeverReturn

_ClockInit:
  ldr   r0, =CLK_POWER_BASE
  ldr   r1, [r0, #OTHERS]
  mov   r2, #0x40
  orr   r1, r1, r2
  str   r1, [r0, #OTHERS]

  nop
  nop
  nop
  nop
  nop

  ldr   r2, =0x80
  orr   r1, r1, r2
  str   r1, [r0, #OTHERS]

_CheckSyncack:
  ldr   r1, [r0, #OTHERS]
  ldr   r2, =0xf00
  and   r1, r1, r2
  cmp   r1, #0xf00
  bne   _CheckSyncack

  mov   r1, #0xff00
  orr   r1, r1, #0xff
  str   r1, [r0, #APLL_LOCK]
  str   r1, [r0, #MPLL_LOCK]
  str   r1, [r0, #EPLL_LOCK]

  ldr   r1, [r0, #CLK_DIV0]  /*Set Clock Divider*/
  bic   r1, r1, #0x30000
  bic   r1, r1, #0xff00
  bic   r1, r1, #0xff
  ldr   r2, =CLK_DIV_VAL
  orr   r1, r1, r2
  str   r1, [r0, #CLK_DIV0]

  ldr   r1, =APLL_VAL
  str   r1, [r0, #APLL_CON]
  ldr   r1, =MPLL_VAL
  str   r1, [r0, #MPLL_CON]

  ldr   r1, =0x80200203     /* FOUT of EPLL is 96MHz */
  str   r1, [r0, #EPLL_CON0]
  ldr   r1, =0x0
  str   r1, [r0, #EPLL_CON1]

  ldr   r1, [r0, #CLK_SRC] /* APLL, MPLL, EPLL select to Fout */
  ldr   r2, =0x7
  orr   r1, r1, r2

  str   r1, [r0, #CLK_SRC]

  /* wait at least 200us to stablize all clock */
  mov   r1, #0x10000
_Wait:
  subs  r1, r1, #1
  bne   _Wait

  ldr   r1, [r0, #OTHERS]
  orr   r1, r1, #0x20
  str   r1, [r0, #OTHERS]

  bx    lr

_MemoryInit:
  ldr   r0, =ELFIN_MEM_SYS_CFG      @Memory sussystem address 0x7e00f120
  mov   r1, #0xd        @ Xm0CSn2 = NFCON CS0, Xm0CSn3 = NFCON CS1
  str   r1, [r0]

  ldr   r0, =ELFIN_DMC1_BASE      @DMC1 base address 0x7e001000

  ldr   r1, =0x04
  str   r1, [r0, #INDEX_DMC_MEMC_CMD]

  ldr   r1, =DMC_DDR_REFRESH_PRD
  str   r1, [r0, #INDEX_DMC_REFRESH_PRD]

  ldr   r1, =DMC_DDR_CAS_LATENCY
  str   r1, [r0, #INDEX_DMC_CAS_LATENCY]

  ldr   r1, =DMC_DDR_t_DQSS
  str   r1, [r0, #INDEX_DMC_T_DQSS]

  ldr   r1, =DMC_DDR_t_MRD
  str   r1, [r0, #INDEX_DMC_T_MRD]

  ldr   r1, =DMC_DDR_t_RAS
  str   r1, [r0, #INDEX_DMC_T_RAS]

  ldr   r1, =DMC_DDR_t_RC
  str   r1, [r0, #INDEX_DMC_T_RC]

  ldr   r1, =DMC_DDR_t_RCD
  ldr   r2, =DMC_DDR_schedule_RCD
  orr   r1, r1, r2
  str   r1, [r0, #INDEX_DMC_T_RCD]

  ldr   r1, =DMC_DDR_t_RFC
  ldr   r2, =DMC_DDR_schedule_RFC
  orr   r1, r1, r2
  str   r1, [r0, #INDEX_DMC_T_RFC]

  ldr   r1, =DMC_DDR_t_RP
  ldr   r2, =DMC_DDR_schedule_RP
  orr   r1, r1, r2
  str   r1, [r0, #INDEX_DMC_T_RP]

  ldr   r1, =DMC_DDR_t_RRD
  str   r1, [r0, #INDEX_DMC_T_RRD]

  ldr   r1, =DMC_DDR_t_WR
  str   r1, [r0, #INDEX_DMC_T_WR]

  ldr   r1, =DMC_DDR_t_WTR
  str   r1, [r0, #INDEX_DMC_T_WTR]

  ldr   r1, =DMC_DDR_t_XP
  str   r1, [r0, #INDEX_DMC_T_XP]

  ldr   r1, =DMC_DDR_t_XSR
  str   r1, [r0, #INDEX_DMC_T_XSR]

  ldr   r1, =DMC_DDR_t_ESR
  str   r1, [r0, #INDEX_DMC_T_ESR]

  ldr   r1, =DMC1_MEM_CFG
  str   r1, [r0, #INDEX_DMC_MEMORY_CFG]

  ldr   r1, =DMC1_MEM_CFG2
  str   r1, [r0, #INDEX_DMC_MEMORY_CFG2]

  ldr   r1, =DMC1_CHIP0_CFG
  str   r1, [r0, #INDEX_DMC_CHIP_0_CFG]

  ldr   r1, =DMC_DDR_32_CFG
  str   r1, [r0, #INDEX_DMC_USER_CONFIG]

  @DMC0 DDR Chip 0 configuration direct command reg
  ldr   r1, =DMC_NOP0
  str   r1, [r0, #INDEX_DMC_DIRECT_CMD]

  @Precharge All
  ldr   r1, =DMC_PA0
  str   r1, [r0, #INDEX_DMC_DIRECT_CMD]

  @Auto Refresh 2 time
  ldr   r1, =DMC_AR0
  str   r1, [r0, #INDEX_DMC_DIRECT_CMD]
  str   r1, [r0, #INDEX_DMC_DIRECT_CMD]

  @MRS
  ldr   r1, =DMC_mDDR_EMR0
  str   r1, [r0, #INDEX_DMC_DIRECT_CMD]

  @Mode Reg
  ldr   r1, =DMC_mDDR_MR0
  str   r1, [r0, #INDEX_DMC_DIRECT_CMD]

  @Enable DMC1
  mov   r1, #0x0
  str   r1, [r0, #INDEX_DMC_MEMC_CMD]

_CheckDmc1Ready:
  ldr   r1, [r0, #INDEX_DMC_MEMC_STATUS]
  mov   r2, #0x3
  and   r1, r1, r2
  cmp   r1, #0x1
  bne   _CheckDmc1Ready
  nop

  bx lr

_CopyMMCtoMem:
  // Set temp stack to call SD/MMC Copy function
  LoadConstantToReg (FixedPcdGet32(PcdSystemMemoryBase), r0)
  LoadConstantToReg (FixedPcdGet32(PcdSystemMemorySize), r1)
  add   sp, r0, r1

  str   lr, [sp, #-4]!

  LoadConstantToReg (CONTROL4_0, r0)
  ldr   r1, [r0]
  mov   r2, #3
  orr   r1, r1, r2, lsl #16
  str   r1, [r0]

  mov   r0, #0
  str   r0, [sp, #-4]!

  LoadConstantToReg (FixedPcdGet32(PcdFdSize), r2)
  mov   r2, r2, lsr #9

  LoadConstantToReg (GLOBAL_BLOCK_CNT, r1)
  ldr   r1, [r1]
  sub   r1, r1, r2
  LoadConstantToReg (BL1_SIG_RESERV_CNT, r3)
  sub   r1, r1, r3

  LoadConstantToReg (FixedPcdGet32(PcdFdBaseAddress), r3)

  LoadConstantToReg (COPY_MMC_TO_MEM, r4)
  ldr   r4, [r4]
  // Jump to Copy MMC to Memory code
  //    r0 = Channel
  //    r1 = StartBlkAddress
  //    r2 = BlockSize
  //    r3 = MemoryPtr
  //    sp = With_init
  blx   r4

  ldr   lr, [sp, #4]
  bx    lr
