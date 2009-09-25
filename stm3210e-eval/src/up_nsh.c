/****************************************************************************
 * config/stm3210e_eval/src/up_nsh.c
 * arch/arm/src/board/up_nsh.c
 *
 *   Copyright (C) 2009 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <spudmonkey@racsa.co.cr>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <sys/types.h>

#include <stdio.h>
#include <debug.h>
#include <errno.h>

#include <nuttx/spi.h>
#include <nuttx/mmcsd.h>

/****************************************************************************
 * Pre-Processor Definitions
 ****************************************************************************/

/* Configuration ************************************************************/

/* PORT and SLOT number probably depend on the board configuration */

/* Can't support USB features if USB is not enabled */

#ifndef CONFIG_USBDEV
#  undef CONFIG_EXAMPLES_NSH_HAVEUSBDEV
#endif

/* Can't support MMC/SD features if mountpoints are disabled */

#if defined(CONFIG_DISABLE_MOUNTPOINT)
#  undef CONFIG_EXAMPLES_NSH_HAVEMMCSD
#endif

#ifndef CONFIG_EXAMPLES_NSH_MMCSDMINOR
#  define CONFIG_EXAMPLES_NSH_MMCSDMINOR 0
#endif

/* Debug ********************************************************************/

#ifdef CONFIG_CPP_HAVE_VARARGS
#  ifdef CONFIG_DEBUG
#    define message(...) lib_lowprintf(__VA_ARGS__)
#  else
#    define message(...) printf(__VA_ARGS__)
#  endif
#else
#  ifdef CONFIG_DEBUG
#    define message lib_lowprintf
#  else
#    define message printf
#  endif
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: nsh_archinitialize
 *
 * Description:
 *   Perform architecture specific initialization
 *
 ****************************************************************************/

int nsh_archinitialize(void)
{
  FAR struct spi_dev_s *spi;
  int ret;

  /* Get the SPI port */

  message("nsh_archinitialize: Initializing SPI port %d\n",
          CONFIG_EXAMPLES_NSH_MMCSDSPIPORTNO);

  spi = up_spiinitialize(CONFIG_EXAMPLES_NSH_MMCSDSPIPORTNO);
  if (!spi)
    {
      message("nsh_archinitialize: Failed to initialize SPI port %d\n",
              CONFIG_EXAMPLES_NSH_MMCSDSPIPORTNO);
      return -ENODEV;
    }

  message("nsh_archinitialize: Successfully initialized SPI port %d\n",
          CONFIG_EXAMPLES_NSH_MMCSDSPIPORTNO);

  /* Bind the SPI port to the slot */

  message("nsh_archinitialize: Binding SPI port %d to MMC/SD slot %d\n",
          CONFIG_EXAMPLES_NSH_MMCSDSPIPORTNO, CONFIG_EXAMPLES_NSH_MMCSDSLOTNO);

  ret = mmcsd_spislotinitialize(CONFIG_EXAMPLES_NSH_MMCSDMINOR, CONFIG_EXAMPLES_NSH_MMCSDSLOTNO, spi);
  if (ret < 0)
    {
      message("nsh_archinitialize: Failed to bind SPI port %d to MMC/SD slot %d: %d\n",
              CONFIG_EXAMPLES_NSH_MMCSDSPIPORTNO, CONFIG_EXAMPLES_NSH_MMCSDSLOTNO, ret);
      return ret;
    }

  message("nsh_archinitialize: Successfuly bound SPI port %d to MMC/SD slot %d\n",
          CONFIG_EXAMPLES_NSH_MMCSDSPIPORTNO, CONFIG_EXAMPLES_NSH_MMCSDSLOTNO);
  return OK;
}
