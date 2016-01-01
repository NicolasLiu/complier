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
	add esp,-28
	push eax
	mov eax,[ebp-16]
	push ebx
	mov ebx,[ebp-12]
	mov dword ptr [ebp-24],eax
	add dword ptr [ebp-24],ebx
	pop ebx
	pop eax
	push ebx
	mov ebx,[ebp-24]
	mov dword ptr [ebp-20],ebx
	pop ebx
	push ebx
	mov ebx,[ebp-24]
	mov dword ptr [ebp-8],ebx
	pop ebx
	push eax
	mov eax,[ebp-20]
	push ebx
	mov ebx,[ebp-8]
	mov dword ptr [ebp-28],eax
	add dword ptr [ebp-28],ebx
	pop ebx
	pop eax
	push ebx
	mov ebx,[ebp-28]
	mov dword ptr [ebp-4],ebx
	pop ebx
	mov esp,ebp
	pop ebp
	ret
main endp

end start
