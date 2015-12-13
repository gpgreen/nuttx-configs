/************************************************************************************
 * configs/same70-xplained/src/same70-xplained.h
 *
 *   Copyright (C) 2015 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
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
 ************************************************************************************/

#ifndef __CONFIGS_SAME70_XPLAINED_SRC_SAME70_XPLAINED_H
#define __CONFIGS_SAME70_XPLAINED_SRC_SAME70_XPLAINED_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <nuttx/config.h>

#include <stdint.h>
#include <stdbool.h>

#include <arch/irq.h>
#include <nuttx/irq.h>

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/
/* Configuration ********************************************************************/

#define HAVE_HSMCI           1
#define HAVE_AUTOMOUNTER     1
#define HAVE_USB             1
#define HAVE_USBDEV          1
#define HAVE_USBMONITOR      1
#define HAVE_NETWORK         1
#define HAVE_MACADDR         1
#define HAVE_MTDCONFIG       1
#define HAVE_PROGMEM_CHARDEV 1

/* HSMCI */
/* Can't support MMC/SD if the card interface is not enabled */

#if !defined(CONFIG_SAMV7_HSMCI0)
#  undef HAVE_HSMCI
#endif

/* Can't support MMC/SD features if mountpoints are disabled */

#if defined(HAVE_HSMCI) && defined(CONFIG_DISABLE_MOUNTPOINT)
#  warning Mountpoints disabled.  No MMC/SD support
#  undef HAVE_HSMCI
#endif

/* We need PIO interrupts on GPIOC to support card detect interrupts */

#if defined(HAVE_HSMCI) && !defined(CONFIG_SAMV7_GPIOC_IRQ)
#  warning PIOC interrupts not enabled.  No MMC/SD support.
#  undef HAVE_HSMCI
#endif

/* MMC/SD minor numbers */

#ifndef CONFIG_NSH_MMCSDMINOR
#  define CONFIG_NSH_MMCSDMINOR 0
#endif

#ifndef CONFIG_NSH_MMCSDMINOR
#  define CONFIG_NSH_MMCSDSLOTNO 0
#endif

#if CONFIG_NSH_MMCSDMINOR != 0
#  error SAME70 has only one MMC/SD slot (CONFIG_NSH_MMCSDMINOR)
#  undef CONFIG_NSH_MMCSDSLOTNO
#  define CONFIG_NSH_MMCSDSLOTNO 0
#endif

#define HSMCI0_SLOTNO CONFIG_NSH_MMCSDSLOTNO
#define HSMCI0_MINOR  CONFIG_NSH_MMCSDMINOR

/* Automounter.  Currently only works with HSMCI. */

#if !defined(CONFIG_FS_AUTOMOUNTER) || !defined(HAVE_HSMCI)
#  undef HAVE_AUTOMOUNTER
#  undef CONFIG_SAME70XPLAINED_HSMCI0_AUTOMOUNT
#endif

#ifndef CONFIG_SAME70XPLAINED_HSMCI0_AUTOMOUNT
#  undef HAVE_AUTOMOUNTER
#endif

#ifdef HAVE_AUTOMOUNTER
#  ifdef CONFIG_SAME70XPLAINED_HSMCI0_AUTOMOUNT
  /* HSMCI0 Automounter defaults */

#    ifndef CONFIG_SAME70XPLAINED_HSMCI0_AUTOMOUNT_FSTYPE
#      define CONFIG_SAME70XPLAINED_HSMCI0_AUTOMOUNT_FSTYPE "vfat"
#    endif

#    ifndef CONFIG_SAME70XPLAINED_HSMCI0_AUTOMOUNT_BLKDEV
#      define CONFIG_SAME70XPLAINED_HSMCI0_AUTOMOUNT_BLKDEV "/dev/mmcds0"
#    endif

#    ifndef CONFIG_SAME70XPLAINED_HSMCI0_AUTOMOUNT_MOUNTPOINT
#      define CONFIG_SAME70XPLAINED_HSMCI0_AUTOMOUNT_MOUNTPOINT "/mnt/sdcard0"
#    endif

