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
	mov dword ptr [ebp-28],1
	push 0
	push eax
	lea eax,[ebp-4]
	mov [ebp-32],eax
	pop eax
	push eax
	mov eax,[ebp-32]
	mov dword ptr [eax],0
	pop eax
	push 0
	push eax
	lea eax,[ebp-8]
	mov [ebp-36],eax
	pop eax
	push eax
	mov eax,[ebp-36]
	mov dword ptr [eax],1
	pop eax
	push 0
	push eax
	lea eax,[ebp-12]
	mov [ebp-40],eax
	pop eax
	push eax
	mov eax,[ebp-40]
	mov dword ptr [eax],2
	pop eax
	push 0
	push eax
	lea eax,[ebp-16]
	mov [ebp-44],eax
	pop eax
	push eax
	mov eax,[ebp-44]
	mov dword ptr [eax],3
	pop eax
	push 0
	push eax
	lea eax,[ebp-20]
	mov [ebp-48],eax
	pop eax
	push eax
	mov eax,[ebp-48]
	mov dword ptr [eax],4
	pop eax
label_0:
	push 0
	push eax
	lea eax,[ebp-8]
	mov [ebp-52],eax
	pop eax
	push 0
	push eax
	lea eax,[ebp-8]
	mov [ebp-56],eax
	pop eax
	push 0
	push eax
	mov eax,[ebp-56]
	mov eax,[eax]
	mov dword ptr [ebp-60],eax
	add dword ptr [ebp-60],1
	pop eax
	push ebx
	mov ebx,[ebp-60]
	push eax
	mov eax,[ebp-52]
	mov dword ptr [eax],ebx
	pop eax
	pop ebx
	push 0
	push eax
	mov eax,[ebp-28]
	mov dword ptr [ebp-64],eax
	add dword ptr [ebp-64],1
	pop eax
	push ebx
	mov ebx,[ebp-64]
	mov dword ptr [ebp-28],ebx
	pop ebx
	push eax
	mov eax,[ebp-28]
	cmp eax,5
	pop eax
	jl label_0
	push 0
	push eax
	lea eax,[ebp-20]
	mov [ebp-68],eax
	pop eax
	push 0
	push eax
	lea eax,[ebp-20]
	mov [ebp-72],eax
	pop eax
	push 0
	push edx
	push eax
	mov edx,[ebp-72]
	mov eax,[edx]
	cdq
	push ebx
	mov ebx,+2
	idiv ebx
	pop ebx
	mov [ebp-76],eax
	pop eax
	pop edx
	push ebx
	mov ebx,[ebp-76]
	push eax
	mov eax,[ebp-68]
	mov dword ptr [eax],ebx
	pop eax
	pop ebx
	push 0
	push eax
	lea eax,[ebp-4]
	mov [ebp-80],eax
	pop eax
	push eax
	mov eax,[ebp-80]
	mov eax,[eax]
	printf("%d\n",eax)
	pop eax
	push 0
	push eax
	lea eax,[ebp-8]
	mov [ebp-84],eax
	pop eax
	push eax
	mov eax,[ebp-84]
	mov eax,[eax]
	printf("%d\n",eax)
	pop eax
	push 0
	push eax
	lea eax,[ebp-12]
	mov [ebp-88],eax
	pop eax
	push eax
	mov eax,[ebp-88]
	mov eax,[eax]
	printf("%d\n",eax)
	pop eax
	push 0
	push eax
	lea eax,[ebp-16]
	mov [ebp-92],eax
	pop eax
	push eax
	mov eax,[ebp-92]
	mov eax,[eax]
	printf("%d\n",eax)
	pop eax
	push 0
	push eax
	lea eax,[ebp-20]
	mov [ebp-96],eax
	pop eax
	push eax
	mov eax,[ebp-96]
	mov eax,[eax]
	printf("%d\n",eax)
	pop eax
	push eax
	mov eax,[ebp-28]
	printf("%d\n",eax)
	pop eax
	mov esp,ebp
	pop ebp
	ret
main endp

end start
