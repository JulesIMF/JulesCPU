;выводит строку в обратном порядке

getch
pushl data
push 2000
push 52
tran

mov 2000 rax
call printf
;cls
scr

hlt

printf:
	;Адрес в rax
	pusha
	mov 0 rbx ;ox
	mov 0 rdx ;oy
	continue_printing:
	;debug
	;push p rax
	;putch
	;pop rsp
	;нуль-терминатор

	cmp 0 p rax
	je prn_out

	;перевод строки
	cmp 10 p rax
	jne not_10
	call newLine
	mov -1 rbx
	not_10:
	cmp 50 rbx
	jne not_endline
	call newLine
	not_endline:
	mul 50 rdx
	pop rcx
	add rcx rbx
	pop rcx
	mov p rax p rcx
	inc rax
	inc rbx
	jmp continue_printing

	prn_out:
	popa
	ret

newLine:
	je newline_continue
	ret
	newline_continue:
	inc rdx
	mov 0 rbx
	cmp rdx 25
	jne newline_out
	mov 0 rdx
	newline_out:
	ret

data:
~Hi MIPT ECS from SPBSU MP\nHi SPBSU MP from MIPT ECS\0
