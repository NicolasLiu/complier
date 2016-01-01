include masm32\include\masm32rt.inc
.data
	item dd 0
	value dd 0
	intFmt db ' %d',0
	charFmt db ' %c',0

.code

start:

call main
inkey
exit

main proc
	push ebp
	mov ebp,esp  
	mov esp,ebp
	add esp,-40
	push eax
	mov eax,[ebp-24]
	push ebx
	mov ebx,[ebp-20]
	mov dword ptr [ebp-32],eax
	add dword ptr [ebp-32],ebx
	pop ebx
	pop eax
	push ebx
	mov ebx,[ebp-32]
	mov dword ptr [ebp-28],ebx
	pop ebx
	push ebx
	mov ebx,[ebp-28]
	mov dword ptr [ebp-16],ebx
	pop ebx
	push eax
	mov eax,[ebp-24]
	push ebx
	mov ebx,[ebp-28]
	mov dword ptr [ebp-36],eax
	add dword ptr [ebp-36],ebx
	pop ebx
	pop eax
	push ebx
	mov ebx,[ebp-36]
	mov dword ptr [ebp-12],ebx
	pop ebx
	push eax
	mov eax,[ebp-24]
	push ebx
	mov ebx,[ebp-16]
	mov dword ptr [ebp-40],eax
	add dword ptr [ebp-40],ebx
	pop ebx
	pop eax
	push ebx
	mov ebx,[ebp-40]
	mov dword ptr [ebp-8],ebx
	pop ebx
	mov esp,ebp
	pop ebp
	ret
main endp

end start
