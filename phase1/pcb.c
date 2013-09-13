#include "pcb.e"
/* process control block type */
typedef struct pcb_t {
	/* process queue fields */
	struct pcb_t *p_next, /* pointer to next entry */
	struct pcb_t *p_prev, /* pointer to previous  entry */
	/* process tree fields */
		*p_prnt, /* pointer to parent */
		*p_child, /* pointer to 1st child */
		*p_sib; /* pointer to sibling */
		*p_prev_sib; /* pointer to previous sibling */
	
	state_t p_s; /* processor state */
	
	int *p_semAdd; /* pointer to sema4 on */
	/* which process blocked */
	/* plus other entries to be added later */
} pcb_t;

pcb_t freePcb_tp;

void initPcbs(){
	static pcb_t pcbs[max];
	pcb_t free = makeEmptyProcQ();
	int i = 0;
	while( i < 0){
		freePcb(&pcbs[i]);
		i++; 
	}
	return 0;
}

/* Insert the element pointed to by p onto the pcbFree list. */
void freePcb(pcb_t *p){
	insertProcQ(&freePcb_tp, p)
}

/* Return NULL if the pcbFree list is empty. Otherwise, remove
an element from the pcbFree list, provide initial values for ALL
of the ProcBlk’s ﬁelds (i.e. NULL and/or 0) and then return a
pointer to the removed element. ProcBlk’s get reused, so it is
important that no previous value persist in a ProcBlk when it
gets reallocated. */
pcb_t *allocPcb(){
	pcb_t *temp;
	temp = removeProcQ(&freePcb_tp);
	//null evertying
	return temp;

}

/* This method is used to initialize a variable to be tail pointer to a
process queue.
Return a pointer to the tail of an empty process queue; i.e. NULL. */
pcb_t makeEmptyProcQ() {
	return(null);
}

/* Return TRUE if the queue whose tail is pointed to by tp is empty.
Return FALSE otherwise. */
int emptyProcQ (pcb_t *tp){
	return (tp == null); 
}

/* Insert the ProcBlk pointed to by p into the process queue whose
tail-pointer is pointed to by tp. Note the double indirection through
tp to allow for the possible updating of the tail pointer as well. */
void insertProcQ( pcb_t **tp, pcb_t *p){
	if(emptyProcQ(*tp)){
		*tp = p;
	}else if(*tp->p_next == *tp){
		p->p_next = *tp;
		p->p_prev = *tp;
		*tp->p_prev = *tp;
		*tp->p_next = *tp;
		*tp = p;
	}else{
		p->p_next = *tp->p_next;
		p->p_next->p_prev = p;
		p->p_prev = *tp;
		*tp->p_next = p;
		*tp = p;
	}
	return 0;
}

/* Return a pointer to the ﬁrst ProcBlk from the process queue whose
tail is pointed to by tp. Do not remove this ProcBlkfrom the process
queue. Return NULL if the process queue is empty. */
pcb_ptr *headProcQ(pcb_t **tp){
	pcb_ptr head = NULL;
	if(tp != null) {
		head = *tp->p_next;
	}
	return(head);
}


pcb_t *removeProcQ(pcb_t **tp){
	if(emptyProcQ(*tp)){
		return(NULL);
	}else if(*tp->p_next == *tp){
		*tp = makeEmptyProcQ();
	}else{
		*tp->p_next->p_next->p_prev = *tp;
		pcb_t old = *tp-prev;
		*tp->p_next = *tp->p_next->p_next;
	}
	return 0;
}

/* Remove the ProcBlk pointed to by p from the process queue whose
tail-pointer is pointed to by tp. Update the process queue’s tail
pointer if necessary. If the desired entry is not in the indicated queue
(an error condition), return NULL; otherwise, return p. Note that p
can point to any element of the process queue. */
pcb_ptr *outProcQ(pcb_t **tp, pcb_t *p){
	if(emptyProcQ(*tp)){
		return(NULL);
	}else if(*tp->p_prev == *tp){
		if(*tp == p){
			pcb_t outproc = *tp;
			*tp = makeEmptyProcQ();
			return(outproc);
		}else{
			return(NULL);
		}
	}else{
		if(*tp == p){
			pcb_t outproc = *tp;
			*tp->p_prev->p_next = *tp->p_next;
			*tp->p_next->p_prev = *tp->p_prev;
			*tp = *tp->p_prev;
			return(outproc);
		}else{
			pcb_t index = *tp->p_next;
			while(index != *tp){
		}

		}
	}
	
}

