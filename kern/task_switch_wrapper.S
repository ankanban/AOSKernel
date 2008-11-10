#include <x86/cr.h>

#define INTEGER_SIZE 4
#define TADDR_OFFSET 1
#define CS_OFFSET 2
#define EFL_OFFSET 3
#define SP_OFFSET 4
#define SS_OFFSET 5
#define DS_OFFSET 6
	
	

.globl do_task_switch

do_task_switch:
	// Set the frame pointer
        movl %esp, %ebp
	// Set up args for set_esp0, to save kernel pointer
	//pushl %esp
	// XXX Actually, this should be set right at the beginning.
	//call set_esp0
	//popl %ebx
	
	// SS
	movl $SS_OFFSET, %ebx
	movl (%ebp, %ebx, INTEGER_SIZE), %eax			
	pushl %eax

	// SP
	movl $SP_OFFSET, %ebx
	movl (%ebp, %ebx, INTEGER_SIZE), %eax
	pushl %eax
	
	// EFL
	movl $EFL_OFFSET, %ebx
	movl (%ebp, %ebx, INTEGER_SIZE), %eax
	pushl %eax

	// CS
	movl $CS_OFFSET, %ebx
	movl (%ebp, %ebx, INTEGER_SIZE), %eax
	pushl %eax

	
	// EIP
	movl $TADDR_OFFSET, %ebx
	movl (%ebp, %ebx, INTEGER_SIZE), %eax
	pushl %eax
	
	// DS
	movl $DS_OFFSET, %ebx
	mov (%ebp, %ebx, INTEGER_SIZE), %ds			

	movl $DS_OFFSET, %ebx
	mov (%ebp, %ebx, INTEGER_SIZE), %es			

	movl $DS_OFFSET, %ebx
	mov (%ebp, %ebx, INTEGER_SIZE), %fs			

	movl $DS_OFFSET, %ebx
	mov (%ebp, %ebx, INTEGER_SIZE), %gs			
	
	// Switch to user-mode
	iret
