/*
 * For the time being, I have copied bztsrc's script and modified a small amount,
 * as to make progress without worrying about the stuff I do not know.
 * https://github.com/bztsrc/raspi3-tutorial/blob/master/02_multicorec/start.S
 * 
 * bztsrc's github contains useful information for building a bare metal c os,
 * where some of the code in the project may be inspired/based on (relevant sections
 * will be explicitly stated, otherwise individually coded)
 *
 * In the future, I may write my own section depending on the needs of the project.
 */

/*
 *
 * Copyright (C) 2018 bzt (bztsrc@github)
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */

.section ".text.boot"

.global _start

_start:
    // read cpu id, stop slave cores
    mrs     x1, mpidr_el1
    and     x1, x1, #3
    cbz     x1, 2f

    // Modified to redirect cores to their own main functions.
    cmp     x1, 1
    b.eq    _run_core_2

    cmp     x1, 2
    b.eq    _run_core_3
    
    cmp     x1, 3
    b.eq    _run_core_4

    // cpu id > 0, stop
1:  wfe
    b       1b
2:  // cpu id == 0
    // jump to C code, should not return
    mov x1, 0x1B000000
    mov x2, 0xFF0000
    add x1, x1, x2
    mov sp, x1
4:  bl      main
    // for failsafe, halt this core too
    b       1b

_run_core_2:  
    mov x1, 0x1B000000
    mov x2, 0xFF0000
    add x1, x1, x2
    mov x2, 0x10000
    sub x1, x1, x2
    mov sp, x1
    bl main_core_2
    b 1b

_run_core_3:  
    mov x1, 0x1B000000
    mov x2, 0xFF0000
    add x1, x1, x2
    mov x2, 0x20000
    sub x1, x1, x2
    mov sp, x1
    bl main_core_3
    b 1b

_run_core_4:  
    mov x1, 0x1B000000
    mov x2, 0xFF0000
    add x1, x1, x2
    mov x2, 0x30000
    sub x1, x1, x2
    mov sp, x1
    bl main_core_4
    b 1b