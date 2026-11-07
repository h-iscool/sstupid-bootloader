ENTRY(stage2_start)

SECTIONS
{

    . = 0x7e00; /* starting point in mem of stage2_start */

    .text : {
        stage2.o(.text)
        *(.text)
    }

    .data : {
        *(.data)
    }

    .bss : {
        *(.bss)
    }

}