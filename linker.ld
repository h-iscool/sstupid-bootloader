ENTRY(stage2_start)

SECTIONS
{

    . = 0x7e00; /* starting point in mem of stage2_start */

    .text : {
       KEEP(*(.text.boot))
       *(.text .text.*)
       _init_array_start = .;
       *(.init_array)
       *(.ctors)
       _init_array_end = .;
    }

    .data : {
        *(.data)
    }

    .bss : {
        *(.bss)
    }

}
