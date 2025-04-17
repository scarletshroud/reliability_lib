SECTIONS
{
  . = 0x80000000;			/* Загрузочный адрес QEMU для запуска bios */
  .text : {
		KEEP(*(.text._start)); 
		*(.text*);			
	}

    . = ALIGN (CONSTANT (COMMONPAGESIZE));

  .data : { 
		*(.data*)
	}
}