;
;Выводит круг на экран
;


;rdx - радиус круга в квадрате
;rcx - радиус
;rax   - координата x
;rbx   - координата y

entry:


	;Позовем пользователя
	pushl RequestMessage
	mul 50 24
	push 50
	tran
	scr

	pushl Empty
	mul 50 24
	push 50
	tran

	in ;радиус круга
   	   ;нет проверок на корректность!
	pop rcx
	mul rcx rcx
	pop rdx
	mul rdx 4
	pop rdx
	mov 0 rax
	mov 0 rbx
        
        next_step:
        ;call _DumpAwait
        ;--------------
        ;dump
        ;getch
        ;--------------
        
	;call _DumpAwait
	call _IsInCircle
	;call _DumpAwait
        
        jg out_of_circle
        call _Draw

        out_of_circle:
	call _NextCoords
	jmp next_step



_IsInCircle:
	pusha

	;call _DumpAwait

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
	mul 4 rbx
	pop rbx

	add rax rbx
	pop rax
	cmp rax rdx

	;call _DumpAwait

	popa
	ret

;fnc
;Обязана очистить стек
_NextCoords:
	
	inc rax
	mul rcx 4
	cmp rax rsp
	pop rsp
	jle _NextCoords__ret
	mov 0 rax
	inc rbx
	mul rcx 2
	cmp rbx rsp
	pop rsp
	jle _NextCoords__ret
	scr
	hlt


	_NextCoords__ret:
	ret

;fnc
;принимает параметры в rax и rbx и после исполнения возвращает все как есть
;проверяет флаги
_Draw:
	pusha
	;;mul 2 rax
	;;pop rax
	;address == x + y*50
	mul rbx 50
	pop rbx
	add rbx rax
	pop rax
	mov 64 p rax
	;;inc rax
	;;mov 64 p rax
	popa
	ret


_DumpAwait:
	dump
	getch
	pop rsp
	ret

RequestMessage:
~This program draws a circle. Enter the radius.                          
Empty:
~\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0