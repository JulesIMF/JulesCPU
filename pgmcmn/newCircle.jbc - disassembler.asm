;
;JulesIMF disassemler, ver. 20201026
;
;Created:	Fri Oct 30 00:53:37 2020
;Binary: 	"newCircle.jbc"
;
;Set stack size
@2000


ENTRY_POINT:
	in
	pop rcx
	mul rcx rcx
	pop rdx
	mul rdx 0x4
	pop rdx
	mov 0x0 rax
	mov 0x0 rbx

LABEL_0x67:
	call LABEL_0x80
	jg LABEL_0x76
	call LABEL_0x1D8

LABEL_0x76:
	call LABEL_0x159
	jmp LABEL_0x67

LABEL_0x80:
	pusha
	sub rax rcx
	pop rax
	sub rax rcx
	pop rax
	mul rax rax
	pop rax
	sub rbx rcx
	pop rbx
	mul rbx rbx
	pop rbx
	mul 0x4 rbx
	pop rbx
	add rax rbx
	pop rax
	cmp rax rdx
	popa
	ret

LABEL_0x159:
	inc rax
	mul rcx 0x4
	cmp rax rsp
	pop rsp
	jle LABEL_0x1D7
	mov 0x0 rax
	inc rbx
	mul rcx 0x2
	cmp rbx rsp
	pop rsp
	jle LABEL_0x1D7
	scr
	hlt

LABEL_0x1D7:
	ret

LABEL_0x1D8:
	pusha
	mul rbx 0x32
	pop rbx
	add rbx rax
	pop rax
	mov 0x40 p rax
	popa
	ret
	dump
	getch
	pop rsp
	ret
