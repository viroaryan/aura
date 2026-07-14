    .global main
    .extern printf

    .data
msg:
    .asciz "Hello from Native x86-64 Assembly!\n"

    .text
main:
    # Windows x64 ABI requires 32 bytes of shadow space + 8 bytes alignment = 40 bytes
    subq $40, %rsp
    
    leaq msg(%rip), %rcx
    call printf
    
    addq $40, %rsp
    movq $0, %rax
    ret