#    ifndef CONFIG_SAME70XPLAINED_HSMCI0_AUTOMOUNT_DDELAY
#      define CONFIG_SAME70XPLAINED_HSMCI0_AUTOMOUNT_DDELAY 1000
#    endif

#    ifndef CONFIG_SAME70XPLAINED_HSMCI0_AUTOMOUNT_UDELAY
#      define CONFIG_SAME70XPLAINED_HSMCI0_AUTOMOUNT_UDELAY 2000
#    endif
#  endif /* CONFIG_SAME70XPLAINED_HSMCI0_AUTOMOUNT */
#endif /* HAVE_AUTOMOUNTER */

/* USB Device */
/* CONFIG_SAMV7_UDP and CONFIG_USBDEV must be defined, or there is no USB
 * device.
 */

#if !defined(CONFIG_SAMV7_UDP) || !defined(CONFIG_USBDEV)
#  undef HAVE_USB
#  undef HAVE_USBDEV
#endif

/* Check if we should enable the USB monitor before starting NSH */

#ifndef HAVE_USBDEV
#  undef CONFIG_USBDEV_TRACE
#endif

#if !defined(CONFIG_SYSTEM_USBMONITOR) || !defined(CONFIG_USBDEV_TRACE)
#  undef HAVE_USBMONITOR
#endif

/* Networking and AT24-based MTD config */

#if !defined(CONFIG_NET) || !defined(CONFIG_SAMV7_EMAC)
#  undef HAVE_NETWORK
#  undef HAVE_MACADDR
#endif

#if !defined(CONFIG_SAMV7_TWIHS0) || !defined(CONFIG_MTD_AT24XX)
#  undef HAVE_MACADDR
#  undef HAVE_MTDCONFIG
#endif

#if defined(CONFIG_NSH_NOMAC) || !defined(CONFIG_AT24XX_EXTENDED)
#  undef HAVE_MACADDR
#endif

#if !defined(CONFIG_MTD_CONFIG)
#  undef HAVE_MTDCONFIG
#endif

/* On-chip Programming Memory */

#if !defined(CONFIG_SAMV7_PROGMEM) || !defined(CONFIG_MTD_PROGMEM)
#  undef HAVE_PROGMEM_CHARDEV
#endif

/* This is the on-chip progmem memroy driver minor number */

#define PROGMEM_MTD_MINOR 0

/* procfs File System */

#ifdef CONFIG_FS_PROCFS
#  ifdef CONFIG_NSH_PROC_MOUNTPOINT
#    define SAME70_PROCFS_MOUNTPOINT CONFIG_NSH_PROC_MOUNTPOINT
#  else
#    define SAME70_PROCFS_MOUNTPOINT "/proc"
#  endif
#endif

/* SAME70-XPLD GPIO Pin Definitions *************************************************/

/* Ethernet MAC.
 *
 * KSZ8081RNACA Connections
 * ------------------------
 *
 *   ------ --------- ---------
 *   SAME70 SAME70    Ethernet
 *   Pin    Function  Functio
 *   ------ --------- ---------
 *   PD0    GTXCK     REF_CLK
 *   PD1    GTXEN     TXEN
 *   PD2    GTX0      TXD0
 *   PD3    GTX1      TXD1
 *   PD4    GRXDV     CRS_DV
 *   PD5    GRX0      RXD0
 *   PD6    GRX1      RXD1
 *   PD7    GRXER     RXER
 *   PD8    GMDC      MDC
 *   PD9    GMDIO     MDIO
 *   PA14   GPIO      INTERRUPT
 *   PC10   GPIO      RESET
 *   ------ --------- ---------
 */

#define GPIO_EMAC0_INT    (GPIO_INPUT | GPIO_CFG_PULLUP | GPIO_CFG_DEGLITCH | \
                           GPIO_INT_FALLING | GPIO_PORT_PIOA | GPIO_PIN14)
#define GPIO_EMAC0_RESET  (GPIO_OUTPUT | GPIO_CFG_PULLUP | GPIO_OUTPUT_SET | \
                           GPIO_PORT_PIOC | GPIO_PIN10)

#define IRQ_EMAC0_INT     SAM_IRQ_PA14

