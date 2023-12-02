#include "9cc.h"

static int depth;
static char *argreg[] = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};
static Obj *current_fn;

static void gen_expr(Node *node);

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
    switch (node->kind) {
    case ND_VAR:
        if (node->var->is_local) {
            // Local variable
            printf("  lea rax, [rbp + %d]\n", node->var->offset);
        } else {
            // Global variable
            printf("  lea rax, [rip + %s]\n", node->var->name);
        }
        return;
    case ND_DEREF:
        gen_expr(node->lhs);
        return;
    }

    error_tok(node->tok, "not an lvalue");
}

// Load a value from where rax is pointing to.
static void load(Type *ty) {
    if (ty->kind == TY_ARRAY) {
        return;
    }
    printf("  mov rax, [rax]\n");
}

// Store rax to an address that the stack top is pointing to.
static void store(void) {
    pop("rdi\n");
    printf("  mov [rdi], rax\n");
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
        load(node->ty);
        return;
    case ND_ASSIGN:
        gen_addr(node->lhs);
        push();
        gen_expr(node->rhs);
        store();
        return;
    case ND_DEREF:
        gen_expr(node->lhs);
        load(node->ty);
        return;
    case ND_ADDR:
        gen_addr(node->lhs);
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

static void assign_lvar_offsets(Obj *prog) {
    for (Obj *fn = prog; fn; fn = fn->next) {
        if (!fn->is_function)
            continue;

        int offset = 0;
        for (Obj *var = fn->locals; var; var = var->next) {
            offset += var->ty->size;
            var->offset = -offset;
        }
        fn->stack_size = align_to(offset, 16);   
    }
}

void codegen(Obj *prog) {
    assign_lvar_offsets(prog);

    printf(".intel_syntax noprefix\n");

    for (Obj *fn = prog; fn; fn = fn->next) {
        if (fn->is_function)
            continue;
        printf("  .data\n");
        printf("  .globl %s\n", fn->name);
        printf("%s:\n", fn->name);
        printf("  .zero %d\n", fn->ty->size);
    }

    for (Obj *fn = prog; fn; fn = fn->next) {
        if (fn->is_function) {
            printf("  .globl %s\n", fn->name);
            printf("  .text\n");
            printf("%s:\n", fn->name);
            current_fn = fn;
            
            // Prologue
            printf("  push rbp\n");
            printf("  mov rbp, rsp\n");
            printf("  sub rsp, %d\n", fn->stack_size);
            int i = 0;
            for (Obj *var = fn->params; var; var = var->next)
                printf("  mov [rbp + %d], %s\n", var->offset, argreg[i++]);

            gen_stmt(fn->body);
            assert(depth == 0);

            // Epilogue
            printf(".L.return.%s:\n", fn->name);
            printf("  mov rsp, rbp\n");
            printf("  pop rbp\n");
            printf("  ret\n");
        }
    }
}
