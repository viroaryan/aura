    .global main
    .extern printf

    .data
format_num:
    .asciz "%d\n"
var_a:
    .quad 0
var_b:
    .quad 0
var_c:
    .quad 0

    .text
main:
    pushq %rbp
    movq %rsp, %rbp
    movq $50, %rax
    pushq %rax
    popq %rax
    movq %rax, var_a(%rip)
    movq $100, %rax
    pushq %rax
    popq %rax
    movq %rax, var_b(%rip)
    movq var_a(%rip), %rax
    pushq %rax
    movq var_b(%rip), %rax
    pushq %rax
    popq %rbx
    popq %rax
    imulq %rbx, %rax
    pushq %rax
    movq $25, %rax
    pushq %rax
    popq %rbx
    popq %rax
    addq %rbx, %rax
    pushq %rax
    popq %rax
    movq %rax, var_c(%rip)
    movq var_c(%rip), %rax
    pushq %rax
    popq %rdx
    leaq format_num(%rip), %rcx
    subq $32, %rsp
    call printf
    addq $32, %rsp
    movq $0, %rax
    popq %rbp
    ret
