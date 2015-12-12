include masm32\include\masm32rt.inc
.data?
	value dd ?
.data
	item dd 0

.code

start:

call main
inkey
exit

sort proc
	push ebp
	mov ebp,esp
	push 0
	push 0
	push 0
	push eax
	push ebx
	push ecx
	push edx
	push edi
	push esi
	mov dword ptr [ebp-12],0
	jmp label_1
label_0:
	push 0
	push eax
	mov eax,[ebp-12]
	mov dword ptr [ebp-40],eax
	add dword ptr [ebp-40],1
	pop eax
	push ebx
	mov ebx,[ebp-40]
	mov dword ptr [ebp-8],ebx
	pop ebx
	jmp label_5
label_4:
	push 0
	push eax
	mov eax,[ebp-12]
	shl eax, 2
	neg eax
	push ebp
	mov ebp,[ebp+8]
	lea eax,[ebp-4+eax]
	pop ebp
	mov [ebp-44],eax
	pop eax
	push 0
	push eax
	mov eax,[ebp-8]
	shl eax, 2
	neg eax
	push ebp
	mov ebp,[ebp+8]
	lea eax,[ebp-4+eax]
	pop ebp
	mov [ebp-48],eax
	pop eax
	push eax
	mov eax,[ebp-44]
	mov eax,[eax]
	push ebx
	mov ebx,[ebp-48]
	mov ebx,[ebx]
	cmp eax,ebx
	pop ebx
	pop eax
	jle label_8
	push 0
	push eax
	mov eax,[ebp-12]
	shl eax, 2
	neg eax
	push ebp
	mov ebp,[ebp+8]
	lea eax,[ebp-4+eax]
	pop ebp
	mov [ebp-52],eax
	pop eax
	push ebx
	mov ebx,[ebp-52]
	mov ebx,[ebx]
	mov dword ptr [ebp-4],ebx
	pop ebx
	push 0
	push eax
	mov eax,[ebp-12]
	shl eax, 2
	neg eax
	push ebp
	mov ebp,[ebp+8]
	lea eax,[ebp-4+eax]
	pop ebp
	mov [ebp-56],eax
	pop eax
	push 0
	push eax
	mov eax,[ebp-8]
	shl eax, 2
	neg eax
	push ebp
	mov ebp,[ebp+8]
	lea eax,[ebp-4+eax]
	pop ebp
	mov [ebp-60],eax
	pop eax
	push ebx
	mov ebx,[ebp-60]
	mov ebx,[ebx]
	push eax
	mov eax,[ebp-56]
	mov dword ptr [eax],ebx
	pop eax
	pop ebx
	push 0
	push eax
	mov eax,[ebp-8]
	shl eax, 2
	neg eax
	push ebp
	mov ebp,[ebp+8]
	lea eax,[ebp-4+eax]
	pop ebp
	mov [ebp-64],eax
	pop eax
	push ebx
	mov ebx,[ebp-4]
	push eax
	mov eax,[ebp-64]
	mov dword ptr [eax],ebx
	pop eax
	pop ebx
label_8:
	inc dword ptr [ebp-8]
	push eax
	mov eax,[ebp-8]
	cmp eax,4
	pop eax
	jle label_4
	jmp label_6
label_5:
	push eax
	mov eax,[ebp-8]
	cmp eax,4
	pop eax
	jle label_4
	jmp label_7
label_6:
	dec dword ptr [ebp-8]
label_7:
	inc dword ptr [ebp-12]
	push eax
	mov eax,[ebp-12]
	cmp eax,3
	pop eax
	jle label_0
	jmp label_2
label_1:
	push eax
	mov eax,[ebp-12]
	cmp eax,3
	pop eax
	jle label_0
	jmp label_3
label_2:
	dec dword ptr [ebp-12]
label_3:
	pop esi
	pop edi
	pop edx
	pop ecx
	pop ebx
	pop eax
	mov esp,ebp
	pop ebp
	ret
sort endp

main proc
	push ebp
	mov ebp,esp  
	push 0
	push 0
	push 0
	push 0
	push 0
	push 0
	push 0
	push 0
	push eax
	lea eax,[ebp-4]
	mov [ebp-32],eax
	pop eax
	push eax
	mov eax,[ebp-32]
	mov dword ptr [eax],2
	pop eax
	push 0
	push eax
	lea eax,[ebp-8]
	mov [ebp-36],eax
	pop eax
	push eax
	mov eax,[ebp-36]
	mov dword ptr [eax],3
	pop eax
	push 0
	push eax
	lea eax,[ebp-12]
	mov [ebp-40],eax
	pop eax
	push eax
	mov eax,[ebp-40]
	mov dword ptr [eax],1
	pop eax
	push 0
	push eax
	lea eax,[ebp-16]
	mov [ebp-44],eax
	pop eax
	push eax
	mov eax,[ebp-44]
	mov dword ptr [eax],4
	pop eax
	push 0
	push eax
	lea eax,[ebp-20]
	mov [ebp-48],eax
	pop eax
	push eax
	mov eax,[ebp-48]
	mov dword ptr [eax],5
	pop eax
	push ebp
	call sort
	add esp,+4
	push 0
	push eax
	lea eax,[ebp-4]
	mov [ebp-52],eax
	pop eax
	push eax
	mov eax,[ebp-52]
	mov eax,[eax]
	printf("%d\n",eax)
	pop eax
	push 0
	push eax
	lea eax,[ebp-8]
	mov [ebp-56],eax
	pop eax
	push eax
	mov eax,[ebp-56]
	mov eax,[eax]
	printf("%d\n",eax)
	pop eax
	push 0
	push eax
	lea eax,[ebp-12]
	mov [ebp-60],eax
	pop eax
	push eax
	mov eax,[ebp-60]
	mov eax,[eax]
	printf("%d\n",eax)
	pop eax
	push 0
	push eax
	lea eax,[ebp-16]
	mov [ebp-64],eax
	pop eax
	push eax
	mov eax,[ebp-64]
	mov eax,[eax]
	printf("%d\n",eax)
	pop eax
	push 0
	push eax
	lea eax,[ebp-20]
	mov [ebp-68],eax
	pop eax
	push eax
	mov eax,[ebp-68]
	mov eax,[eax]
	printf("%d\n",eax)
	pop eax
	mov esp,ebp
	pop ebp
	ret
main endp

end start
