void syshandler(){
	state_t *sys_old = (state_t *) SYS_OLD;

	// get the cause from the SYS old

	switch(sys_old->reg_a0){
        case CREATEPROCESS:
   			//, an error code of -1 is placed/returned in the caller’s v0, otherwise, return the value 0 in the caller’s v0
            currentProcess->p_state.reg_v0 = createProcess((state_t *) arg1);
        break;
                                
        case TERMINATEPROCESS:        
            currentProcess->p_state.reg_v0 = terminateProcess((int) arg1);
        break;
        
        case VERHOGEN:
            Verhogen((int *) arg1);
        break;
        
        case PASSEREN:
            Passeren((int *)  arg1);
        break;
        
        case EXCEPTION:
        	//SYS5 
        break;
        
        case GETCPUTIME:
            currentProcess->p_state.reg_v0 = getCpuTime();
        break;
        
        case WAITFORCLOCK:
            waitforclock();
        break;
        
        case WAITFORIODEVICE:
            currentProcess->p_state.reg_v0 = waitForIO((int) arg1, (int) arg2, (int) arg3);
        break;
        
        default:
               //something
    }
                        
    scheduler();

}


void pgmTrapHandler(){

	//did the current job perform a sys 5 for pgmtraps?
	//if no, nuke it
	// if yes, 

}


void TLBHandler(){

}


void createProcess( state_t *state){
	pcb_t *newPcb;
	if((newPcb = allocPcb()) == NULL){
		
		state->s_v0 = -1;
		
		return -1;

	}else{
		
		processCnt++; // get a pcb, processcnt++)alloc
		
		newPcb->p_s = state->s_a1;// Copy the state pointed by a1 into the p_s of the new pct
		 	
		currentProc->currentProc_child = newPcb; // make the newpcb a child of current
		 	
		insertProcQ(&readyQue, newPcb);// put the new pcb  on the readyQue
		
		state->s_v0 = 0;
		
		return 0;
		
	}
	
}

//Syscall 2. killemAll should probably either go in terminateJob and is called here,
//or goes here and is called in terminateJob().
void terminateProcess(){
    while(!emptychild(currentProc)){
        terminateProcess(removeChild(currentProc));     
    }
    currentProc = NULL;;

}

//When this service (syscall 3) is requested, it is interpreted by the nucleus as a request to
//perform a V operation on a semaphore. (What is a V operation?)
void Verhogen(int *semaddr){

	pcb_t *p;

	semaddr++;

	if(semaddr <= 0){
		p = removeBlocked(semaddr);
		insertProcQ ( & readyQueue ,  p );
	}
	
}
//When this service (syscall 4) is requested, it is interpreted by the nucleus as a request to
//perform a P operation on a semaphore. (What is a P operation?)
void Passeren(){
	
	pcb_t *p;

	semaddr--;

	if(semaddr <= -1){
		insertBlocked (semaddr , currentProcess);
	}
}

void getCpuTime(){

}

void waitforclock(){

}

void waitForIO(){

}
