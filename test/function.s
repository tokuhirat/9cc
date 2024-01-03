.intel_syntax noprefix
  .data
  .globl .L..10
.L..10:
  .byte 79
  .byte 75
  .byte 92
  .byte 110
  .byte 0
  .data
  .globl .L..9
.L..9:
  .byte 40
  .byte 123
  .byte 32
  .byte 115
  .byte 117
  .byte 98
  .byte 95
  .byte 99
  .byte 104
  .byte 97
  .byte 114
  .byte 40
  .byte 55
  .byte 44
  .byte 32
  .byte 51
  .byte 44
  .byte 32
  .byte 51
  .byte 41
  .byte 59
  .byte 32
  .byte 125
  .byte 41
  .byte 0
  .data
  .globl .L..8
.L..8:
  .byte 102
  .byte 105
  .byte 98
  .byte 40
  .byte 57
  .byte 41
  .byte 0
  .data
  .globl .L..7
.L..7:
  .byte 115
  .byte 117
  .byte 98
  .byte 50
  .byte 40
  .byte 52
  .byte 44
  .byte 51
  .byte 41
  .byte 0
  .data
  .globl .L..6
.L..6:
  .byte 97
  .byte 100
  .byte 100
  .byte 50
  .byte 40
  .byte 51
  .byte 44
  .byte 52
  .byte 41
  .byte 0
  .data
  .globl .L..5
.L..5:
  .byte 97
  .byte 100
  .byte 100
  .byte 54
  .byte 40
  .byte 49
  .byte 44
  .byte 50
  .byte 44
  .byte 97
  .byte 100
  .byte 100
  .byte 54
  .byte 40
  .byte 51
  .byte 44
  .byte 97
  .byte 100
  .byte 100
  .byte 54
  .byte 40
  .byte 52
  .byte 44
  .byte 53
  .byte 44
  .byte 54
  .byte 44
  .byte 55
  .byte 44
  .byte 56
  .byte 44
  .byte 57
  .byte 41
  .byte 44
  .byte 49
  .byte 48
  .byte 44
  .byte 49
  .byte 49
  .byte 44
  .byte 49
  .byte 50
  .byte 44
  .byte 49
  .byte 51
  .byte 41
  .byte 44
  .byte 49
  .byte 52
  .byte 44
  .byte 49
  .byte 53
  .byte 44
  .byte 49
  .byte 54
  .byte 41
  .byte 0
  .data
  .globl .L..4
.L..4:
  .byte 97
  .byte 100
  .byte 100
  .byte 54
  .byte 40
  .byte 49
  .byte 44
  .byte 50
  .byte 44
  .byte 97
  .byte 100
  .byte 100
  .byte 54
  .byte 40
  .byte 51
  .byte 44
  .byte 52
  .byte 44
  .byte 53
  .byte 44
  .byte 54
  .byte 44
  .byte 55
  .byte 44
  .byte 56
  .byte 41
  .byte 44
  .byte 57
  .byte 44
  .byte 49
  .byte 48
  .byte 44
  .byte 49
  .byte 49
  .byte 41
  .byte 0
  .data
  .globl .L..3
.L..3:
  .byte 97
  .byte 100
  .byte 100
  .byte 54
  .byte 40
  .byte 49
  .byte 44
  .byte 50
  .byte 44
  .byte 51
  .byte 44
  .byte 52
  .byte 44
  .byte 53
  .byte 44
  .byte 54
  .byte 41
  .byte 0
  .data
  .globl .L..2
.L..2:
  .byte 115
  .byte 117
  .byte 98
  .byte 50
  .byte 40
  .byte 53
  .byte 44
  .byte 32
  .byte 51
  .byte 41
  .byte 0
  .data
  .globl .L..1
.L..1:
  .byte 97
  .byte 100
  .byte 100
  .byte 50
  .byte 40
  .byte 51
  .byte 44
  .byte 32
  .byte 53
  .byte 41
  .byte 0
  .data
  .globl .L..0
.L..0:
  .byte 114
  .byte 101
  .byte 116
  .byte 51
  .byte 40
  .byte 41
  .byte 0
  .globl main
  .text
