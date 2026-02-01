;; nasm -f elf64 aoc1.asm && gcc aoc1.o -o aoc1 && ./aoc1

global main

extern printf

section .data
message: db "Result: %d", 0xA, 0x0

;; use relative addresses
default rel

;; input's size=100
inputs: dd 123265, 68442, 94896, 94670, 145483, 93807, 88703, 139755, 53652, 52754, 128052, 81533, 56602, 96476, 87674, 102510, 95735, 69174, 136331, 51266, 148009, 72417, 52577, 86813, 60803, 149232, 115843, 138175, 94723, 85623, 97925, 141772, 63662, 107293, 130779, 147027, 88003, 77238, 53184, 149255, 71921, 139799, 84851, 104899, 92290, 74438, 55631, 58655, 140496, 110176, 138718, 104768, 93177, 53212, 129572, 69877, 139944, 116062, 51362, 135245, 59682, 128705, 98105, 69172, 89244, 109048, 88690, 62124, 53981, 71885, 59216, 107718, 146343, 138788, 73588, 51648, 122227, 54507, 59283, 101230, 93080, 123120, 148248, 102909, 91199, 105704, 113956, 120368, 75020, 103734, 81791, 87323, 77278, 123013, 58901, 136351, 121295, 132994, 84039, 76813

section .text
main:
    push rbp
    mov rbp, rsp

    xor rbx, rbx
    mov rcx, 100
    xor rsi, rsi
    lea r9, [inputs]

mainloop:
    mov eax, [r9 + rsi * 4]

    xor edx, edx
    mov r8d, 3
    div r8d

    sub eax, 2

    add rbx, rax

    inc rsi
    dec rcx
    jnz mainloop

    lea rdi, [rel message]
    mov rsi, rbx
    xor eax, eax
    ;; use Procedure Linkage Table 
    call printf wrt ..plt

    xor eax, eax
    pop rbp
    ret