/* LEDs
 *
 * A single LED is available driven by PC8.
 */

#define GPIO_LED0     (GPIO_OUTPUT | GPIO_CFG_DEFAULT | GPIO_OUTPUT_SET | \
                       GPIO_PORT_PIOC | GPIO_PIN8)

/* Buttons
 *
 * SAM E70 Xplained contains two mechanical buttons. One button is the RESET
 * button connected to the SAM E70 reset line and the other, PA11, is a generic
 * user configurable button. When a button is pressed it will drive the I/O
 * line to GND.
 *
 * NOTE: There are no pull-up resistors connected to the generic user buttons
 * so it is necessary to enable the internal pull-up in the SAM E70 to use the
 * button.
 */

#define GPIO_SW0      (GPIO_INPUT | GPIO_CFG_PULLUP | GPIO_CFG_DEGLITCH | \
                       GPIO_INT_BOTHEDGES | GPIO_PORT_PIOA | GPIO_PIN11)
#define IRQ_SW0       SAM_IRQ_PA11

/* HSMCI SD Card Detect
 *
 * The SAM E70 Xplained has one standard SD card connector that is connected
 * to the High Speed Multimedia Card Interface (HSMCI) of the SAM E70. SD
 * card connector:
 *
 *   ------ ----------------- ---------------------
 *   SAME70 SAME70            Shared functionality
 *   Pin    Function
 *   ------ ----------------- ---------------------
 *   PA30   MCDA0 (DAT0)
 *   PA31   MCDA1 (DAT1)
 *   PA26   MCDA2 (DAT2)
 *   PA27   MCDA3 (DAT3)      Camera
 *   PA25   MCCK (CLK)        Shield
 *   PA28   MCCDA (CMD)
 *   PC16   Card Detect (C/D) Shield
 *   ------ ----------------- ---------------------
 */

#define GPIO_MCI0_CD (GPIO_INPUT | GPIO_CFG_DEFAULT | GPIO_CFG_DEGLITCH | \
                      GPIO_INT_BOTHEDGES | GPIO_PORT_PIOC | GPIO_PIN16)
#define IRQ_MCI0_CD   SAM_IRQ_PC16

/* USB Host
 *
 * The SAM E70 Xplained has a Micro-USB connector for use with the SAM E70 USB
 * module labeled as TARGET USB on the kit. In USB host mode VBUS voltage is
 * provided by the kit and has to be enabled by setting the "VBUS Host Enable"
 * pin (PC16) low.
 */

#define GPIO_VBUSON (GPIO_OUTPUT | GPIO_CFG_DEFAULT | GPIO_OUTPUT_SET | \
                     GPIO_PORT_PIOC | GPIO_PIN16)

/************************************************************************************
 * Public Types
 ************************************************************************************/

/************************************************************************************
 * Public data
 ************************************************************************************/

#ifndef __ASSEMBLY__

/************************************************************************************
 * Public Functions
 ************************************************************************************/

/************************************************************************************
 * Name: sam_sdram_config
 *
 * Description:
 *   Configures the on-board SDRAM.  SAME70 Xplained features one external
 *   IS42S16100E-7BLI, 512Kx16x2, 10ns, SDRAM. SDRAM0 is connected to chip select
 *   NCS1.
 *
 *  Input Parameters:
 *     None
 *
 *  Assumptions:
 *    The DDR memory regions is configured as strongly ordered memory.  When we
 *    complete initialization of SDRAM and it is ready for use, we will make DRAM
 *    into normal memory.
 *
 ************************************************************************************/

#ifdef CONFIG_SAMV7_SDRAMC
void sam_sdram_config(void);
#else
#  define sam_sdram_config(t)
#endif

/************************************************************************************
 * Name: sam_bringup
 *
 * Description:
 *   Bring up board features
 *
 ************************************************************************************/

#if defined(CONFIG_NSH_ARCHINIT) || defined(CONFIG_BOARD_INITIALIZE)
int sam_bringup(void);
#endif

/************************************************************************************
 * Name: sam_spi_initialize
 *
 * Description:
 *   Called to configure SPI chip select GPIO pins for the SAME70-XPLD board.
 *
 ************************************************************************************/

