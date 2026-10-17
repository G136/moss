;; read the N bytes above ourselves (storage);
;; write the N bytes above ourselves (memory)

;; where BIOS put us (this being a multiple of 0x10 is assumed)
%define LOAD_ADDRESS 0x7C00
;;TODO move this to inside the Makefile maybe


[BITS 16]

	cli

	; us above, stack below
	xor ax, ax
	mov ds, ax
	mov ss, ax
	mov sp, LOAD_ADDRESS

	; reading FLOPPY sectors, bios gave us DL (drive number)
	mov ch, 0                             ; C*, floppy: [0, 79]
	mov dh, 0                             ; H,  floppy: [0, 1]
	mov cl, 2 ; don't (re-)read ourselves ; S,  floppy: [1, 18]

	mov di, (LOAD_ADDRESS + 0x200) / 0x10 ; reading destination
	mov si, (N + 0x1ff) / 0x200           ; remaining unread sectors
	;          ^ ...so this division doesn't round down

read_sector:
	; reading one sector at a time by moving the extra segment (no offset)
	mov ah, 0x2
	mov al, 1
	mov es, di
	mov bx, 0
	int 0x13

;;HWNOTE: multiple attempts -- also probably won't be a CHS

	; handle reading error
	jc party


	; iterate
	add di, 0x200 / 0x10
	dec si
	cmp si, 0
	jna read_fin


	inc cl
	cmp cl, 18
	jna read_sector

	; carry S
	mov cl, 1
	inc dh
	cmp dh, 1
	jna read_sector

	; carry H
	mov dh, 0
	inc ch
	cmp ch, 79
	jna read_sector

	; handle overflow
	jmp party


party:
	mov ah, 0x0B
	;xor bh, bh
	inc bl
	int 0x10

	mov ah, 0x0E
	mov al, '!'
	int 0x10

	jmp party


[EXTERN kernel_entry]
read_fin:
	; set up the basic flat memory model and go to kernel ! 32 bits!
	cld
	lgdt [gdtd]

;;HWNOTE: A20

	; ENABLE PROTECTED MODE
	mov eax, CR0
	or eax, 1
	mov CR0, eax

;;HWNOTE: cli

	; GO GO GO !!!!!!!
	mov ax, 8
	mov SS, ax
	mov DS, ax
	mov ES, ax
	mov FS, ax
	mov GS, ax

	call 16:kernel_entry


gdtd:
	dw 0x17
	dd gdt

align 8
gdt:
	; nil
	dq 0

	; data
	;    +base2-+ B V    P	S E A			     +-----lmt1-----+
	;	     G 0 lmt2 PL X W +---------base1--------+
	dq 0b0000000011001111100100100000000000000000000000001111111111111111
	;    3210987654321098765432109876543210987654321098765432109876543210
	;    FEDCBA9876543210FEDCBA9876543210FEDCBA9876543210FEDCBA9876543210

	; code
	;    +base2-+ D V    P	S C A			     +-----lmt1-----+
	;	     G 0 lmt2 PL X R +---------base1--------+
	dq 0b0000000011001111100110100000000000000000000000001111111111111111
	;    3210987654321098765432109876543210987654321098765432109876543210
	;    FEDCBA9876543210FEDCBA9876543210FEDCBA9876543210FEDCBA9876543210


times 510-($-$$) db 0
signature:
	db 0b01010101
	db 0b10101010
