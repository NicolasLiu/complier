include masm32\include\masm32rt.inc
.data
	item dd 0
	value dd 0
	intFmt db ' %d',0
	charFmt db ' %c',0
	string_temp0 db ',',0

.code

start:

call main
inkey
exit

swap proc
	push ebp
	mov ebp,esp
	push [ebp+12]
	push [ebp+16]
	mov esp,ebp
	add esp,-12
	push eax
	push ebx
	push ecx
	push edx
	push edi
	push esi
	push ebx
	mov ebx,[ebp-4]
	mov ebx,[ebx]
	mov dword ptr [ebp-12],ebx
	pop ebx
	push ebx
	mov ebx,[ebp-8]
	mov ebx,[ebx]
	push eax
	mov eax,[ebp-4]
	mov dword ptr [eax],ebx
	pop eax
	pop ebx
	push ebx
	mov ebx,[ebp-12]
	push eax
	mov eax,[ebp-8]
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
swap endp

p proc
	push ebp
	mov ebp,esp
	push [ebp+12]
	push [ebp+16]
	mov esp,ebp
	add esp,-76
	push eax
	push ebx
	push ecx
	push edx
	push edi
	push esi
	push ebx
	mov ebx,[ebp-4]
	mov dword ptr [ebp-24],ebx
	pop ebx
	push ebx
	mov ebx,[ebp-8]
	mov dword ptr [ebp-16],ebx
	pop ebx
	push eax
	mov eax,[ebp-24]
	push ebx
	mov ebx,[ebp-16]
	mov dword ptr [ebp-28],eax
	add dword ptr [ebp-28],ebx
	pop ebx
	pop eax
	push edx
	push eax
	mov eax,[ebp-28]
	cdq
	push ebx
	mov ebx,+2
	idiv ebx
	pop ebx
	mov [ebp-32],eax
	pop eax
	pop edx
	push eax
	mov eax,[ebp-32]
	shl eax, 2
	neg eax
	push ebp
	mov ebp,[ebp+8]
	lea eax,[ebp-12+eax]
	pop ebp
	mov [ebp-36],eax
	pop eax
	push ebx
	mov ebx,[ebp-36]
	mov ebx,[ebx]
	mov dword ptr [ebp-12],ebx
	pop ebx
label_0:
	push eax
	mov eax,[ebp-16]
	push ebx
	mov ebx,[ebp-24]
	cmp eax,ebx
	pop ebx
	pop eax
	jl label_1
label_2:
	push eax
	mov eax,[ebp-24]
	shl eax, 2
	neg eax
	push ebp
	mov ebp,[ebp+8]
	lea eax,[ebp-12+eax]
	pop ebp
	mov [ebp-40],eax
	pop eax
	push eax
	mov eax,[ebp-40]
	mov eax,[eax]
	push ebx
	mov ebx,[ebp-12]
	cmp eax,ebx
	pop ebx
	pop eax
	jge label_3
	push eax
	mov eax,[ebp-24]
	mov dword ptr [ebp-44],eax
	add dword ptr [ebp-44],1
	pop eax
	push eax
	mov eax,[ebp-44]
	push ecx
	mov ecx,eax
	imul ecx,2
	mov [ebp-48],ecx
	pop ecx
	pop eax
	push eax
	mov eax,[ebp-48]
	push ebx
	mov ebx,[ebp-44]
	mov [ebp-52],eax
	sub dword ptr [ebp-52],ebx
	pop ebx
	pop eax
	push ebx
	mov ebx,[ebp-52]
	mov dword ptr [ebp-24],ebx
	pop ebx
	jmp label_2
label_3:
label_4:
	push eax
	mov eax,[ebp-16]
	shl eax, 2
	neg eax
	push ebp
	mov ebp,[ebp+8]
	lea eax,[ebp-12+eax]
	pop ebp
	mov [ebp-56],eax
	pop eax
	push eax
	mov eax,[ebp-56]
	mov eax,[eax]
	push ebx
	mov ebx,[ebp-12]
	cmp eax,ebx
	pop ebx
	pop eax
	jle label_5
	push eax
	mov eax,[ebp-16]
	mov [ebp-60],eax
	sub dword ptr [ebp-60],1
	pop eax
	push ebx
	mov ebx,[ebp-60]
	mov dword ptr [ebp-16],ebx
	pop ebx
	jmp label_4
