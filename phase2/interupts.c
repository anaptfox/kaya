void inthandler(){
	//Interrupt Handler
	//In order to occur two things have to happen
		// Ints enable bit needs to be on
			// two tiered bit, IE bit and Int mask
				// Int mask is a 8 bit field associated with a line
				// Turn the mask to all ones
		// The IE bit needs to be on and then the bit for the corresponding line to be on
	// which device signaled the inter. ( there could be multiple but only handle one of higher proity )( handle int. on lowest number line)
	//determine lowest number
		// Pending int line
	// go to the cause register
		// while
			// look for the next bit that is on for the lowest bit

	// find out which device number it is given the line number
		// in device register area, interupt device bitmap
		// @ 0x1000.003C pg. 36 of yellow book
		// 0x1000.0040
		// 0x1000.0044
			// each one is a word 
			// go to the approaite word for that line , then use that word to fine the lowest orderd bit that's on to find the disk

	// get the device register
		// read the status
		// ack the int
			//write the ack command in the command felid
			// define an ack constant
			// something-> d_command = 1
		// the bitmap of the degivice word -1

		// V(device's sema4)
			// removedBlock, wake somebody up waiting for I/O
				// put contents of status in the v0 and put them into the readyQue

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
