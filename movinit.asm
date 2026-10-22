[BITS 32]

[GLOBAL memmove]
memmove:
	push edi
	push esi

	mov edi, dword [esp + 12]
	mov esi, dword [esp + 16]
	mov ecx, dword [esp + 20]

	cmp esi, edi
	jae .skip

;; move backwards when the src is before ("below") the dst
;; (otherwise we copy in front of us)
	std
	lea esi, [esi + ecx - 1]
	lea edi, [edi + ecx - 1]
.skip:

	rep movsb

	cld
	pop esi
	pop edi
	ret


[GLOBAL memzero]
memzero:
	push edi

	mov edi, dword [esp + 8]
	mov ecx, dword [esp + 12]
	mov al, 0
	
	rep stosb
	
	pop edi
	ret
