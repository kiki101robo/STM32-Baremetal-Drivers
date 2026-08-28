/**
 ******************************************************************************
 * @file      LinkerScript.ld
 * @brief     Linker script for STM32F411RETx (512KB FLASH, 128KB RAM)
 *            Updated to avoid RWX LOAD segment:
 *              - RAM is (rw), not executable
 *              - .ramfunc placed in its own RX section in RAM (AT>FLASH)
 ******************************************************************************
 */

/* Entry Point */
ENTRY(Reset_Handler)

/* Highest address of the user mode stack */
_estack = ORIGIN(RAM) + LENGTH(RAM);   /* end of "RAM" */

/* Heap/Stack (adjust as needed) */
_Min_Heap_Size  = 0x200;  /* 512 B heap  */
_Min_Stack_Size = 0x400;  /* 1 KB stack */

/* Memories definition */
MEMORY
{
  RAM   (rw)  : ORIGIN = 0x20000000, LENGTH = 128K
  FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 512K
}

/* Sections */
SECTIONS
{
  /* Vector table into FLASH */
  .isr_vector :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector)) /* Startup code */
    . = ALIGN(4);
  } > FLASH

  /* Program code into FLASH */
  .text :
  {
    . = ALIGN(4);
    *(.text)           /* .text sections (code) */
    *(.text*)          /* .text* sections (code) */
    *(.glue_7)         /* glue arm to thumb code */
    *(.glue_7t)        /* glue thumb to arm code */
    *(.eh_frame)

    KEEP (*(.init))
    KEEP (*(.fini))

    . = ALIGN(4);
    _etext = .;        /* end of code in FLASH */
  } > FLASH

  /* Read-only data into FLASH */
  .rodata :
  {
    . = ALIGN(4);
    *(.rodata)         /* constants, strings */
    *(.rodata*)
    . = ALIGN(4);
  } > FLASH

  .ARM.extab :
  {
    . = ALIGN(4);
    *(.ARM.extab* .gnu.linkonce.armextab.*)
    . = ALIGN(4);
  } > FLASH

  .ARM :
  {
    . = ALIGN(4);
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
    . = ALIGN(4);
  } > FLASH

  .preinit_array :
  {
    . = ALIGN(4);
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP(*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
    . = ALIGN(4);
  } > FLASH

  .init_array :
  {
    . = ALIGN(4);
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP(*(SORT(.init_array.*)))
    KEEP(*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
    . = ALIGN(4);
  } > FLASH

  .fini_array :
  {
    . = ALIGN(4);
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP(*(SORT(.fini_array.*)))
    KEEP(*(.fini_array*))
    PROVIDE_HIDDEN (__fini_array_end = .);
    . = ALIGN(4);
  } > FLASH

  /* Used by startup to initialize .data */
  _sidata = LOADADDR(.data);

  /* ---- Code that must execute from RAM (separate from .data) ---- */
  /* Place functions with attribute/section .RamFunc into RAM (RX), loaded from FLASH. */
  .ramfunc :
  {
    . = ALIGN(4);
    _sramfunc = .;         /* start of ramfunc in RAM */
    *(.RamFunc)
    *(.RamFunc*)
    . = ALIGN(4);
    _eramfunc = .;         /* end of ramfunc in RAM */
  } > RAM AT > FLASH
  _siramfunc = LOADADDR(.ramfunc);  /* source address in FLASH for copying */

  /* ---- Initialized data (RW in RAM, image stored in FLASH) ---- */
  .data :
  {
    . = ALIGN(4);
    _sdata = .;            /* start of .data in RAM */
    *(.data)
    *(.data*)
    . = ALIGN(4);
    _edata = .;            /* end of .data in RAM */
  } > RAM AT > FLASH

  /* ---- Zero-initialized data (BSS in RAM) ---- */
  . = ALIGN(4);
  .bss :
  {
    _sbss = .;
    __bss_start__ = _sbss;
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;
    __bss_end__ = _ebss;
  } > RAM

  /* ---- Heap + Stack reservation (in RAM) ---- */
  ._user_heap_stack :
  {
    . = ALIGN(8);
    PROVIDE(end  = .);
    PROVIDE(_end = .);
    . = . + _Min_Heap_Size;
    . = . + _Min_Stack_Size;
    . = ALIGN(8);
  } > RAM

  /* Remove information from compiler libraries (keep minimal) */
  /DISCARD/ :
  {
    libc.a  ( * )
    libm.a  ( * )
    libgcc.a( * )
  }

  .ARM.attributes 0 : { *(.ARM.attributes) }
}
