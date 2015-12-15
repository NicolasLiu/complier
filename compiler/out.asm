include masm32\include\masm32rt.inc
.data
	item dd 0
	value dd 0
	intFmt db '%d',0
	charFmt db '%c',0
	string_temp0 db 'input x: ',0
	string_temp1 db 'input y: ',0
	string_temp2 db 'x mod y = ',0
	string_temp3 db 'test recursion',0
	string_temp4 db 'input test condition',0
	string_temp5 db 'test condition',0
	string_temp6 db 'test recursion',0

.code

start:

call main
inkey
exit

testarray proc
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
	push 0
	push 0
	push eax
	push ebx
	push ecx
	push edx
	push edi
	push esi
	push ebp
	mov ebp,[ebp+8]
	mov dword ptr [ebp-16],10
	pop ebp
	jmp label_1
label_0:
	push 0
	push eax
	push ebp
	mov ebp,[ebp+8]
	mov eax,[ebp-16]
	shl eax, 2
	neg eax
	pop ebp
	lea eax,[ebp-4+eax]
	mov [ebp-72],eax
	pop eax
	push ebx
	push ebp
	mov ebp,[ebp+8]
	mov ebx,[ebp-16]
	pop ebp
	push eax
	mov eax,[ebp-72]
	mov dword ptr [eax],ebx
	pop eax
	pop ebx
	push ebp
	mov ebp,[ebp+8]
	dec dword ptr [ebp-16]
	pop ebp
	push eax
	push ebp
	mov ebp,[ebp+8]
	mov eax,[ebp-16]
	pop ebp
	cmp eax,1
	pop eax
	jge label_0
	jmp label_2
label_1:
	push eax
	push ebp
	mov ebp,[ebp+8]
	mov eax,[ebp-16]
	pop ebp
	cmp eax,1
	pop eax
	jge label_0
	jmp label_3
label_2:
	push ebp
	mov ebp,[ebp+8]
	inc dword ptr [ebp-16]
	pop ebp
label_3:
	push ebp
	mov ebp,[ebp+8]
	mov dword ptr [ebp-16],10
	pop ebp
	jmp label_5
label_4:
	push 0
	push eax
	push ebp
	mov ebp,[ebp+8]
	mov eax,[ebp-16]
	shl eax, 2
	neg eax
	pop ebp
	lea eax,[ebp-4+eax]
	mov [ebp-76],eax
	pop eax
	push 0
	push eax
	push ebp
	mov ebp,[ebp+8]
	mov eax,[ebp-4]
	pop ebp
	push ebx
	mov ebx,[ebp-76]
	mov ebx,[ebx]
	mov dword ptr [ebp-80],eax
	add dword ptr [ebp-80],ebx
	pop ebx
	pop eax
	push ebx
	mov ebx,[ebp-80]
	push ebp
	mov ebp,[ebp+8]
	mov dword ptr [ebp-4],ebx
	pop ebp
	pop ebx
	push ebp
	mov ebp,[ebp+8]
	dec dword ptr [ebp-16]
	pop ebp
	push eax
	push ebp
	mov ebp,[ebp+8]
	mov eax,[ebp-16]
	pop ebp
	cmp eax,1
	pop eax
	jge label_4
	jmp label_6
label_5:
	push eax
	push ebp
	mov ebp,[ebp+8]
	mov eax,[ebp-16]
	pop ebp
	cmp eax,1
	pop eax
	jge label_4
	jmp label_7
label_6:
	push ebp
	mov ebp,[ebp+8]
	inc dword ptr [ebp-16]
	pop ebp
label_7:
	pop esi
	pop edi
	pop edx
	pop ecx
	pop ebx
	pop eax
	mov esp,ebp
	pop ebp
	ret
testarray endp

swap proc
	push ebp
	mov ebp,esp
	push 0
	push eax
	push ebx
	push ecx
	push edx
	push edi
	push esi
	push ebx
	push ebp
	mov ebp,[ebp+8]
	mov ebx,[ebp-24]
	pop ebp
	mov dword ptr [ebp-4],ebx
	pop ebx
	push ebx
	push ebp
	mov ebp,[ebp+8]
	mov ebx,[ebp-20]
	pop ebp
	push ebp
	mov ebp,[ebp+8]
	mov dword ptr [ebp-24],ebx
	pop ebp
	pop ebx
	push ebx
	mov ebx,[ebp-4]
	push ebp
	mov ebp,[ebp+8]
	mov dword ptr [ebp-20],ebx
	pop ebp
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

