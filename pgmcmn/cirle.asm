;
;������� ���� �� �����
;


;rdx - ������ ����� � ��������
;rcx - ������
;rax   - ���������� x
;rbx   - ���������� y

entry:

	in ;������ �����
   	   ;��� �������� �� ������������!
   	pop rcx
   	mul rcx rcx
   	pop rdx ;������ �� ��� ����
   	mov 0 rax
   	mov 0 rbx
   	;������ ����������

   	mov 0 p 2000
   	
   	next_step:
   	dump
   	getch

   	;������� ����� rbx > 2rcx
   	;   
   	mul 2 rcx
   	cmp rbx rsp
   	pop rsp
   	jg out
   	

   	;c������ ��� ���?
   	
   	pusha ;��� ��� ��������, � ����� ������ ��������
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

   	;�� ��� ��� �����? ������ ���� �������
   	pusha
   	;������ �� ������ rax + 50*rbx
   	mul rbx 50
   	pop rbx
   	add rbx rax 
   	pop rbx ;�����
   	mov 64 p rbx
   	inc rbx
   	mov 64 p rbx
   	popa   	

   	;������� ������
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
        