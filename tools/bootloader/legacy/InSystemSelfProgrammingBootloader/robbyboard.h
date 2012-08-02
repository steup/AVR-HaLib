
/*CPU Clockrate*/
//#define F_CPU 8000000
#define F_CPU 16000000

/* UART Baudrate */
// #define BAUDRATE 9600
#define BAUDRATE 19200
//#define BAUDRATE 115200

/* use "Double Speed Operation" */
//#define UART_DOUBLESPEED

/* use second UART on mega128 / can128 */
#define UART_USE_SECOND


/*Device-Type:
 *For AVRProg the BOOT-option is prefered
 *which is the "correct" value for a bootloader.
 *avrdude may only detect the part-code for ISP
 */
//#define DEVTYPE     DEVTYPE_BOOT
#define DEVTYPE     DEVTYPE_ISP


/*
 * Select startup-mode
 * SIMPLE-Mode - Jump to bootloader main BL-loop if key is
 *   pressed (Pin grounded) "during" reset or jump to the
 *   application if the pin is not grounded. The internal
 *   pull-up resistor is enabled during the startup and
 *   gets disabled before the application is started.
 * POWERSAVE-Mode - Startup is separated in two loops
 *   which makes power-saving a little easier if no firmware
 *   is on the chip. Needs more memory
 * BOOTICE-Mode - to flash the JTAGICE upgrade.ebn file.
 *   No startup-sequence in this mode. Jump directly to the
 *   parser-loop on reset
 *   F_CPU in BOOTICEMODE must be 7372800 Hz to be compatible
 *   with the org. JTAGICE-Firmware
 * WAIT-mode waits 1 sec for the defined character if nothing
 *    is recived then the user prog is started.
 */
#define START_SIMPLE
//#define START_WAIT
//#define WAIT_SHORTCUT
//#define START_POWERSAVE
//#define START_BOOTICE
#define BLINK_BOOTLOADER

/*
 * Pin "STARTPIN" on port "STARTPORT" in this port has to grounded
 * (active low) to start the bootloader
 */
#define BLPORT      PORTA
#define BLDDR       DDRA
#define BLPIN       PINA
#define BLPNUM      PINA7

/*
 * if using BLINK_BOOTLOADER configure it here
 */
#define BLINKPORT   PORTB
#define BLINKBIT    7
#define BLINKDDR    DDRB
#define BLINKLEVEL  0

#define OLD_ROBBY
//#define RCBBreakout



/* character to start the bootloader in mode START_WAIT */
#define START_WAIT_UARTCHAR 'S'

/* wait 1s in START_WAIT mode (10ms steps) */
#define WAIT_VALUE 1000

/*
 * enable/disable readout of fuse and lock-bits
 * (AVRPROG has to detect the AVR correctly by device-code
 * to show the correct information).
 */
#define ENABLEREADFUSELOCK

/* enable/disable write of lock-bits
 * WARNING: lock-bits can not be reseted by bootloader (as far as I know)
 * Only protection no unprotection, "chip erase" from bootloader only
 * clears the flash but does no real "chip erase" (this is not possible
 * with a bootloader as far as I know)
 * Keep this undefined!
 */
//#define WRITELOCKBITS

#define VERSION_HIGH '0'
#define VERSION_LOW  '8'

#define GET_LOCK_BITS           0x0001
#define GET_LOW_FUSE_BITS       0x0000
#define GET_HIGH_FUSE_BITS      0x0003
#define GET_EXTENDED_FUSE_BITS  0x0002

