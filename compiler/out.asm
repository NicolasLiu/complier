include \masm32\include\masm32rt.inc

.code

start:

call main
inkey
exit

p1 proc
	push ebp
	mov ebp,esp
	push [ebp+12]
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
	push ebx
	mov ebx,[eax]
	mov ebx,[ebx]
	imul ebx,[ebp-4],ebx
	pop ebx
	mov [ebp-4],ebx
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
	push [ebp+12]
	push eax
	push ebx
	push ecx
	push edx
	push edi
	push esi
	mov ecx,[ebp-4]
	cmp ecx,eax
	jg label_0
	mov ecx,[ebp-24]
	mov edx,[ebp-20]
	push eax
	mov eax,[edx]
	push ebx
	mov ebx,[ebp-4]
	mov esi,eax
	add esi,ebx
	pop eax
	pop ebx
	mov [ecx],esi
	mov [ebp-4],1
	push [ebp-4]
	push [ebp+8]
	call p2
	add esp,-4
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
	push [ebp+12]
	push 0
	push eax
	push ebx
	push ecx
	push edx
	push edi
	push esi
	mov [ebp-12],1
	push ebx
	mov ebx,[ebp-12]
	mov ecx,95
	add ecx,ebx
	pop ebx
	mov [ebp-8],ecx
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
	push eax
	mov[eax], input()
	invoke atodw, reparg([eax])
	mov [ebp-16],eax
	pop eax
	push eax
	mov[eax], input()
	invoke atodw, reparg([eax])
	mov [ebp-12],eax
	pop eax
	push ebp-16
	call p1
	add esp,-4
	push 1
	call p2
	add esp,-4
	push ebx
	mov ebx,[ebp-12]
	imul edi,[ebp-16],ebx
	pop ebx
	mov [ebp-8],edi
	mov [ebp-4],1
label_1:
	push 0
	push eax
	mov eax,[ebp-8]
	push ebx
	mov ebx,[ebp-4]
	mov [ebp-24],eax
	sub [ebp-24],ebx
	pop eax
	pop ebx
	push ebx
	mov ebx,[ebp-24]
	mov [ebp-8],ebx
	pop ebx
	dec [ebp-4]
	push eax
	mov eax,[ebp-4]
	cmp eax,[ebp-12]
	jle label_1
	push eax
	printf("%d\n",[ebp-16])
	pop eax
	printf("%d\n",)
	push eax
	printf("%d\n",[ebp-12])
	pop eax
	printf("%d\n",)
	push eax
	mov eax,[ebp-16]
	cmp eax,[ebp-12]
	jle label_2
	push [ebp-16]
	call f1
	add esp,-4
	mov [ebp-16],eax
label_2:
	push [ebp-12]
	call f1
	add esp,-4
	mov [ebp-12],eax
	printf("a1\n")
	push eax
	printf("%d\n",eax)
	pop eax
	push eax
	printf("%d\n",[ebp-12])
	pop eax
	printf("%d\n",)
	push eax
	printf("%d\n",[ebp-4])
	pop eax
	printf("%d\n",)
	push eax
	printf("%d\n",[ebp-8])
	pop eax
	printf("%d\n",)
	push 0
	push eax
	mov eax,[ebp-20]
	mov [ebp-28],eax
	pop eax
	push eax
	mov eax,[ebp-32]
	printf("%d\n",[eax])
	pop eax
	printf("%d\n",)
	push 0
	push eax
	mov eax,[ebp-24]
	mov [ebp-36],eax
	pop eax
	push eax
	mov eax,[ebp-40]
	printf("%d\n",[eax])
	pop eax
	printf("%d\n",)
	push 0
	push eax
	mov eax,[ebp-28]
	mov [ebp-44],eax
	pop eax
	push eax
	mov eax,[ebp-48]
	printf("%d\n",[eax])
	pop eax
	printf("%d\n",)
main endp

end start
