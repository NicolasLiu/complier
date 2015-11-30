include \masm32\include\masm32rt.inc

.code

start:

call main
inkey
exit

p1 proc
	push ebp
	mov ebp,esp
	push [ebp+8]
	push eax
	push ebx
	push ecx
	push edx
	push edi
	push esi
	pop esi
	pop edi
	pop edx
	pop ecx
	pop ebx
	pop eax
	mov esp,ebp
	pop ebp
	ret
p1 endp

p2 proc
	push ebp
	mov ebp,esp
	push [ebp+8]
	push eax
	push ebx
	push ecx
	push edx
	push edi
	push esi
label_0:
	pop esi
	pop edi
	pop edx
	pop ecx
	pop ebx
	pop eax
	mov esp,ebp
	pop ebp
	ret
p2 endp

f1 proc
	push ebp
	mov ebp,esp
	push [ebp+8]
	push 0
	push eax
	push ebx
	push ecx
	push edx
	push edi
	push esi
	pop esi
	pop edi
	pop edx
	pop ecx
	pop ebx
	pop eax
	mov esp,ebp
	pop ebp
	ret
f1 endp

main proc
	cls
	push 0
	push 0
	push 0
	push 0
	push 0
	push 0
	push 0
	push 0
	push 0
label_1:
label_2:
main endp

