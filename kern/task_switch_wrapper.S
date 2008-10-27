

#define INTEGER_SIZE 4
#define TADDR_OFFSET 2
#define EFL_OFFSET 3
#define CS_OFFSET 4
#define SS_OFFSET 5
#define DS_OFFSET 6
	
	

.globl do_task_switch

do_task_switch:
	movl $TADDR_OFFSET, %ebx
	movl (%ebp, %ebx, INTEGER_SIZE), %eax
	pushl %eax
	
	movl $EFL_OFFSET, %ebx
	movl (%ebp, %ebx, INTEGER_SIZE), %eax
	pushl %eax

	movl $CS_OFFSET, %ebx
	movl (%ebp, %ebx, INTEGER_SIZE), %eax

	movl $CS_OFFSET, 