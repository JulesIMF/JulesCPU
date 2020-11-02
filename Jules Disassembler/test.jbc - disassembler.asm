;
;JulesIMF disassemler, ver. 20201030
;
;Created:	Sun Nov  1 08:15:31 2020
;Binary: 	"test.jbc"
;
;Set stack size
@2000


ENTRY_POINT:
	in 
	mov rsp  rax  
	mov +0x0 rbx  

LABEL_0x25:
	inc rbx
	push rbx  
	out
	pop rsp  
	cmp rbx  rax  
	jne LABEL_0x25
	hlt 
