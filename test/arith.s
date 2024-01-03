.intel_syntax noprefix
  .data
  .globl .L..26
.L..26:
  .byte 79
  .byte 75
  .byte 92
  .byte 110
  .byte 0
  .data
  .globl .L..25
.L..25:
  .byte 49
  .byte 62
  .byte 61
  .byte 50
  .byte 0
  .data
  .globl .L..24
.L..24:
  .byte 49
  .byte 62
  .byte 61
  .byte 49
  .byte 0
  .data
  .globl .L..23
.L..23:
  .byte 49
  .byte 62
  .byte 61
  .byte 48
  .byte 0
  .data
  .globl .L..22
.L..22:
  .byte 49
  .byte 62
  .byte 50
  .byte 0
  .data
  .globl .L..21
.L..21:
  .byte 49
  .byte 62
  .byte 49
  .byte 0
  .data
  .globl .L..20
.L..20:
  .byte 49
  .byte 62
  .byte 48
  .byte 0
  .data
  .globl .L..19
.L..19:
  .byte 50
  .byte 60
  .byte 61
  .byte 49
  .byte 0
  .data
  .globl .L..18
.L..18:
  .byte 49
  .byte 60
  .byte 61
  .byte 49
  .byte 0
  .data
  .globl .L..17
.L..17:
  .byte 48
  .byte 60
  .byte 61
  .byte 49
  .byte 0
  .data
  .globl .L..16
.L..16:
  .byte 50
  .byte 60
  .byte 49
  .byte 0
  .data
  .globl .L..15
.L..15:
  .byte 49
  .byte 60
  .byte 49
  .byte 0
  .data
  .globl .L..14
.L..14:
  .byte 48
  .byte 60
  .byte 49
  .byte 0
  .data
  .globl .L..13
.L..13:
  .byte 52
  .byte 50
  .byte 33
  .byte 61
  .byte 52
  .byte 50
  .byte 0
  .data
  .globl .L..12
.L..12:
  .byte 48
  .byte 33
  .byte 61
  .byte 49
  .byte 0
  .data
  .globl .L..11
.L..11:
  .byte 52
  .byte 50
  .byte 61
  .byte 61
  .byte 52
  .byte 50
  .byte 0
  .data
  .globl .L..10
.L..10:
  .byte 48
  .byte 61
  .byte 61
  .byte 49
  .byte 0
  .data
  .globl .L..9
.L..9:
  .byte 45
  .byte 32
  .byte 45
  .byte 32
  .byte 43
  .byte 49
  .byte 48
  .byte 0
  .data
  .globl .L..8
.L..8:
  .byte 45
  .byte 32
  .byte 45
  .byte 49
  .byte 48
  .byte 0
  .data
  .globl .L..7
.L..7:
  .byte 45
  .byte 49
  .byte 48
  .byte 43
  .byte 50
  .byte 48
  .byte 0
  .data
  .globl .L..6
.L..6:
  .byte 40
  .byte 51
  .byte 43
  .byte 53
  .byte 41
  .byte 47
  .byte 50
  .byte 0
  .data
  .globl .L..5
.L..5:
  .byte 53
  .byte 42
  .byte 40
  .byte 57
  .byte 45
  .byte 54
  .byte 41
  .byte 0
  .data
  .globl .L..4
.L..4:
  .byte 53
  .byte 43
  .byte 54
  .byte 42
  .byte 55
  .byte 0
  .data
  .globl .L..3
.L..3:
  .byte 49
  .byte 50
  .byte 32
  .byte 43
  .byte 32
  .byte 51
  .byte 52
  .byte 32
  .byte 45
  .byte 32
  .byte 53
  .byte 0
  .data
  .globl .L..2
.L..2:
  .byte 53
  .byte 43
  .byte 50
  .byte 48
  .byte 45
  .byte 52
  .byte 0
  .data
  .globl .L..1
.L..1:
  .byte 52
  .byte 50
  .byte 0
  .data
  .globl .L..0
.L..0:
  .byte 48
  .byte 0
  .globl main
  .text
