/** @file

  Copyright (c) 2008 - 2009, Apple Inc. All rights reserved.<BR>

  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#ifndef __S3C6410RTC_H__
#define __S3C6410RTC_H__

#define RTC_INTP             0x7E005030
#define RTC_CON              0x7E005040
#define RTC_TICCNT           0x7E005044
#define RTC_ALM              0x7E005050
#define RTC_ALMSEC           0x7E005054
#define RTC_ALMMIN           0x7E005058
#define RTC_ALMHOUR          0x7E00505C
#define RTC_ALMDATE          0x7E005060
#define RTC_ALMMON           0x7E005064
#define RTC_ALMYEAR          0x7E005068
#define RTC_BCDSEC           0x7E005070
#define RTC_BCDMIN           0x7E005074
#define RTC_BCDHOUR          0x7E005078
#define RTC_BCDDATE          0x7E00507C
#define RTC_BCDDAY           0x7E005080
#define RTC_BCDMON           0x7E005084
#define RTC_BCDYEAR          0x7E005088
#define RTC_CURTICCNT        0x7E005090

#endif // __S3C6410RTC_H__
