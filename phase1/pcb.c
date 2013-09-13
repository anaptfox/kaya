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

void initPcbs(){
	static pcb_t foo[MAX];
	int i = 0;
	while(i < max){
		freePcb(&(foo[i]));
	}
}

/* Insert the element pointed to by p onto the pcbFree list. */
void freePcb(pcb_t *p){

}

/* Return NULL if the pcbFree list is empty. Otherwise, remove
an element from the pcbFree list, provide initial values for ALL
of the ProcBlk’s ﬁelds (i.e. NULL and/or 0) and then return a
pointer to the removed element. ProcBlk’s get reused, so it is
important that no previous value persist in a ProcBlk when it
gets reallocated. */
pcb_t *allocPcb(){

}

/* This method is used to initialize a variable to be tail pointer to a
process queue.
Return a pointer to the tail of an empty process queue; i.e. NULL. */
pcb_ptr makeEmptyProcQ() {
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
		tp = p;
	}else{
		pcb_t old_next = tp->p_next;
		tp->p_next = p;
		old_next->p_prev = p;
	}
	return 0;
}

/* Return a pointer to the ﬁrst ProcBlk from the process queue whose
tail is pointed to by tp. Do not remove this ProcBlkfrom the process
queue. Return NULL if the process queue is empty. */
pcb_ptr *headProcQ(pcb_t *tp){
	pcb_ptr head = NULL;
	if(tp != null) {
		head = tp->p_next;
	}
	return(head);
}

/* Remove the ProcBlk pointed to bypfrom the process queue whose
tail-pointer is pointed to by tp. Update the process queue’s tail
pointer if necessary. If the desired entry is not in the indicated queue
(an error condition), return NULL; otherwise, return p. Note that p
can point to any element of the process queue. */
pcb_t *removeProcQ(pcb_t **tp){
	if(tp != NULL) {
		reutrn(NULL)
	}
	tp->p_next = NULL;
	return(head);
}

/* Remove the ProcBlk pointed to bypfrom the process queue whose
tail-pointer is pointed to by tp. Update the process queue’s tail
pointer if necessary. If the desired entry is not in the indicated queue
(an error condition), return NULL; otherwise, return p. Note that p
can point to any element of the process queue. */
pcb_ptr *outProcQ(pcb_t **tp, pcb_t *p){
	pcb_Ptr head = NULL;
	if(tp != null) {
		head = tp->next;
	}
	return(head);
}

