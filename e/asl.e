#ifndef ASL
#define ASL

/************************** ASL.E ******************************
*
*  The externals declaration file for the Active Semaphore List
*    Module.
*
*  Written by Mikeyg
*/

#include "../h/const.h"
#include "../h/types.h"
#include "../e/pcb.e"

extern int insertBlocked (int *semAdd, pcb_t *p);
extern pcb_t *removeBlocked (int *semAdd);
extern pcb_t *outBlocked (pcb_t *p);
extern pcb_t *headBlocked (int *semAdd);
extern void initASL ();

/***************************************************************/

#endif
