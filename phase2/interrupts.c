#include "../h/const.h"
#include "../h/types.h"
#include "../e/pcb.e"
#include "../e/exceptions.e"
#include "../e/scheduler.e"

void inthandler(){
	state_t p = p->p_state->status;
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
				if(p & IEc & IM){ // 0x0000ff01
				// go to the cause register
				// while
				// look for the next bit that is on for the lowest bit
					
			// find out which device number it is given the line number
			// in device register area, interrupt device bitmap
			if(devRegArea == 0x1000.003C) {//line 3 word 0
				}
			if(devRegArea == 0x1000.0040) { //line 4 word 1
				}
			if(devRegArea == 0x1000.0044) { //line 5 word 2
				}
			if(devRegArea == 0x1000.0048) { //line 6 word 3
				}
			if(devRegArea == 0x1000.004C) { // line 7 word 4
				}
			// each one is a word 
			// go to the appropriate word for that line , then use that word to fine the lowest orderd bit that's on to find the disk

				
		
	// get the device register
		// read the status
		// ack the int
			//write the ack command in the command field
			// define an ack constant
			// something-> d_command = 1
		// the bitmap of the degivice word -1

		// V(device's sema4)
			// removedBlock, wake somebody up waiting for I/O
				// put contents of status in the v0 and put them into the readyQue}

}

