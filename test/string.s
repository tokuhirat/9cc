.intel_syntax noprefix
  .data
  .globl .L..10
.L..10:
  .byte 79
  .byte 75
  .byte 10
  .byte 0
  .data
  .globl .L..9
.L..9:
  .byte 115
  .byte 105
  .byte 122
  .byte 101
  .byte 111
  .byte 102
  .byte 40
  .byte 34
  .byte 97
  .byte 98
  .byte 99
  .byte 34
  .byte 41
  .byte 0
  .data
  .globl .L..8
.L..8:
  .byte 97
  .byte 98
  .byte 99
  .byte 0
  .data
  .globl .L..7
.L..7:
  .byte 34
  .byte 97
  .byte 98
  .byte 99
  .byte 34
  .byte 91
  .byte 51
  .byte 93
  .byte 0
  .data
  .globl .L..6
.L..6:
  .byte 97
  .byte 98
  .byte 99
  .byte 0
  .data
  .globl .L..5
.L..5:
  .byte 34
  .byte 97
  .byte 98
  .byte 99
  .byte 34
  .byte 91
  .byte 50
  .byte 93
  .byte 0
  .data
  .globl .L..4
.L..4:
  .byte 97
  .byte 98
  .byte 99
  .byte 0
  .data
  .globl .L..3
.L..3:
  .byte 34
  .byte 97
  .byte 98
  .byte 99
  .byte 34
  .byte 91
  .byte 49
  .byte 93
  .byte 0
  .data
  .globl .L..2
.L..2:
  .byte 97
  .byte 98
  .byte 99
  .byte 0
  .data
  .globl .L..1
.L..1:
  .byte 34
  .byte 97
  .byte 98
  .byte 99
  .byte 34
  .byte 91
  .byte 48
  .byte 93
  .byte 0
  .data
  .globl .L..0
.L..0:
  .byte 97
  .byte 98
  .byte 99
  .byte 0
  .globl main
  .text
main:
  push rbp
  mov rbp, rsp
  sub rsp, 0
  mov rax, 97
  push rax
  mov rax, 1
  push rax
  mov rax, 0
  pop rdi
  imul rax, rdi
  push rax
  lea rax, .L..0[rip]
  pop rdi
  add rax, rdi
  movsx eax, BYTE PTR [rax]
  push rax
  lea rax, .L..1[rip]
  push rax
  pop rdx
  pop rsi
  pop rdi
  mov rax, 3
  call assert
  mov rax, 98
  push rax
  mov rax, 1
  push rax
  mov rax, 1
  pop rdi
  imul rax, rdi
  push rax
  lea rax, .L..2[rip]
  pop rdi
  add rax, rdi
  movsx eax, BYTE PTR [rax]
  push rax
  lea rax, .L..3[rip]
  push rax
  pop rdx
  pop rsi
  pop rdi
  mov rax, 3
  call assert
  mov rax, 99
  push rax
  mov rax, 1
  push rax
  mov rax, 2
  pop rdi
  imul rax, rdi
  push rax
  lea rax, .L..4[rip]
  pop rdi
  add rax, rdi
  movsx eax, BYTE PTR [rax]
  push rax
  lea rax, .L..5[rip]
  push rax
  pop rdx
  pop rsi
  pop rdi
  mov rax, 3
  call assert
  mov rax, 0
  push rax
  mov rax, 1
  push rax
  mov rax, 3
  pop rdi
  imul rax, rdi
  push rax
  lea rax, .L..6[rip]
  pop rdi
  add rax, rdi
  movsx eax, BYTE PTR [rax]
  push rax
  lea rax, .L..7[rip]
  push rax
  pop rdx
  pop rsi
  pop rdi
  mov rax, 3
  call assert
  mov rax, 4
  push rax
  mov rax, 4
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