#ifdef CONFIG_SAMV7_SPI
void sam_spi_initialize(void);
#endif

/************************************************************************************
 * Name: sam_hsmci_initialize
 *
 * Description:
 *   Initialize HSMCI support
 *
 ************************************************************************************/

#ifdef HAVE_HSMCI
int sam_hsmci_initialize(int slot, int minor);
#else
# define sam_hsmci_initialize(s,m) (-ENOSYS)
#endif

/************************************************************************************
 * Name:  sam_usbinitialize
 *
 * Description:
 *   Called from stm32_boardinitialize very early in initialization to setup USB-
 *   related GPIO pins for the SAME70-XPLD board.
 *
 ************************************************************************************/

#ifdef HAVE_USB
void sam_usbinitialize(void);
#endif

/************************************************************************************
 * Name: sam_netinitialize
 *
 * Description:
 *   Configure board resources to support networking.
 *
 ************************************************************************************/

#ifdef HAVE_NETWORK
void sam_netinitialize(void);
#endif

/************************************************************************************
 * Name: sam_emac0_setmac
 *
 * Description:
 *   Read the Ethernet MAC address from the AT24 FLASH and configure the Ethernet
 *   driver with that address.
 *
 ************************************************************************************/

#ifdef HAVE_MACADDR
int sam_emac0_setmac(void);
#endif

/************************************************************************************
 * Name: sam_cardinserted
 *
 * Description:
 *   Check if a card is inserted into the selected HSMCI slot
 *
 ************************************************************************************/

#ifdef HAVE_HSMCI
bool sam_cardinserted(int slotno);
#else
#  define sam_cardinserted(slotno) (false)
#endif

/************************************************************************************
 * Name: sam_writeprotected
 *
 * Description:
 *   Check if the card in the MMCSD slot is write protected
 *
 ************************************************************************************/

#ifdef HAVE_HSMCI
bool sam_writeprotected(int slotno);
#endif

/************************************************************************************
 * Name:  sam_automount_initialize
 *
 * Description:
 *   Configure auto-mounters for each enable and so configured HSMCI
 *
 * Input Parameters:
 *   None
 *
 *  Returned Value:
 *    None
 *
 ************************************************************************************/

#ifdef HAVE_AUTOMOUNTER
void sam_automount_initialize(void);
#endif

/************************************************************************************
 * Name:  sam_automount_event
 *
 * Description:
 *   The HSMCI card detection logic has detected an insertion or removal event.  It
 *   has already scheduled the MMC/SD block driver operations.  Now we need to
 *   schedule the auto-mount event which will occur with a substantial delay to make
 *   sure that everything has settle down.
 *
 * Input Parameters:
 *   slotno - Identifies the HSMCI0 slot: HSMCI0 or HSMCI1_SLOTNO.  There is a
 *      terminology problem here:  Each HSMCI supports two slots, slot A and slot B.
 *      Only slot A is used.  So this is not a really a slot, but an HSCMI peripheral
 *      number.
 *   inserted - True if the card is inserted in the slot.  False otherwise.
 *
 *  Returned Value:
 *    None
 *
 *  Assumptions:
 *    Interrupts are disabled.
 *
 ************************************************************************************/

#ifdef HAVE_AUTOMOUNTER
void sam_automount_event(int slotno, bool inserted);
#endif

/************************************************************************************
 * Name: sam_writeprotected
 *
 * Description:
 *   Check if the card in the MMCSD slot is write protected
 *
 ************************************************************************************/

#ifdef HAVE_HSMCI
bool sam_writeprotected(int slotno);
#else
#  define sam_writeprotected(slotno) (false)
#endif

/************************************************************************************
 * Name: sam_at24config
 *
 * Description:
 *   Create an AT24xx-based MTD configuration device for storage device configuration
 *   information.
 *
 ************************************************************************************/

#ifdef HAVE_MTDCONFIG
int sam_at24config(void);
#endif

#endif /* __ASSEMBLY__ */
#endif /* __CONFIGS_SAME70_XPLAINED_SRC_SAME70_XPLAINED_H */