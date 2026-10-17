SECTIONS {
	. = 0x7C00; /* wouldn't it be better to have all this in just one place?
	...the Makefile ? */

	.boot-sector : {
		boot-sector.o (.text);
	}

	.kernel : {
		*(.text);
		*(.rodata);
		*(.rodata.*);
		*(.data);
		*(.bss);
	}

	/DISCARD/ : {
		*(.eh_frame)
		*(.note.gnu.property)
		*(.comment)
	}
}
