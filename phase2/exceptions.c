void syshandler(){

}


void pgmTrapHandler(){

	//did the current job perform a sys 5 for pgmtraps?
	//if no, nuke it
	// if yes, 

}

void TLBHandler(){

}
//Syscall 1 basic idea, I believe
void createProcess(){
	if(free ProcBlk == 0) {
		$v0 = -1;
		return $v0;
	}
	else{
		insertProcQ(pcb_t newProcBlk $a1);
		$v0 = 0;
		return $v0;
	}
}
//Syscall 2. killemAll should probably either go in terminateJob and is called here,
//or goes here and is called in terminateJob().
void terminateProcess(){
	terminateJob(currentProc);
}

//When this service (syscall 3) is requested, it is interpreted by the nucleus as a request to
//perform a V operation on a semaphore. (What is a V operation?)
void Verhogen(){
	
}
//When this service (syscall 4) is requested, it is interpreted by the nucleus as a request to
//perform a P operation on a semaphore. (What is a P operation?)
void Passeren(){
	
}
