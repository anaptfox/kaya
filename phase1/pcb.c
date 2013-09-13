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

int pcb_t *emptyProQ (pcb_t *tp){
	return ( tp == null); 
}
