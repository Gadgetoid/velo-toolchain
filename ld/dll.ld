ENTRY(DllMain)
SECTIONS
{
    . = 0x11000;
    .text : { *(.text .text.*) }
    . = ALIGN(0x1000);
    .rdata : { *(.rodata .rodata.*) }
    . = ALIGN(0x1000);
    .data : { *(.data .data.* .sdata .sdata.*) *(.sbss .sbss.* .bss .bss.* COMMON) *(.got) LONG(0) }
    . = ALIGN(0x1000);
    .idata : { . = . + (DEFINED(VELO_IMPORT_RESERVE) ? VELO_IMPORT_RESERVE : 0x200); *(.iat) *(SORT_BY_NAME(.iat.*)) }
    /DISCARD/ : { *(.MIPS.abiflags) *(.reginfo) *(.comment) *(.pdr) *(.gnu.attributes) *(.mdebug.*) *(.rel.pdr) *(.eh_frame) *(.note*) }
}
