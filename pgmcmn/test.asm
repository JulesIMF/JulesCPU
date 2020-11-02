in
mov rsp rax
mov 0 rbx

label:
inc rbx
push rbx
out
pop rsp
cmp rbx rax
jne label
hlt