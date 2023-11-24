#include "9cc.h"

static int depth;
static char *argreg[] = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};
static Function *current_fn;

static int count(void) {
    static int i = 1;
    return i++;
}

static void push(void) {
    printf("  push rax\n");
    depth++;
}

static void pop(char *arg) {
    printf("  pop %s\n", arg);
    depth--;
}

static int align_to(int n, int align) {
    return (n + align - 1) / align * align;
}

static void gen_addr(Node *node) {
    if (node->kind == ND_VAR) {
        printf("  lea rax, [rbp + %d]\n", node->var->offset);
        return;
    }

    error_tok(node->tok, "not an lvalue");
}

static void gen_expr(Node *node) {
    switch (node->kind) {
    case ND_NUM:
        printf("  mov rax, %d\n", node->val);
        return;
    case ND_NEG:
        gen_expr(node->lhs);
        printf("  neg rax\n");
        return;
    case ND_VAR:
        gen_addr(node);
        printf("  mov rax, [rax]\n");
        return;
    case ND_ASSIGN:
        gen_addr(node->lhs);
        push();
        gen_expr(node->rhs);
        pop("rdi");
        printf("  mov [rdi], rax\n");
        return;
    case ND_FUNCALL: {
        int nargs = 0;
        for (Node *args = node->args; args; args = args->next) {
            gen_expr(args);
            push();
            nargs++;
        }

        for (int i = nargs - 1; i >= 0; i--)
            pop(argreg[i]);
        
        printf("  mov rax, %d\n", nargs);
        printf("  call %s\n", node->funcname);
        return;
    }
    }

    gen_expr(node->rhs);
    push();
    gen_expr(node->lhs);
    pop("rdi");

    switch (node->kind) {
    case ND_ADD:
        printf("  add rax, rdi\n");
        return;
    case ND_SUB:
        printf("  sub rax, rdi\n");
        return;
    case ND_MUL:
        printf("  imul rax, rdi\n");
        return;
    case ND_DIV:
        printf("  cqo\n");
        printf("  idiv rdi\n");
        return;
    case ND_EQ:
    case ND_NE:
    case ND_LT:
    case ND_LE:
        printf("  cmp rax, rdi\n");
        if (node->kind == ND_EQ)
            printf("  sete al\n");
        else if (node->kind == ND_NE)
            printf("  setne al\n");
        else if (node->kind == ND_LT)
            printf("  setl al\n");
        else if (node->kind == ND_LE)
            printf("  setle al\n");
        printf("  movzb rax, al\n");
        return;
    }

    error_tok(node->tok, "invalid expression");
}

static void gen_stmt(Node *node) {
    switch (node->kind) {
    case ND_IF: {
        int c = count();
        gen_expr(node->cond);
        printf("  cmp rax, 0\n");
        printf("  je .L.else.%d\n", c);
        gen_stmt(node->then);
        printf("  jmp .L.end.%d\n", c);
        printf(".L.else.%d:\n", c);
        if (node->els)
            gen_stmt(node->els);
        printf(".L.end.%d:\n", c);
        return;
    }
    case ND_FOR: {
        int c = count();
        if (node->init)
            gen_stmt(node->init);
        printf(".L.begin.%d:\n", c);
        if (node->cond) {
            gen_expr(node->cond);
            printf("  cmp rax, 0\n");
            printf("  je .L.end.%d\n", c);
        }
        gen_stmt(node->then);
        if (node->inc)
            gen_expr(node->inc);
        printf("  jmp .L.begin.%d\n", c);
        printf(".L.end.%d:\n", c);
        return;
    }
    case ND_BLOCK:
        for (Node *n = node->body; n; n= n->next)
            gen_stmt(n);
        return;
    case ND_RETURN:
        gen_expr(node->lhs);
        printf("  jmp .L.return.%s\n", current_fn->name);
        return;
    case ND_EXPR_STMT:
        gen_expr(node->lhs);
        return;
    }

    error_tok(node->tok, "invalud statement");
}

static void assign_lvar_offsets(Function *prog) {
    for (Function *fn = prog; fn; fn = fn->next) {
        int offset = 0;
        for (Obj *var = fn->locals; var; var = var->next) {
            offset += 8;
            var->offset = -offset;
        }
        fn->stack_size = align_to(offset, 16);   
    }
}

void codegen(Function *prog) {
    assign_lvar_offsets(prog);

    printf(".intel_syntax noprefix\n");

    for (Function *fn = prog; fn; fn = fn->next) {
        printf("  .globl %s\n", fn->name);
        printf("%s:\n", fn->name);
        current_fn = fn;
        
        // Prologue
        printf("  push rbp\n");
        printf("  mov rbp, rsp\n");
        printf("  sub rsp, %d\n", fn->stack_size);

        gen_stmt(fn->body);
        assert(depth == 0);

        // Epilogue
        printf(".L.return.%s:\n", fn->name);
        printf("  mov rsp, rbp\n");
        printf("  pop rbp\n");
        printf("  ret\n");
    }
}
