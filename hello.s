.intel_syntax noprefix
.global main
.text
main:
    push rbp
    mov rbp, rsp
    sub rsp, 16
    mov rax, 15
    mov QWORD PTR [rbp-8], rax
    mov rax, 20
    mov QWORD PTR [rbp-16], rax
    mov rax, QWORD PTR [rbp-8]
    add rax, QWORD PTR [rbp-16]
    mov rsp, rbp
    pop rbp
    ret