main:
  push rbp
  mov rbp, rsp
  sub rsp, 0
  mov rax, 3
  push rax
  mov rax, 0
  call ret3
  push rax
  lea rax, .L..0[rip]
  push rax
  pop rdx
  pop rsi
  pop rdi
  mov rax, 3
  call assert
  mov rax, 8
  push rax
  mov rax, 3
  push rax
  mov rax, 5
  push rax
  pop rsi
  pop rdi
  mov rax, 2
  call add2
  push rax
  lea rax, .L..1[rip]
  push rax
  pop rdx
  pop rsi
  pop rdi
  mov rax, 3
  call assert
  mov rax, 2
  push rax
  mov rax, 5
  push rax
  mov rax, 3
  push rax
  pop rsi
  pop rdi
  mov rax, 2
  call sub2
  push rax
  lea rax, .L..2[rip]
  push rax
  pop rdx
  pop rsi
  pop rdi
  mov rax, 3
  call assert
  mov rax, 21
  push rax
  mov rax, 1
  push rax
  mov rax, 2
  push rax
  mov rax, 3
  push rax
  mov rax, 4
  push rax
  mov rax, 5
  push rax
  mov rax, 6
  push rax
  pop r9
  pop r8
  pop rcx
  pop rdx
  pop rsi
  pop rdi
  mov rax, 6
  call add6
  push rax
  lea rax, .L..3[rip]
  push rax
  pop rdx
  pop rsi
  pop rdi
  mov rax, 3
  call assert
  mov rax, 66
  push rax
  mov rax, 1
  push rax
  mov rax, 2
  push rax
  mov rax, 3
  push rax
  mov rax, 4
  push rax
  mov rax, 5
  push rax
  mov rax, 6
  push rax
  mov rax, 7
  push rax
  mov rax, 8
  push rax
  pop r9
  pop r8
  pop rcx
  pop rdx
  pop rsi
  pop rdi
  mov rax, 6
  call add6
  push rax
  mov rax, 9
  push rax
  mov rax, 10
  push rax
  mov rax, 11
  push rax
  pop r9
  pop r8
  pop rcx
  pop rdx
  pop rsi
  pop rdi
  mov rax, 6
  call add6
  push rax
  lea rax, .L..4[rip]
  push rax
  pop rdx
  pop rsi
  pop rdi
  mov rax, 3
  call assert
  mov rax, 136
  push rax
  mov rax, 1
  push rax
  mov rax, 2
  push rax
  mov rax, 3
  push rax
  mov rax, 4
  push rax
  mov rax, 5
  push rax
  mov rax, 6
  push rax
  mov rax, 7
  push rax
  mov rax, 8
  push rax
  mov rax, 9
  push rax
  pop r9
  pop r8
  pop rcx
  pop rdx
  pop rsi
  pop rdi
  mov rax, 6
  call add6
  push rax
  mov rax, 10
  push rax
  mov rax, 11
  push rax
  mov rax, 12
  push rax
  mov rax, 13
  push rax
  pop r9
  pop r8
  pop rcx
  pop rdx
  pop rsi
  pop rdi
  mov rax, 6
  call add6
  push rax
  mov rax, 14
  push rax
  mov rax, 15
  push rax
  mov rax, 16
  push rax
  pop r9
  pop r8
  pop rcx
  pop rdx
  pop rsi
  pop rdi
  mov rax, 6
  call add6
  push rax
  lea rax, .L..5[rip]
  push rax
  pop rdx
  pop rsi
  pop rdi
  mov rax, 3
  call assert
  mov rax, 7
  push rax
  mov rax, 3
  push rax
  mov rax, 4
  push rax
  pop rsi
  pop rdi
  mov rax, 2
  call add2
  push rax
  lea rax, .L..6[rip]
  push rax
  pop rdx
  pop rsi
  pop rdi
  mov rax, 3
  call assert
  mov rax, 1
  push rax
  mov rax, 4
  push rax
  mov rax, 3
  push rax
  pop rsi
  pop rdi
  mov rax, 2
  call sub2
  push rax
  lea rax, .L..7[rip]
  push rax
  pop rdx
  pop rsi
  pop rdi
  mov rax, 3
  call assert
  mov rax, 55
  push rax
  mov rax, 9
  push rax
  pop rdi
  mov rax, 1
  call fib
  push rax
  lea rax, .L..8[rip]
  push rax
  pop rdx
  pop rsi
  pop rdi
  mov rax, 3
  call assert
  mov rax, 1
  push rax
  mov rax, 7
  push rax
  mov rax, 3
  push rax
  mov rax, 3
  push rax
  pop rdx
  pop rsi
  pop rdi
  mov rax, 3
  call sub_char
  push rax
  lea rax, .L..9[rip]
  push rax
  pop rdx
  pop rsi
  pop rdi
  mov rax, 3
  call assert
  lea rax, .L..10[rip]
  push rax
  pop rdi
  mov rax, 1
  call printf
  mov rax, 0
  jmp .L.return.main
.L.return.main:
  mov rsp, rbp
  pop rbp
  ret
  .globl fib
  .text
