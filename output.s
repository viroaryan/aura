    .global main
    .extern printf

    .data
format_num:
    .asciz "%d\n"
var_result:
    .quad 0
var_sq:
    .quad 0
var_score:
    .quad 0
var_sum:
    .quad 0
var_i:
    .quad 0
var_fact:
    .quad 0
var_n:
    .quad 0
var_x:
    .quad 0
var_a:
    .quad 0
var_b:
    .quad 0

    .text

aura_square:
    pushq %rbp
    movq %rsp, %rbp
    movq %rcx, var_x(%rip)
    movq var_x(%rip), %rax
    pushq %rax
    movq var_x(%rip), %rax
    pushq %rax
    popq %rbx
    popq %rax
    imulq %rbx, %rax
    pushq %rax
    popq %rax
    popq %rbp
    ret
    movq $0, %rax
    popq %rbp
    ret

aura_add:
    pushq %rbp
    movq %rsp, %rbp
    movq %rcx, var_a(%rip)
    movq %rdx, var_b(%rip)
    movq var_a(%rip), %rax
    pushq %rax
    movq var_b(%rip), %rax
    pushq %rax
    popq %rbx
    popq %rax
    addq %rbx, %rax
    pushq %rax
    popq %rax
    popq %rbp
    ret
    movq $0, %rax
    popq %rbp
    ret

main:
    pushq %rbp
    movq %rsp, %rbp
    movq $20, %rax
    pushq %rax
    movq $10, %rax
    pushq %rax
    popq %rcx
    popq %rdx
    subq $32, %rsp
    call aura_add
    addq $32, %rsp
    pushq %rax
    popq %rax
    movq %rax, var_result(%rip)
    movq var_result(%rip), %rax
    pushq %rax
    popq %rdx
    leaq format_num(%rip), %rcx
    subq $32, %rsp
    call printf
    addq $32, %rsp
    movq $7, %rax
    pushq %rax
    popq %rcx
    subq $32, %rsp
    call aura_square
    addq $32, %rsp
    pushq %rax
    popq %rax
    movq %rax, var_sq(%rip)
    movq var_sq(%rip), %rax
    pushq %rax
    popq %rdx
    leaq format_num(%rip), %rcx
    subq $32, %rsp
    call printf
    addq $32, %rsp
    movq $85, %rax
    pushq %rax
    popq %rax
    movq %rax, var_score(%rip)
    movq var_score(%rip), %rax
    pushq %rax
    movq $90, %rax
    pushq %rax
    popq %rbx
    popq %rax
    cmpq %rbx, %rax
    setg %al
    movzbq %al, %rax
    pushq %rax
    popq %rax
    testq %rax, %rax
    je .L1
    movq $1, %rax
    pushq %rax
    popq %rdx
    leaq format_num(%rip), %rcx
    subq $32, %rsp
    call printf
    addq $32, %rsp
    jmp .L2
.L1:
    movq $0, %rax
    pushq %rax
    popq %rdx
    leaq format_num(%rip), %rcx
    subq $32, %rsp
    call printf
    addq $32, %rsp
.L2:
    movq $0, %rax
    pushq %rax
    popq %rax
    movq %rax, var_sum(%rip)
    movq $1, %rax
    pushq %rax
    popq %rax
    movq %rax, var_i(%rip)
.L3:
    movq var_i(%rip), %rax
    pushq %rax
    movq $10, %rax
    pushq %rax
    popq %rbx
    popq %rax
    cmpq %rbx, %rax
    setle %al
    movzbq %al, %rax
    pushq %rax
    popq %rax
    testq %rax, %rax
    je .L4
    movq var_sum(%rip), %rax
    pushq %rax
    movq var_i(%rip), %rax
    pushq %rax
    popq %rbx
    popq %rax
    addq %rbx, %rax
    pushq %rax
    popq %rax
    movq %rax, var_sum(%rip)
    movq var_i(%rip), %rax
    pushq %rax
    movq $1, %rax
    pushq %rax
    popq %rbx
    popq %rax
    addq %rbx, %rax
    pushq %rax
    popq %rax
    movq %rax, var_i(%rip)
    jmp .L3
.L4:
    movq var_sum(%rip), %rax
    pushq %rax
    popq %rdx
    leaq format_num(%rip), %rcx
    subq $32, %rsp
    call printf
    addq $32, %rsp
    movq $1, %rax
    pushq %rax
    popq %rax
    movq %rax, var_fact(%rip)
    movq $5, %rax
    pushq %rax
    popq %rax
    movq %rax, var_n(%rip)
.L5:
    movq var_n(%rip), %rax
    pushq %rax
    movq $1, %rax
    pushq %rax
    popq %rbx
    popq %rax
    cmpq %rbx, %rax
    setg %al
    movzbq %al, %rax
    pushq %rax
    popq %rax
    testq %rax, %rax
    je .L6
    movq var_fact(%rip), %rax
    pushq %rax
    movq var_n(%rip), %rax
    pushq %rax
    popq %rbx
    popq %rax
    imulq %rbx, %rax
    pushq %rax
    popq %rax
    movq %rax, var_fact(%rip)
    movq var_n(%rip), %rax
    pushq %rax
    movq $1, %rax
    pushq %rax
    popq %rbx
    popq %rax
    subq %rbx, %rax
    pushq %rax
    popq %rax
    movq %rax, var_n(%rip)
    jmp .L5
.L6:
    movq var_fact(%rip), %rax
    pushq %rax
    popq %rdx
    leaq format_num(%rip), %rcx
    subq $32, %rsp
    call printf
    addq $32, %rsp
    movq $0, %rax
    popq %rbp
    ret