main:
  push rbp
  mov rbp, rsp
  sub rsp, 0
  mov rax, 0
  push rax
  mov rax, 0
  push rax
  lea rax, .L..0[rip]
  push rax
  pop rdx
  pop rsi
  pop rdi
  mov rax, 3
  call assert
  mov rax, 42
  push rax
  mov rax, 42
  push rax
  lea rax, .L..1[rip]
  push rax
  pop rdx
  pop rsi
  pop rdi
  mov rax, 3
  call assert
  mov rax, 21
  push rax
  mov rax, 4
  push rax
  mov rax, 20
  push rax
  mov rax, 5
  pop rdi
  add rax, rdi
  pop rdi
  sub rax, rdi
  push rax
  lea rax, .L..2[rip]
  push rax
  pop rdx
  pop rsi
  pop rdi
  mov rax, 3
  call assert
  mov rax, 41
  push rax
  mov rax, 5
  push rax
  mov rax, 34
  push rax
  mov rax, 12
  pop rdi
  add rax, rdi
  pop rdi
  sub rax, rdi
  push rax
  lea rax, .L..3[rip]
  push rax
  pop rdx
  pop rsi
  pop rdi
  mov rax, 3
  call assert
  mov rax, 47
  push rax
  mov rax, 7
  push rax
  mov rax, 6
  pop rdi
  imul rax, rdi
  push rax
  mov rax, 5
  pop rdi
  add rax, rdi
  push rax
  lea rax, .L..4[rip]
  push rax
  pop rdx
  pop rsi
  pop rdi
  mov rax, 3
  call assert
  mov rax, 15
  push rax
  mov rax, 6
  push rax
  mov rax, 9
  pop rdi
  sub rax, rdi
  push rax
  mov rax, 5
  pop rdi
  imul rax, rdi
  push rax
  lea rax, .L..5[rip]
  push rax
  pop rdx
  pop rsi
  pop rdi
  mov rax, 3
  call assert
  mov rax, 4
  push rax
  mov rax, 2
  push rax
  mov rax, 5
  push rax
  mov rax, 3
  pop rdi
  add rax, rdi
  pop rdi
  cqo
  idiv rdi
  push rax
  lea rax, .L..6[rip]
  push rax
  pop rdx
  pop rsi
  pop rdi
  mov rax, 3
  call assert
  mov rax, 10
  push rax
  mov rax, 20
  push rax
  mov rax, 10
  neg rax
  pop rdi
  add rax, rdi
  push rax
  lea rax, .L..7[rip]
  push rax
  pop rdx
  pop rsi
  pop rdi
  mov rax, 3
  call assert
  mov rax, 10
  push rax
  mov rax, 10
  neg rax
  neg rax
  push rax
  lea rax, .L..8[rip]
  push rax
  pop rdx
  pop rsi
  pop rdi
  mov rax, 3
  call assert
  mov rax, 10
  push rax
  mov rax, 10
  neg rax
  neg rax
  push rax
  lea rax, .L..9[rip]
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
  mov rax, 0
  pop rdi
  cmp rax, rdi
  sete al
  movzb rax, al
  push rax
  lea rax, .L..10[rip]
  push rax
  pop rdx
  pop rsi
  pop rdi
  mov rax, 3
  call assert
  mov rax, 1
  push rax
  mov rax, 42
  push rax
  mov rax, 42
  pop rdi
  cmp rax, rdi
  sete al
  movzb rax, al
  push rax
  lea rax, .L..11[rip]
  push rax
  pop rdx
  pop rsi
  pop rdi
  mov rax, 3
  call assert
  mov rax, 1
  push rax
  mov rax, 1
  push rax
  mov rax, 0
  pop rdi
  cmp rax, rdi
  setne al
  movzb rax, al
  push rax
  lea rax, .L..12[rip]
  push rax
  pop rdx
  pop rsi
  pop rdi
  mov rax, 3
  call assert
  mov rax, 0
  push rax
  mov rax, 42
  push rax
  mov rax, 42
  pop rdi
  cmp rax, rdi
  setne al
  movzb rax, al
  push rax
  lea rax, .L..13[rip]
  push rax
  pop rdx
  pop rsi
  pop rdi
  mov rax, 3
  call assert
  mov rax, 1
  push rax
  mov rax, 1
  push rax
  mov rax, 0
  pop rdi
  cmp rax, rdi
  setl al
  movzb rax, al
  push rax
  lea rax, .L..14[rip]
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
  mov rax, 1
  pop rdi
  cmp rax, rdi
  setl al
  movzb rax, al
  push rax
  lea rax, .L..15[rip]
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
  mov rax, 2
  pop rdi
  cmp rax, rdi
  setl al
  movzb rax, al
  push rax
  lea rax, .L..16[rip]
  push rax
  pop rdx
  pop rsi
  pop rdi
  mov rax, 3
  call assert
  mov rax, 1
  push rax
  mov rax, 1
  push rax
  mov rax, 0
  pop rdi
  cmp rax, rdi
  setle al
  movzb rax, al
  push rax
  lea rax, .L..17[rip]
  push rax
  pop rdx
  pop rsi
  pop rdi
  mov rax, 3
  call assert
  mov rax, 1
  push rax
  mov rax, 1
  push rax
  mov rax, 1
  pop rdi
  cmp rax, rdi
  setle al
  movzb rax, al
  push rax
  lea rax, .L..18[rip]
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
  mov rax, 2
  pop rdi
  cmp rax, rdi
  setle al
  movzb rax, al
  push rax
  lea rax, .L..19[rip]
  push rax
  pop rdx
  pop rsi
  pop rdi
  mov rax, 3
  call assert
  mov rax, 1
  push rax
  mov rax, 1
  push rax
  mov rax, 0
  pop rdi
  cmp rax, rdi
  setl al
  movzb rax, al
  push rax
  lea rax, .L..20[rip]
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
  mov rax, 1
  pop rdi
  cmp rax, rdi
  setl al
  movzb rax, al
  push rax
  lea rax, .L..21[rip]
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
  mov rax, 2
  pop rdi
  cmp rax, rdi
  setl al
  movzb rax, al
  push rax
  lea rax, .L..22[rip]
  push rax
  pop rdx
  pop rsi
  pop rdi
  mov rax, 3
  call assert
  mov rax, 1
  push rax
  mov rax, 1
  push rax
  mov rax, 0
  pop rdi
  cmp rax, rdi
  setle al
  movzb rax, al
  push rax
  lea rax, .L..23[rip]
  push rax
  pop rdx
  pop rsi
  pop rdi
  mov rax, 3
  call assert
  mov rax, 1
  push rax
  mov rax, 1
  push rax
  mov rax, 1
  pop rdi
  cmp rax, rdi
  setle al
  movzb rax, al
  push rax
  lea rax, .L..24[rip]
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
  mov rax, 2
  pop rdi
  cmp rax, rdi
  setle al
  movzb rax, al
  push rax
  lea rax, .L..25[rip]
  push rax
  pop rdx
  pop rsi
  pop rdi
  mov rax, 3
  call assert
  lea rax, .L..26[rip]
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
