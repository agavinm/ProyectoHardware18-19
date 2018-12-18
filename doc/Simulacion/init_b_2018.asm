.text
#        ENTRY            		/*  mark the first instruction to call */
.global	start
start:
.arm /*indicates that we are using the ARM instruction set */
#------standard initial code
# --- Setup interrupt / exception vectors 
      B       Reset_Handler 
/* In this version we do not use the following handlers */
#######################################################################################################
#-----------Undefined_Handler: 
#      B       Undefined_Handler 
#----------SWI_Handler: 
#      B       SWI_Handler 
#----------Prefetch_Handler: 
#      B       Prefetch_Handler 
#----------Abort_Handler: 
#      B       Abort_Handler 
#         NOP      /* Reserved vector */ 
#----------IRQ_Handler: 
#      B       IRQ_Handler 
#----------FIQ_Handler: 
#      B       FIQ_Handler 
#######################################################################################################
# Reset Handler:
# the processor starts executing this code after system reset       
#######################################################################################################
Reset_Handler:  
#        
        MOV     sp, #0x4000      /*  set up stack pointer (r13) */ 

.extern     Main
        ldr         r5, = Main
        mov         lr, pc 
        bx          r5 


stop: 	
 		B     	stop    	/*  end of program */

#################################################################################################################		
.data



.end
#        END