fib:
  push rbp
  mov rbp, rsp
  sub rsp, 16
  mov [rbp + -8], rdi
  mov rax, 1
  push rax
  lea rax, [rbp + -8]
  mov rax, [rax]
  pop rdi
  cmp rax, rdi
  setle al
  movzb rax, al
  cmp rax, 0
  je .L.else.1
  mov rax, 1
  jmp .L.return.fib
  jmp .L.end.1
.L.else.1:
.L.end.1:
  mov rax, 2
  push rax
  lea rax, [rbp + -8]
  mov rax, [rax]
  pop rdi
  sub rax, rdi
  push rax
  pop rdi
  mov rax, 1
  call fib
  push rax
  mov rax, 1
  push rax
  lea rax, [rbp + -8]
  mov rax, [rax]
  pop rdi
  sub rax, rdi
  push rax
  pop rdi
  mov rax, 1
  call fib
  pop rdi
  add rax, rdi
  jmp .L.return.fib
.L.return.fib:
  mov rsp, rbp
  pop rbp
  ret
  .globl sub_char
  .text
sub_char:
  push rbp
  mov rbp, rsp
  sub rsp, 16
  mov [rbp + -1], dil
  mov [rbp + -2], sil
  mov [rbp + -3], dl
  lea rax, [rbp + -3]
  movsx eax, BYTE PTR [rax]
  push rax
  lea rax, [rbp + -2]
  movsx eax, BYTE PTR [rax]
  push rax
  lea rax, [rbp + -1]
  movsx eax, BYTE PTR [rax]
  pop rdi
  sub rax, rdi
  pop rdi
  sub rax, rdi
  jmp .L.return.sub_char
.L.return.sub_char:
  mov rsp, rbp
  pop rbp
  ret
  .globl addx
  .text
addx:
  push rbp
  mov rbp, rsp
  sub rsp, 16
  mov [rbp + -8], rdi
  mov [rbp + -16], rsi
  lea rax, [rbp + -16]
  mov rax, [rax]
  push rax
  lea rax, [rbp + -8]
  mov rax, [rax]
  mov rax, [rax]
  pop rdi
  add rax, rdi
  jmp .L.return.addx
.L.return.addx:
  mov rsp, rbp
  pop rbp
  ret
  .globl add6
  .text
add6:
  push rbp
  mov rbp, rsp
  sub rsp, 48
  mov [rbp + -8], rdi
  mov [rbp + -16], rsi
  mov [rbp + -24], rdx
  mov [rbp + -32], rcx
  mov [rbp + -40], r8
  mov [rbp + -48], r9
  lea rax, [rbp + -48]
  mov rax, [rax]
  push rax
  lea rax, [rbp + -40]
  mov rax, [rax]
  push rax
  lea rax, [rbp + -32]
  mov rax, [rax]
  push rax
  lea rax, [rbp + -24]
  mov rax, [rax]
  push rax
  lea rax, [rbp + -16]
  mov rax, [rax]
  push rax
  lea rax, [rbp + -8]
  mov rax, [rax]
  pop rdi
  add rax, rdi
  pop rdi
  add rax, rdi
  pop rdi
  add rax, rdi
  pop rdi
  add rax, rdi
  pop rdi
  add rax, rdi
  jmp .L.return.add6
.L.return.add6:
  mov rsp, rbp
  pop rbp
  ret
  .globl sub2
  .text
sub2:
  push rbp
  mov rbp, rsp
  sub rsp, 16
  mov [rbp + -8], rdi
  mov [rbp + -16], rsi
  lea rax, [rbp + -16]
  mov rax, [rax]
  push rax
  lea rax, [rbp + -8]
  mov rax, [rax]
  pop rdi
  sub rax, rdi
  jmp .L.return.sub2
.L.return.sub2:
  mov rsp, rbp
  pop rbp
  ret
  .globl add2
  .text
add2:
  push rbp
  mov rbp, rsp
  sub rsp, 16
  mov [rbp + -8], rdi
  mov [rbp + -16], rsi
  lea rax, [rbp + -16]
  mov rax, [rax]
  push rax
  lea rax, [rbp + -8]
  mov rax, [rax]
  pop rdi
  add rax, rdi
  jmp .L.return.add2
.L.return.add2:
  mov rsp, rbp
  pop rbp
  ret
  .globl ret3
  .text
ret3:
  push rbp
  mov rbp, rsp
  sub rsp, 0
  mov rax, 3
  jmp .L.return.ret3
  mov rax, 5
  jmp .L.return.ret3
.L.return.ret3:
  mov rsp, rbp
  pop rbp
  ret
