@4000

in
pop rax
call fib
push rbx
out
pop rsp
hlt


;call convention - индекс лежит в rax, возвращаемое значение в rbx
fib:
	cmp rax 1
	jl jl_0
	je jl_1
	dec rax
	call fib
	push rbx ;на стеке лежит нечто
	dec rax
	call fib
	add rsp rbx ; два значения
	pop rbx
	pop rsp
	inc rax
	inc rax
	ret


	jl_0:
	mov 0 rbx
	ret

	jl_1:
	mov 1 rbx
	ret
;	sdfsdf
		