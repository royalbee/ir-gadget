_stack_size = 4K;

MEMORY {                                           /* memory map of LM3S811 */
    FLASH (rx)  : ORIGIN = 0x00000000, LENGTH = 256K
    SRAM (xrw) : ORIGIN = 0x20000000, LENGTH = 32K
}

SECTIONS
{
    .text :
    {
        _text = .;
        KEEP(*(.isr_vector))
        *(.text*)
        *(.rodata*)
    } > FLASH

    .ARM.exidx :
    {
        __exidx_start = .;
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        __exidx_end = .;
    } > FLASH
      
    _etext = .;

    .data : AT(_etext)
    {
        _data = .;
        *(vtable)
        *(.data*)
        _edata = .;
    } > SRAM

    .bss : AT(ADDR(.data) + SIZEOF(.data))
    {
        _bss = .;
        *(.bss*)
        *(COMMON)
        _ebss = .;
    } > SRAM

    _heap_bottom = .;
    _heap_top = ORIGIN(SRAM) + LENGTH(SRAM) - _stack_size;

    _stack_bottom = _heap_top;
    _stack_top = ORIGIN(SRAM) + LENGTH(SRAM);
    PROVIDE ( end = _ebss );
}
