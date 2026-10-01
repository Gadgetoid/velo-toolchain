ENTRY(WinMain)
SECTIONS
{
    . = 0x11000;
    .text : { *(.text .text.*) }
    . = ALIGN(0x1000);
    .rdata : { *(.rodata .rodata.*) }
    . = ALIGN(0x1000);
    .data : { *(.iat) *(SORT_BY_NAME(.iat.*)) *(.data .data.* .sdata .sdata.*) *(.sbss .sbss.* .bss .bss.* COMMON) }
    /DISCARD/ : { *(.MIPS.abiflags) *(.reginfo) *(.comment) *(.pdr) *(.gnu.attributes) *(.mdebug.*) *(.rel.pdr) *(.eh_frame) *(.note*) }
}
