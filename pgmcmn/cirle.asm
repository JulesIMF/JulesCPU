;
;Выводит круг на экран
;


;rdx - радиус круга в квадрате
;rcx - радиус
;rax   - координата x
;rbx   - координата y

entry:

	in ;радиус круга
   	   ;нет проверок на корректность!
   	pop rcx
   	mul rcx rcx
   	pop rdx ;теперь то что надо
   	mov 0 rax
   	mov 0 rbx
   	;пойдем итерациями

   	mov 0 p 2000
   	
   	next_step:
   	dump
   	getch

   	;выходим когда rbx > 2rcx
   	;   
   	mul 2 rcx
   	cmp rbx rsp
   	pop rsp
   	jg out
   	

   	;cтавить или нет?
   	
   	pusha ;щас все попортим, а потом быстро исправим
   	sub rbx rcx
   	pop rbx
   	div rbx 2
   	pop rbx
   	sub rax rcx
   	pop rax 	
   	mul rax rax
   	pop rax
   	mul rbx rbx
   	pop rbx
   	add rax rbx ;(x-r)^2 + (y-r)^2
   	pop rax
   	cmp rax rdx 	
   	popa

   	jg new_line

   	;вы все еще здесь? значит надо ставить
   	pusha
   	;ставим по адресу rax + 50*rbx
   	mul rbx 50
   	pop rbx
   	add rbx rax 
   	pop rbx ;адрес
   	mov 64 p rbx
   	inc rbx
   	mov 64 p rbx
   	popa   	

   	;Перевод строки
   	new_line:
   	inc rax
   	inc rax
   	mul 4 rcx
   	cmp rax rsp
   	pop rsp
   	jle next_step
   	mov 0 rax
   	inc rbx
   	jmp next_step 




out:
	scr
	hlt
        