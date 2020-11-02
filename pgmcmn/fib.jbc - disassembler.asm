;
;JulesIMF disassemler, ver. 20201024
;
;Created:	Mon Oct 26 21:24:06 2020
;Binary: 	"fib.jbc"
;
;Set stack size
@4000


ENTRY_POINT:
	in
	pop rax
	call LABEL_0x27
	push rbx
	out
	pop rsp
	hlt

LABEL_0x27:
	cmp rax 0x1
	jl LABEL_0x86
	je LABEL_0x99
	dec rax
	call LABEL_0x27
	push rbx
	dec rax
	call LABEL_0x27
	add rsp rbx
	pop rbx
	pop rsp
	inc rax
	inc rax
	ret

LABEL_0x86:
	mov 0x0 rbx
	ret

LABEL_0x99:
	mov 0x1 rbx
	ret
