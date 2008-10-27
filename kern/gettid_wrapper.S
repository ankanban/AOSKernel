
.globl gettid_wrapper

gettid_wrapper:
	call sys_gettid
	iret