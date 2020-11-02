;
;JulesIMF disassemler, ver. 20201102
;
;Created:	Mon Nov  2 23:20:25 2020
;Binary: 	"videotest.jbc"
;
;Set stack size
@2000


ENTRY_POINT:
	getch 
	pushl LABEL_0x154
	push +0x7D0 
	push +0x34 
	tran 
	mov +0x7D0 rax 
	call LABEL_0x34
	scr 
	hlt 

LABEL_0x34:
	pusha 
	mov +0x0 rbx 
	mov +0x0 rdx 

LABEL_0x59:
	cmp +0x0 [rax+-0x0] 
	je LABEL_0x10D
	cmp +0xA [rax+0x0] 
	jne LABEL_0x9E
	call LABEL_0x10F
	mov +0xFFFFFFFFFFFFFFFF rbx 

LABEL_0x9E:
	cmp +0x32 rbx 
	jne LABEL_0xBA
	call LABEL_0x10F

LABEL_0xBA:
	mul +0x32 rdx 
	pop rcx 
	add rcx rbx 
	pop rcx 
	mov [rax+0x0] [rcx+0x0] 
	inc rax
	inc rbx
	jmp LABEL_0x59

LABEL_0x10D:
	popa 
	ret 

LABEL_0x10F:
	je LABEL_0x115
	ret 

LABEL_0x115:
	inc rdx
	mov +0x0 rbx 
	cmp rdx +0x19 
	jne LABEL_0x152
	mov +0x0 rdx 

LABEL_0x152:
	ret 
	
LABEL_0x154:
~Hi MIPT ECS from SPBSU MP\nHi SPBSU MP from MIPT ECS\0