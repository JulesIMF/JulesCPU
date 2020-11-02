;
;JulesIMF disassemler, ver. 20201030
;
;Created:	Mon Nov  2 03:29:47 2020
;Binary: 	"equation.jbc"
;
;Set stack size
@4000


ENTRY_POINT:
	in 
	in 
	in 
	pop rcx  
	pop rbx  
	pop rax  
	fmul rax  rax  
	cmp rsp  +0x3EB0C6F7A0B5ED8D 
	pop rsp  
	jl LABEL_0x2C6
	call LABEL_0x94
	mov [+0xEDA] rdx  
	pusha 
	call LABEL_0x129
	cmp rax  +0x0 
	popa 
	je LABEL_0x201
	jl LABEL_0x195
	jg LABEL_0x1A6
	hlt 

LABEL_0x94:
	fmul rbx  rbx  
	fmul rax  rcx  
	fmul rsp  +0x4010000000000000 
	pop rdx  
	pop rsp  
	fsub rsp  rdx  
	pop rdx  
	pop rsp  
	mov rdx  [+0xEDA] 
	mov +0x0 rdx  
	ret 

LABEL_0x129:
	fcmp rdx  +0x3F1A36E2EB1C432D 
	jg LABEL_0x15C
	fcmp rdx  +0x0 
	jl LABEL_0x182
	jmp LABEL_0x16F

LABEL_0x15C:
	mov +0x0 rax  
	ret 

LABEL_0x16F:
	mov +0x1 rax  
	ret 

LABEL_0x182:
	mov +0xFFFFFFFFFFFFFFFF rax  
	ret 

LABEL_0x195:
	push +0x7FF8000000000000 
	out lf
	jmp LABEL_0x36F

LABEL_0x1A6:
	fmul +0x4000000000000000 rax  
	fsub +0x0 rbx  
	pop rbx  
	fdiv rbx  rsp  
	out lf
	pop rsp  
	pop rsp  
	jmp LABEL_0x36F

LABEL_0x201:
	fmul +0x4000000000000000 rax  
	fsub +0x0 rbx  
	pop rbx  
	fdiv rbx  rsp  
	pop rbx  
	sqrt rdx  
	pop rdx  
	fdiv rdx  rsp  
	pop rdx  
	fadd rbx  rdx  
	out lf
	fsub rbx  rdx  
	out lf
	pop rsp  
	pop rsp  
	pop rsp  
	jmp LABEL_0x36F

LABEL_0x2C6:
	fmul rbx  rbx  
	cmp rsp  +0x3E7AD7F29ABCAF48 
	pop rsp  
	jl LABEL_0x324
	fsub +0x0 rcx  
	fdiv rsp  rbx  
	out lf
	jmp LABEL_0x36F

LABEL_0x324:
	fmul rcx  rcx  
	cmp rsp  +0x3E7AD7F29ABCAF48 
	jl LABEL_0x35E
	push +0x7FF8000000000000 
	out lf
	jmp LABEL_0x36F

LABEL_0x35E:
	push +0x7FF0000000000000 
	out lf
	jmp LABEL_0x36F

LABEL_0x36F:
	hlt 
