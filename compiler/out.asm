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
	mov eax,[ebp-20]
	mov ebx,[ebp-4]
	mov [eax],ebx
	mov eax,[ebp-20]
	mov [-4],eax
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
	mov ebx,[ebp-24]
	mov ecx,[ebp-20]
	mov eax,ecx
	add eax,[ebp-4]
	mov [ebx],eax
	mov [-4],1
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
	mov [-8],1
	mov eax,95
	add eax,[ebp-8]
	mov [0],eax
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
	mov [-8],eax
	mov [-4],1
label_1:
	mov eax,[ebp-8]
	sub eax,[ebp-4]
	mov [-8],eax
dec [ebp-4]
	mov [-16],eax
label_2:
	mov [-12],eax
	mov ebx,[ebp-20]
	mov edx,[ebp-24]
	mov esi,[ebp-28]
main endp

