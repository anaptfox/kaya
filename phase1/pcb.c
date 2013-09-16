#include "../h/const.h"
#include "../h/types.h"
#include "../e/pcb.e"

static pcb_t freePcb_tp;


/* This method is used to initialize a variable to be tail pointer to a
process queue.
Return a pointer to the tail of an empty process queue; i.e. NULL. */
pcb_t *mkEmptyProcQ() {
	return(NULL);
}

/* Return TRUE if the queue whose tail is pointed to by tp is empty.
Return FALSE otherwise. */
int emptyProcQ (pcb_t *tp){
	return (tp == NULL); 
}

/* Insert the ProcBlk pointed to by p into the process queue whose
tail-pointer is pointed to by tp. Note the double indirection through
tp to allow for the possible updating of the tail pointer as well. */
void insertProcQ(pcb_t *tp, pcb_t *p){
	if(emptyProcQ(tp)){
		tp = p;
	}else if(tp->p_next == tp){
		p->p_next = tp;
		p->p_prev = tp;
		tp->p_prev = tp;
		tp->p_next = tp;
		tp = p;
	}else{
		p->p_next = tp->p_next;
		p->p_next->p_prev = p;
		p->p_prev = tp;
		tp->p_next = p;
		tp = p;
	}
	return 0;
}

/* Return a pointer to the ﬁrst ProcBlk from the process queue whose
tail is pointed to by tp. Do not remove this ProcBlkfrom the process
queue. Return NULL if the process queue is empty. */
pcb_t *headProcQ(pcb_t *tp){
	pcb_t *head = NULL;
	if(tp != NULL) {
		head = tp->p_next;
	}
	return(head);
}


pcb_t *removeProcQ(pcb_t *tp){
	if(emptyProcQ(tp)){
		return(NULL);
	}else if(tp->p_next == tp){
		tp = mkEmptyProcQ();
		return(NULL);
	}else{
		tp->p_next->p_next->p_prev = tp;
		pcb_t *old = tp->p_next;
		tp->p_next = tp->p_next->p_next;
		return(old);
	}
}

/* Remove the ProcBlk pointed to by p from the process queue whose
tail-pointer is pointed to by tp. Update the process queue’s tail
pointer if necessary. If the desired entry is not in the indicated queue
(an error condition), return NULL; otherwise, return p. Note that p
can point to any element of the process queue. */
pcb_t *outProcQ(pcb_t *tp, pcb_t *p){
	if(emptyProcQ(tp)){
		return(NULL);
	}else if(tp->p_prev == tp){
		if(tp == p){
			pcb_t *outproc = tp;
			tp = mkEmptyProcQ();
			return(outproc);
		}else{
			return(NULL);
		}
	}else{
		if(tp == p){
			pcb_t *outproc = tp;
			tp->p_prev->p_next = tp->p_next;
			tp->p_next->p_prev = tp->p_prev;
			tp = tp->p_prev;
			return(outproc);
		}else{
			pcb_t *index = tp->p_next;
			while(index != tp){
				if(p == index && index == tp->p_next){
					return removeProcQ(tp);
				}else if(p == index){
					index->p_prev->p_next = index->p_next;
					index->p_next->p_prev = index->p_prev;
					return(index);
				}else{
					index = index->p_next;
				}
			}
			return(NULL);
		}
	}
	
}


void initPcbs(){
	static pcb_t *pcbs[MAXPROC];
	freePcb_tp = NULL;
	pcb_t *free = mkEmptyProcQ();
	int i = MAXPROC;
	while( i > 0){
		freePcb(&pcbs[i]);
		i--; 
	}
	return 0;
}

/* Insert the element pointed to by p onto the pcbFree list. */
void freePcb(pcb_t *p){
	insertProcQ(&freePcb_tp, p);
}

/* Return NULL if the pcbFree list is empty. Otherwise, remove
an element from the pcbFree list, provide initial values for ALL
of the ProcBlk’s ﬁelds (i.e. NULL and/or 0) and then return a
pointer to the removed element. ProcBlk’s get reused, so it is
important that no previous value persist in a ProcBlk when it
gets reallocated. */
pcb_t *allocPcb(){
	return removeProcQ(&freePcb_tp);
}

int emptyChild (pcb_t *p){
	return (p->p_child == NULL); 
}

void insertChild (pcb_t *prnt, pcb_t *p){
	if(emptyChild(prnt)){
		prnt->p_child = p;
	}else{
		prnt->p_child->p_prev_sib = p;
		p->p_sib = prnt->p_child;
		prnt->p_child = p;
	}
}

pcb_t *removeChild (pcb_t *p){
	if(emptyChild(p)){
		return(NULL);
	}else{
		if(p->p_child->p_sib == NULL){
			p->p_child = NULL;
		}else{
			p->p_child->p_sib->p_prev_sib = NULL;
			p->p_child =  p->p_child->p_sib;
		}
	}
}

pcb_t *outChild(pcb_t *p){
	if(p->p_prnt == NULL){
		return(NULL);
	}else{
		if((p->p_sib == NULL) && (p->p_prev_sib == NULL)){
			p->p_prnt->p_child = NULL;
			p->p_prnt = NULL;
			return(p);
		}else{
			if(p->p_prev_sib == NULL){
				p->p_prnt->p_child = p->p_sib;
				p->p_sib->p_prev_sib = NULL;
				p->p_prnt = NULL;
				p->p_sib = NULL;
				return(p);
			}else if(p->p_sib == NULL){
				p->p_prev_sib->p_sib = NULL;
				p->p_prnt = NULL;
				p->p_prev_sib = NULL;
				return(p);
			}else{
				p->p_prev_sib->p_sib = p->p_sib;
				p->p_sib->p_prev_sib = p->p_prev_sib;
				p->p_prnt = NULL;
				p->p_sib = NULL;
				p->p_prev_sib = NULL;
				return(p);
			}
		}
	
	}
}