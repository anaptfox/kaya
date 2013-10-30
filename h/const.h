#ifndef CONSTS
#define CONSTS

/**************************************************************************** 
 *
 * This header file contains utility constants & macro definitions.
 * 
 ****************************************************************************/

/* Hardware & software constants */
#define PAGESIZE		4096	/* page size in bytes */
#define WORDLEN			4		/* word size in bytes */
#define PTEMAGICNO		0x2A

#define MAXPROC 20

#define ROMPAGESTART	0x20000000	 /* ROM Reserved Page */


/* timer, timescale, TOD-LO and other bus regs */
#define RAMBASEADDR	0x10000000
#define TODLOADDR	0x1000001C
#define INTERVALTMR	0x10000020	
#define TIMESCALEADDR	0x10000024


/* utility constants */
#define	TRUE		1
#define	FALSE		0
#define HIDDEN		static
#define EOS		'\0'

#define NULL ((void *)0xFFFFFFFF)


/* vectors number and type */
#define VECTSNUM	4

#define TLBTRAP		0
#define PROGTRAP	1
#define SYSTRAP		2

#define TRAPTYPES	3


/* device interrupts */
#define DISKINT		3
#define TAPEINT 	4
#define NETWINT 	5
#define PRNTINT 	6
#define TERMINT		7

#define DEVREGLEN	4	/* device register field length in bytes & regs per dev */
#define DEVREGSIZE	16 	/* device register size in bytes */

/* device register field number for non-terminal devices */
#define STATUS		0
#define COMMAND		1
#define DATA0		2
#define DATA1		3

/* device register field number for terminal devices */
#define RECVSTATUS      0
#define RECVCOMMAND     1
#define TRANSTATUS      2
#define TRANCOMMAND     3


/* device common STATUS codes */
#define UNINSTALLED	0
#define READY		1
#define BUSY		3

/* device common COMMAND codes */
#define RESET		0
#define ACK		1

/* operations */
#define	MIN(A,B)	((A) < (B) ? A : B)
#define MAX(A,B)	((A) < (B) ? B : A)
#define	ALIGNED(A)	(((unsigned)A & 0x3) == 0)

/* Useful operations */
#define STCK(T) ((T) = ((* ((cpu_t *) TODLOADDR)) / (* ((cpu_t *) TIMESCALEADDR))))

#define INT_NEW 0x2000008c
#define INT_OLD 0x20000000
#define TLB_NEW 0x200001a4
#define TLB_OLD 0x20000118
#define PGMTRAP_NEW 0x200002bc
#define PGMTRAP_OLD 0x20000230
#define SYS_NEW 0x200003d4
#define SYS_OLD 0x20000348

#define DEVICE_CNT 6
#define DEVICE_LINE 8



#define ALLOFF 0x00000000
//global interrupt enable bit
#define IEc 0x00000001
//The “current” kernel-mode user-mode control bit.
#define KUc 0x00000002
//the “previous” settings of the Status.IEc and Status.KUc
#define IEp 0x00000004
#define KUp 0x00000008
//the “previous” settings of the Status.IEp and Status.KUp
#define IEo 0x00000010
#define KUo 0x00000020
//Interrupt Mask. An 8-bit mask that enables/disables external interrupts.
#define IM  0x0000ff00
//The Bootstrap Exception Vector. This bit determines the starting address for the exception vectors.
#define BEV 0x00400000
//The “current” VM on/off flag bit. Status.VMc=0 indicates that virtual memory translation is currently off.
#define VMc 0x01000000
//the “previous” setting of the Status.VMc bit
#define VMp 0x02000000
//the “previous” setting of the Status.VMp bit
#define VMo 0x04000000
//the processor Local Timer enable bit. A 1-bit mask that enables disables the processor’s Local Timer.
#define TE  0x08000000
//a 4-bit field that controls coprocessor usability
#define CU  0x10000000

#define CREATEPROCESS 1
#define TERMINATEPROCESS 2
#define VERHOGEN 3
#define PASSEREN 4
#define EXCEPTION 5
#define GETCPUTIME 6
#define WAITFORCLOCK 7
#define WAITFORIODEVICE 8

#endif
