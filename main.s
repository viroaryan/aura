.intel_syntax noprefix
.global main
.extern puts
.extern malloc
.extern WSAStartup
.extern socket
.extern bind
.extern listen
.extern accept
.extern recv
.extern send
.extern closesocket
.extern CreateFileA
.extern ReadFile
.extern WriteFile
.extern CloseHandle
.text

main:
    push rbp
    mov rbp, rsp
    sub rsp, 128
    lea rax, .L_str_0[rip]
    mov rcx, rax
    call puts
    lea rax, .L_str_1[rip]
    mov rcx, rax
    call puts
    lea rax, .L_str_2[rip]
    mov rcx, rax
    call puts
    lea rax, .L_str_3[rip]
    mov rcx, rax
    call puts
    mov rcx, 400
    call malloc
    push rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 0], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 8], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 16], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 24], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 32], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 40], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 48], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 56], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 64], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 72], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 80], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 88], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 96], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 104], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 112], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 120], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 128], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 136], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 144], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 152], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 160], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 168], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 176], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 184], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 192], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 200], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 208], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 216], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 224], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 232], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 240], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 248], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 256], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 264], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 272], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 280], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 288], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 296], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 304], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 312], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 320], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 328], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 336], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 344], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 352], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 360], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 368], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 376], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 384], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 392], rax
    pop rax
    mov QWORD PTR [rbp-8], rax
    mov rax, 514
    mov rcx, rax
    mov rax, QWORD PTR [rbp-8]
    mov rdx, rax
    call WSAStartup
    mov QWORD PTR [rbp-16], rax
    mov rax, QWORD PTR [rbp-16]
    push rax
    mov rax, 0
    mov rbx, rax
    pop rax
    cmp rax, rbx
    setne al
    movzx rax, al
    cmp rax, 0
    je .L1
    lea rax, .L_str_4[rip]
    mov rcx, rax
    call puts
    mov rax, 1
    mov rsp, rbp
    pop rbp
    ret
    jmp .L2
.L1:
.L2:
    mov rax, 2
    mov rcx, rax
    mov rax, 1
    mov rdx, rax
    mov rax, 6
    mov r8, rax
    call socket
    mov QWORD PTR [rbp-24], rax
    mov rcx, 16
    call malloc
    push rax
    mov rax, 2501836802
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 0], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 8], rax
    pop rax
    mov QWORD PTR [rbp-32], rax
    mov rax, QWORD PTR [rbp-24]
    mov rcx, rax
    mov rax, QWORD PTR [rbp-32]
    mov rdx, rax
    mov rax, 16
    mov r8, rax
    call bind
    mov QWORD PTR [rbp-40], rax
    mov rax, QWORD PTR [rbp-40]
    push rax
    mov rax, 0
    mov rbx, rax
    pop rax
    cmp rax, rbx
    setne al
    movzx rax, al
    cmp rax, 0
    je .L3
    lea rax, .L_str_5[rip]
    mov rcx, rax
    call puts
    mov rax, 1
    mov rsp, rbp
    pop rbp
    ret
    jmp .L4
.L3:
.L4:
    mov rax, QWORD PTR [rbp-24]
    mov rcx, rax
    mov rax, 10
    mov rdx, rax
    call listen
    lea rax, .L_str_6[rip]
    mov rcx, rax
    call puts
    mov rax, QWORD PTR [rbp-24]
    mov rcx, rax
    mov rax, 0
    mov rdx, rax
    mov rax, 0
    mov r8, rax
    call accept
    mov QWORD PTR [rbp-48], rax
    lea rax, .L_str_7[rip]
    mov rcx, rax
    call puts
    mov rcx, 512
    call malloc
    push rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 0], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 8], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 16], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 24], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 32], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 40], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 48], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 56], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 64], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 72], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 80], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 88], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 96], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 104], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 112], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 120], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 128], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 136], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 144], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 152], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 160], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 168], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 176], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 184], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 192], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 200], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 208], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 216], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 224], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 232], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 240], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 248], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 256], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 264], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 272], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 280], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 288], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 296], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 304], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 312], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 320], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 328], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 336], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 344], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 352], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 360], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 368], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 376], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 384], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 392], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 400], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 408], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 416], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 424], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 432], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 440], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 448], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 456], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 464], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 472], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 480], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 488], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 496], rax
    mov rax, 0
    mov rbx, QWORD PTR [rsp]
    mov QWORD PTR [rbx + 504], rax
    pop rax
    mov QWORD PTR [rbp-56], rax
    mov rax, QWORD PTR [rbp-48]
    mov rcx, rax
    mov rax, QWORD PTR [rbp-56]
    mov rdx, rax
    mov rax, 512
    mov r8, rax
    mov rax, 0
    mov r9, rax
    call recv
    lea rax, .L_str_8[rip]
    mov rcx, rax
    call puts
    lea rax, .L_str_9[rip]
    mov QWORD PTR [rbp-64], rax
    mov rax, QWORD PTR [rbp-48]
    mov rcx, rax
    mov rax, QWORD PTR [rbp-64]
    mov rdx, rax
    mov rax, 216
    mov r8, rax
    mov rax, 0
    mov r9, rax
    call send
    lea rax, .L_str_10[rip]
    mov rcx, rax
    call puts
    mov rax, QWORD PTR [rbp-48]
    mov rcx, rax
    call closesocket
    mov rax, QWORD PTR [rbp-24]
    mov rcx, rax
    call closesocket
    lea rax, .L_str_11[rip]
    mov rcx, rax
    call puts
    mov rax, 0
    mov rsp, rbp
    pop rbp
    ret
    mov rsp, rbp
    pop rbp
    ret
.data
.L_str_0:
    .asciz "---------------------------------------"
.L_str_1:
    .asciz "Aura Native Web Server (x86-64 Native)"
.L_str_2:
    .asciz "Port: 8085"
.L_str_3:
    .asciz "---------------------------------------"
.L_str_4:
    .asciz "WSAStartup failed."
.L_str_5:
    .asciz "Bind failed. Is port 8085 in use?"
.L_str_6:
    .asciz "Waiting for connections..."
.L_str_7:
    .asciz "Client connected!"
.L_str_8:
    .asciz "Received HTTP request."
.L_str_9:
    .asciz "HTTP/1.1 200 OK\nContent-Type: text/html\n\n<html><body style='font-family:sans-serif;text-align:center;margin-top:50px;'><h1>Hello from Aura Native Engine!</h1><p>Running natively on x86-64 without C++!</p></body></html>"
.L_str_10:
    .asciz "Response sent."
.L_str_11:
    .asciz "Server shut down successfully."
