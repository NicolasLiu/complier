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
	push 0
	push eax
	push ebp
	mov ebp,[ebp+8]
	lea eax,[ebp-20]
	pop ebp
	mov [ebp-32],eax
	pop eax
	push ebx
	mov ebx,[ebp-4]
	mov ebx,[ebx]
	push eax
	mov eax,[ebp-32]
	mov dword ptr [eax],ebx
	pop eax
	pop ebx
	push 0
	push eax
	push ebp
	mov ebp,[ebp+8]
	lea eax,[ebp-20]
	pop ebp
	mov [ebp-36],eax
	pop eax
	push 0
	push eax
	mov eax,[ebp-4]
	mov eax,[eax]
	push ebx
	mov ebx,[ebp-36]
	mov ebx,[ebx]
	push ecx
	mov ecx,eax
	imul ecx,ebx
	mov [ebp-40],ecx
	pop ecx
	pop ebx
	pop eax
	push ebx
	mov ebx,[ebp-40]
	push eax
	mov eax,[ebp-4]
	mov dword ptr [eax],ebx
	pop eax
	pop ebx
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
	push eax
	mov eax,[ebp-4]
	cmp eax,3
	pop eax
	jg label_0
	push 0
	push eax
	push ebp
	mov ebp,[ebp+8]
	lea eax,[ebp-24]
	pop ebp
	mov [ebp-32],eax
	pop eax
	push 0
	push eax
	push ebp
	mov ebp,[ebp+8]
	lea eax,[ebp-20]
	pop ebp
	mov [ebp-36],eax
	pop eax
	push 0
	push eax
	mov eax,[ebp-36]
	mov eax,[eax]
	push ebx
	mov ebx,[ebp-4]
	mov dword ptr [ebp-40],eax
	add dword ptr [ebp-40],ebx
	pop ebx
	pop eax
	push ebx
	mov ebx,[ebp-40]
	push eax
	mov eax,[ebp-32]
	mov dword ptr [eax],ebx
	pop eax
	pop ebx
	push 0
	push eax
	mov eax,[ebp-4]
	mov dword ptr [ebp-44],eax
	add dword ptr [ebp-44],1
	pop eax
	push ebx
	mov ebx,[ebp-44]
	mov dword ptr [ebp-4],ebx
	pop ebx
	push [ebp-4]
	push [ebp+8]
	call p2
	add esp,+8
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
	push 0
	push [ebp+12]
	push 0
	push eax
	push ebx
	push ecx
	push edx
	push edi
	push esi
	push 0
	push eax
	mov eax,[ebp-8]
	mov dword ptr [ebp-40],eax
	add dword ptr [ebp-40],1
	pop eax
	push ebx
	mov ebx,[ebp-40]
	mov dword ptr [ebp-12],ebx
	pop ebx
	push 0
	push ebx
	mov ebx,[ebp-12]
	mov dword ptr [ebp-44],95
	add dword ptr [ebp-44],ebx
	pop ebx
	push ebx
	mov ebx,[ebp-44]
	push eax
	mov eax,ebx
	mov [ebp-4],eax
	pop eax
	pop ebx
	pop esi
	pop edi
	pop edx
	pop ecx
	pop ebx
	pop eax
	mov esp,ebp
	mov eax,[ebp-4]
	pop ebp
	ret
f1 endp

main proc
	cls
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
	push 0
	push eax
	mov value, input()
	invoke atodw, reparg(value)
	mov [ebp-16],eax
	pop eax
	push eax
	mov value, input()
	invoke atodw, reparg(value)
	mov [ebp-12],eax
	pop eax
	push 0
	push ebp
	lea ebp,[ebp-16]
	mov [esp+4], ebp
	pop ebp
	push ebp
	call p1
	add esp,+8
	push 1
	push ebp
	call p2
	add esp,+8
	push 0
	push eax
	mov eax,[ebp-16]
	push ebx
	mov ebx,[ebp-12]
	push ecx
	mov ecx,eax
	imul ecx,ebx
	mov [ebp-44],ecx
	pop ecx
	pop ebx
	pop eax
	push ebx
	mov ebx,[ebp-44]
	mov dword ptr [ebp-8],ebx
	pop ebx
	mov dword ptr [ebp-4],1
	jmp label_2
label_1:
	push 0
	push eax
	mov eax,[ebp-8]
	push ebx
	mov ebx,[ebp-4]
	mov [ebp-48],eax
	sub dword ptr [ebp-48],ebx
	pop ebx
	pop eax
	push ebx
	mov ebx,[ebp-48]
	mov dword ptr [ebp-8],ebx
	pop ebx
	inc dword ptr [ebp-4]
	push eax
	mov eax,[ebp-4]
	push ebx
	mov ebx,[ebp-12]
	cmp eax,ebx
	pop ebx
	pop eax
	jle label_1
	jmp label_3
label_2:
	push eax
	mov eax,[ebp-4]
	push ebx
	mov ebx,[ebp-12]
	cmp eax,ebx
	pop ebx
	pop eax
	jle label_1
	jmp label_4
label_3:
	dec dword ptr [ebp-4]
label_4:
	push eax
	mov eax,[ebp-16]
	printf("%d\n",eax)
	pop eax
	push eax
	mov eax,[ebp-12]
	printf("%d\n",eax)
	pop eax
	push eax
	mov eax,[ebp-16]
	push ebx
	mov ebx,[ebp-12]
	cmp eax,ebx
	pop ebx
	pop eax
	jle label_5
	push [ebp-16]
	push ebp
	call f1
	add esp,+8
	mov dword ptr [ebp-16],eax
	pop ebx
	jmp label_6
label_5:
	push [ebp-12]
	push ebp
	call f1
	add esp,+8
	mov dword ptr [ebp-12],eax
	pop ebx
label_6:
	printf("0123\n")
	push eax
	mov eax,[ebp-16]
	printf("%d\n",eax)
	pop eax
	push eax
	mov eax,[ebp-12]
	printf("%d\n",eax)
	pop eax
	push eax
	mov eax,[ebp-4]
	printf("%d\n",eax)
	pop eax
	push eax
	mov eax,[ebp-8]
	printf("%d\n",eax)
	pop eax
	push 0
	push eax
	lea eax,[ebp-20]
	mov [ebp-52],eax
	pop eax
	push eax
	mov eax,[ebp-52]
	mov eax,[eax]
	printf("%d\n",eax)
	pop eax
	push 0
	push eax
	lea eax,[ebp-24]
	mov [ebp-56],eax
	pop eax
	push eax
	mov eax,[ebp-56]
	mov eax,[eax]
	printf("%d\n",eax)
	pop eax
	push 0
	push eax
	lea eax,[ebp-28]
	mov [ebp-60],eax
	pop eax
	push eax
	mov eax,[ebp-60]
	mov eax,[eax]
	printf("%d\n",eax)
	pop eax
	mov esp,ebp
	pop ebp
	ret
main endp

end start
