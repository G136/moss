[BITS 32]

[GLOBAL cpu_hlt]
cpu_hlt:
	hlt

[GLOBAL cpu_cli]
cpu_cli:
	cli
	ret