fact proc
	push ebp
	mov ebp,esp
	push 0
	push eax
	push ebx
	push ecx
	push edx
	push edi
	push esi
	push eax
	push ebp
	mov ebp,[ebp+8]
	mov eax,[ebp-36]
	pop ebp
	cmp eax,1
	pop eax
	jle label_8
	push 0
	push eax
	push ebp
	mov ebp,[ebp+8]
	mov eax,[ebp-36]
	pop ebp
	mov [ebp-32],eax
	sub dword ptr [ebp-32],1
	pop eax
	push ebx
	mov ebx,[ebp-32]
	push ebp
	mov ebp,[ebp+8]
	mov dword ptr [ebp-36],ebx
	pop ebp
	pop ebx
	push 0
	push eax
	push ebp
	mov ebp,[ebp+8]
	mov eax,[ebp-36]
	pop ebp
	mov dword ptr [ebp-36],eax
	add dword ptr [ebp-36],1
	pop eax
	push 0
	push eax
	mov eax,[ebp-36]
	push ebx
	mov ebx,[ebp+0]
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
	mov eax,ebx
	mov [ebp-4],eax
	pop eax
	pop ebx
	jmp label_9
label_8:
	push eax
	mov eax,1
	mov [ebp-4],eax
	pop eax
label_9:
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
fact endp

mod2 proc
	push ebp
	mov ebp,esp
	push 0
	push [ebp+12]
	push [ebp+16]
	push eax
	push ebx
	push ecx
	push edx
	push edi
	push esi
	push 0
	push edx
	push eax
	mov edx,[ebp-8]
	mov eax,[edx]
	cdq
	push ebx
	mov ebx,[ebp-12]
	mov ebx,[ebx]
	idiv ebx
	pop ebx
	mov [ebp-40],eax
	pop eax
	pop edx
	push 0
	push eax
	mov eax,[ebp-12]
	mov eax,[eax]
	push ebx
	mov ebx,[ebp-40]
	push ecx
	mov ecx,eax
	imul ecx,ebx
	mov [ebp-44],ecx
	pop ecx
	pop ebx
	pop eax
	push 0
	push eax
	mov eax,[ebp-8]
	mov eax,[eax]
	push ebx
	mov ebx,[ebp-44]
	mov [ebp-48],eax
	sub dword ptr [ebp-48],ebx
	pop ebx
	pop eax
	push ebx
	mov ebx,[ebp-48]
	push eax
	mov eax,[ebp-8]
	mov dword ptr [eax],ebx
	pop eax
	pop ebx
	push ebx
	mov ebx,[ebp-8]
	mov ebx,[ebx]
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
mod2 endp

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
	push 0
	push 0
	mov dword ptr [ebp-16],3
	jmp label_11
label_10:
	invoke crt_printf, offset string_temp0
	push eax
	invoke crt_scanf, offset intFmt,  offset value
	mov eax,value
	mov [ebp-24],eax
	pop eax
	invoke crt_printf, offset string_temp1
	push eax
	invoke crt_scanf, offset intFmt,  offset value
	mov eax,value
	mov [ebp-20],eax
	pop eax
	push 0
	push ebp
	lea ebp,[ebp-20]
	mov [esp+4], ebp
	pop ebp
	push 0
	push ebp
	lea ebp,[ebp-24]
	mov [esp+4], ebp
	pop ebp
	push ebp
	call mod2
	add esp,+12
	mov dword ptr [ebp-24],eax
	pop ebx
	invoke crt_printf, offset string_temp2
	push eax
	mov eax,[ebp-24]
	invoke crt_printf, offset intFmt,eax
	pop eax
	dec dword ptr [ebp-16]
	push eax
	mov eax,[ebp-16]
	cmp eax,1
	pop eax
	jge label_10
	jmp label_12
label_11:
	push eax
	mov eax,[ebp-16]
	cmp eax,1
	pop eax
	jge label_10
	jmp label_13
label_12:
	inc dword ptr [ebp-16]
label_13:
	invoke crt_printf, offset string_temp3
	mov dword ptr [ebp-36],6
	push eax
	mov eax,[ebp+0]
	invoke crt_printf, offset charFmt,eax
	pop eax
	invoke crt_printf, offset string_temp4
	push eax
	invoke crt_scanf, offset intFmt,  offset value
	mov eax,value
	mov [ebp-36],eax
	pop eax
	invoke crt_printf, offset string_temp5
	push ebp
	call testarray
	add esp,+4
	invoke crt_printf, offset string_temp6
	push eax
	mov eax,[ebp-4]
	invoke crt_printf, offset intFmt,eax
	pop eax
	mov esp,ebp
	pop ebp
	ret
main endp

end start
