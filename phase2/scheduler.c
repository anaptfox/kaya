

void syshandler(){

}

void inthandler(){

}

void pgmTrapHandler(){

	//did the current job perform a sys 5 for pgmtraps?
	//if no, nuke it
	// if yes, 

}

void TLSHandler(){

}

void createNewJob(){
	// get a pcb, processcnt++
	// Copy the state pointed by a1 into the p_s of the new pct
	 	
	// mae the newpcb a child of current
	 	
	// put the new pcb  on the readyQUe
	//LDSt(&oldsys)
}

void terminateJob(){
	// kill current job and all its progeny
	//recursivly travel tree and kill everything (freepcb)
	//everytime you call freepcb call procCnt--
}