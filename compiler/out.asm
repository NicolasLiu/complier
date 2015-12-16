include masm32\include\masm32rt.inc
.data
	item dd 0
	value dd 0
	intFmt db '%d',0
	charFmt db '%c',0
	string_temp0 db 'BEGIN:',0
	string_temp1 db 'NEXT:',0
	string_temp2 db 'END:',0

.code

start:

call main
inkey
exit

p1 proc
	push ebp
	mov ebp,esp
	push 0
	push [ebp+12]
	push [ebp+16]
	push 0
	push eax
	push ebx
	push ecx
	push edx
	push edi
	push esi
	push ebx
	mov ebx,[ebp-8]
	mov ebx,[ebx]
	mov dword ptr [ebp-16],ebx
	pop ebx
	push ebx
	mov ebx,[ebp-12]
	mov ebx,[ebx]
	push eax
	mov eax,[ebp-8]
	mov dword ptr [eax],ebx
	pop eax
	pop ebx
	push ebx
	mov ebx,[ebp-16]
	push eax
	mov eax,[ebp-12]
	mov dword ptr [eax],ebx
	pop eax
	pop ebx
	push eax
	mov eax,3
	mov [ebp-4],eax
	pop eax
	mov eax,[ebp-4]
	push ebp
	mov ebp,[ebp+8]
	mov [ebp-16],eax
	pop ebp
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
	push 0
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
	lea eax,[ebp-4]
	pop ebp
	mov [ebp-32],eax
	pop eax
	push ebx
	mov ebx,[ebp-32]
	mov ebx,[ebx]
	mov dword ptr [ebp-4],ebx
	pop ebx
	push 0
	push eax
	push ebp
	mov ebp,[ebp+8]
	lea eax,[ebp-4]
	pop ebp
	mov [ebp-36],eax
	pop eax
	push 0
	push eax
	push ebp
	mov ebp,[ebp+8]
	lea eax,[ebp-8]
	pop ebp
	mov [ebp-40],eax
	pop eax
	push ebx
	mov ebx,[ebp-40]
	mov ebx,[ebx]
	push eax
	mov eax,[ebp-36]
	mov dword ptr [eax],ebx
	pop eax
	pop ebx
	push 0
	push eax
	push ebp
	mov ebp,[ebp+8]
	lea eax,[ebp-8]
	pop ebp
	mov [ebp-44],eax
	pop eax
	push ebx
	mov ebx,[ebp-4]
	push eax
	mov eax,[ebp-44]
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
p2 endp

main proc
	push ebp
	mov ebp,esp  
	push 0
	push 0
	push 0
	push 0
	push 0
	push eax
	lea eax,[ebp-4]
	mov [ebp-20],eax
	pop eax
	push eax
	mov eax,[ebp-20]
	mov dword ptr [eax],1
	pop eax
	push 0
	push eax
	lea eax,[ebp-8]
	mov [ebp-24],eax
	pop eax
	push eax
	mov eax,[ebp-24]
	mov dword ptr [eax],2
	pop eax
	push 0
	push eax
	lea eax,[ebp-4]
	mov [ebp-28],eax
	pop eax
	invoke crt_printf, offset string_temp0
	push eax
	mov eax,[ebp-28]
	mov eax,[eax]
	invoke crt_printf, offset intFmt,eax
	pop eax
	push 0
	push eax
	lea eax,[ebp-8]
	mov [ebp-32],eax
	pop eax
	push eax
	mov eax,[ebp-32]
	mov eax,[eax]
	invoke crt_printf, offset intFmt,eax
	pop eax
	push 0
	push eax
	lea eax,[ebp-12]
	mov [ebp-36],eax
	pop eax
	push 0
	push eax
	lea eax,[ebp-4]
	mov [ebp-40],eax
	pop eax
	push 0
	push eax
	lea eax,[ebp-8]
	mov [ebp-44],eax
	pop eax
	push [ebp-44]
	push [ebp-40]
	push ebp
	call p1
	add esp,+12
	push ebx
	mov ebx,[ebp-16]
	push eax
	mov eax,[ebp-36]
	mov dword ptr [eax],ebx
	pop eax
	pop ebx
	push 0
	push eax
	lea eax,[ebp-4]
	mov [ebp-48],eax
	pop eax
	invoke crt_printf, offset string_temp1
	push eax
	mov eax,[ebp-48]
	mov eax,[eax]
	invoke crt_printf, offset intFmt,eax
	pop eax
	push 0
	push eax
	lea eax,[ebp-8]
	mov [ebp-52],eax
	pop eax
	push eax
	mov eax,[ebp-52]
	mov eax,[eax]
	invoke crt_printf, offset intFmt,eax
	pop eax
	push 0
	push eax
	lea eax,[ebp-12]
	mov [ebp-56],eax
	pop eax
	push eax
	mov eax,[ebp-56]
	mov eax,[eax]
	invoke crt_printf, offset intFmt,eax
	pop eax
	push ebp
	call p2
	add esp,+4
	push 0
	push eax
	lea eax,[ebp-4]
	mov [ebp-60],eax
	pop eax
	invoke crt_printf, offset string_temp2
	push eax
	mov eax,[ebp-60]
	mov eax,[eax]
	invoke crt_printf, offset intFmt,eax
	pop eax
	push 0
	push eax
	lea eax,[ebp-8]
	mov [ebp-64],eax
	pop eax
	push eax
	mov eax,[ebp-64]
	mov eax,[eax]
	invoke crt_printf, offset intFmt,eax
	pop eax
	mov esp,ebp
	pop ebp
	ret
main endp

end start
