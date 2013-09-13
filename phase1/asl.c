#include "asl.h"

/* semaphore descriptor type */
typedef struct semd t {
	struct semd_t *s_next; /* next element on the ASL */
	struct semd_t *s_prev; //prev element on the ASL
	int *s_semAdd; /* pointer to the semaphore*/
	pcb t *s_procQ; /* tail pointer to a process queue */
} semd t;

int insertBlocked(int *semAdd, pcb_t *p){

}

pcb_t *removedBlocked(int *semAdd){

}

pcb_t *outBlocked(pct_t *p){

}

pcb_t *headBlocked(int *semAdd){

}

void initASL{

}