label_5:
	push eax
	mov eax,[ebp-24]
	push ebx
	mov ebx,[ebp-16]
	cmp eax,ebx
	pop ebx
	pop eax
	jg label_6
	push eax
	mov eax,[ebp-24]
	shl eax, 2
	neg eax
	push ebp
	mov ebp,[ebp+8]
	lea eax,[ebp-12+eax]
	pop ebp
	mov [ebp-64],eax
	pop eax
	push eax
	mov eax,[ebp-16]
	shl eax, 2
	neg eax
	push ebp
	mov ebp,[ebp+8]
	lea eax,[ebp-12+eax]
	pop ebp
	mov [ebp-68],eax
	pop eax
	push [ebp-68]
	push [ebp-64]
	push [ebp+8]
	call swap
	add esp,+12
	push eax
	mov eax,[ebp-24]
	mov dword ptr [ebp-72],eax
	add dword ptr [ebp-72],1
	pop eax
	push ebx
	mov ebx,[ebp-72]
	mov dword ptr [ebp-24],ebx
	pop ebx
	push eax
	mov eax,[ebp-16]
	mov [ebp-76],eax
	sub dword ptr [ebp-76],1
	pop eax
	push ebx
	mov ebx,[ebp-76]
	mov dword ptr [ebp-16],ebx
	pop ebx
label_6:
	jmp label_0
label_1:
	push eax
	mov eax,[ebp-4]
	push ebx
	mov ebx,[ebp-16]
	cmp eax,ebx
	pop ebx
	pop eax
	jge label_7
	push [ebp-16]
	push [ebp-4]
	push [ebp+8]
	call p
	add esp,+12
label_7:
	push eax
	mov eax,[ebp-24]
	push ebx
	mov ebx,[ebp-8]
	cmp eax,ebx
	pop ebx
	pop eax
	jge label_8
	push [ebp-8]
	push [ebp-24]
	push [ebp+8]
	call p
	add esp,+12
label_8:
	pop esi
	pop edi
	pop edx
	pop ecx
	pop ebx
	pop eax
	mov esp,ebp
	pop ebp
	ret
p endp

main proc
	push ebp
	mov ebp,esp  
	mov esp,ebp
	add esp,-72
	push eax
	push eax
	print "input an integer : "
	pop eax
	push offset value
	push offset intFmt
	call crt_scanf
	add esp,+8
	mov eax,value
	mov [ebp-8],eax
	pop eax
	push ebx
	mov ebx,[ebp-8]
	mov dword ptr [ebp-64],ebx
	pop ebx
	jmp label_10
label_9:
	push eax
	push eax
	print "input an integer : "
	pop eax
	push offset value
	push offset intFmt
	call crt_scanf
	add esp,+8
	mov eax,value
	mov [ebp-4],eax
	pop eax
	push eax
	mov eax,[ebp-64]
	shl eax, 2
	neg eax
	lea eax,[ebp-12+eax]
	mov [ebp-68],eax
	pop eax
	push ebx
	mov ebx,[ebp-4]
	push eax
	mov eax,[ebp-68]
	mov dword ptr [eax],ebx
	pop eax
	pop ebx
	dec dword ptr [ebp-64]
	push eax
	mov eax,[ebp-64]
	cmp eax,1
	pop eax
	jge label_9
	jmp label_11
label_10:
	push eax
	mov eax,[ebp-64]
	cmp eax,1
	pop eax
	jge label_9
	jmp label_12
label_11:
	inc dword ptr [ebp-64]
label_12:
	push [ebp-8]
	push +1
	push ebp
	call p
	add esp,+12
	mov dword ptr [ebp-64],1
	jmp label_14
label_13:
	push eax
	mov eax,[ebp-64]
	shl eax, 2
	neg eax
	lea eax,[ebp-12+eax]
	mov [ebp-72],eax
	pop eax
	push offset string_temp0
	call crt_printf
	add esp,+4
	push eax
	mov eax,[ebp-72]
	mov eax,[eax]
	push eax
	push offset intFmt
	call crt_printf
	add esp,+8
	pop eax
	inc dword ptr [ebp-64]
	push eax
	mov eax,[ebp-64]
	push ebx
	mov ebx,[ebp-8]
	cmp eax,ebx
	pop ebx
	pop eax
	jle label_13
	jmp label_15
label_14:
	push eax
	mov eax,[ebp-64]
	push ebx
	mov ebx,[ebp-8]
	cmp eax,ebx
	pop ebx
	pop eax
	jle label_13
	jmp label_16
label_15:
	dec dword ptr [ebp-64]
label_16:
	mov esp,ebp
	pop ebp
	ret
main endp

end start
