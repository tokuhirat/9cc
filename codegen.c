#include "9cc.h"

static int depth;
static char *argreg8[] = {"dil", "sil", "dl", "cl", "r8b", "r9b"};
static char *argreg64[] = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};
static Obj *current_fn;

static void gen_expr(Node *node);

static void println(char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    printf("\n");
}

static int count(void) {
    static int i = 1;
    return i++;
}

static void push(void) {
    println("  push rax");
    depth++;
}

static void pop(char *arg) {
    println("  pop %s", arg);
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
            println("  lea rax, [rbp + %d]", node->var->offset);
        } else {
            // Global variable
            println("  lea rax, %s[rip]", node->var->name);
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
    if (ty->size == 1)
        println("  movsx eax, BYTE PTR [rax]");
    else
        println("  mov rax, [rax]");
}

// Store rax to an address that the stack top is pointing to.
static void store(Type *ty) {
    pop("rdi");

    if (ty->size == 1)
        println("  mov [rdi], al");
    else
        println("  mov [rdi], rax");
}

static void gen_expr(Node *node) {
    switch (node->kind) {
    case ND_NUM:
        println("  mov rax, %d", node->val);
        return;
    case ND_NEG:
        gen_expr(node->lhs);
        println("  neg rax");
        return;
    case ND_VAR:
        gen_addr(node);
        load(node->ty);
        return;
    case ND_ASSIGN:
        gen_addr(node->lhs);
        push();
        gen_expr(node->rhs);
        store(node->ty);
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
            pop(argreg64[i]);
        
        println("  mov rax, %d", nargs);
        println("  call %s", node->funcname);
        return;
    }
    }

    gen_expr(node->rhs);
    push();
    gen_expr(node->lhs);
    pop("rdi");

    switch (node->kind) {
    case ND_ADD:
        println("  add rax, rdi");
        return;
    case ND_SUB:
        println("  sub rax, rdi");
        return;
    case ND_MUL:
        println("  imul rax, rdi");
        return;
    case ND_DIV:
        println("  cqo");
        println("  idiv rdi");
        return;
    case ND_EQ:
    case ND_NE:
    case ND_LT:
    case ND_LE:
        println("  cmp rax, rdi");
        if (node->kind == ND_EQ)
            println("  sete al");
        else if (node->kind == ND_NE)
            println("  setne al");
        else if (node->kind == ND_LT)
            println("  setl al");
        else if (node->kind == ND_LE)
            println("  setle al");
        println("  movzb rax, al");
        return;
    }

    error_tok(node->tok, "invalid expression");
}

static void gen_stmt(Node *node) {
    switch (node->kind) {
    case ND_IF: {
        int c = count();
        gen_expr(node->cond);
        println("  cmp rax, 0");
        println("  je .L.else.%d", c);
        gen_stmt(node->then);
        println("  jmp .L.end.%d", c);
        println(".L.else.%d:", c);
        if (node->els)
            gen_stmt(node->els);
        println(".L.end.%d:", c);
        return;
    }
    case ND_FOR: {
        int c = count();
        if (node->init)
            gen_stmt(node->init);
        println(".L.begin.%d:", c);
        if (node->cond) {
            gen_expr(node->cond);
            println("  cmp rax, 0");
            println("  je .L.end.%d", c);
        }
        gen_stmt(node->then);
        if (node->inc)
            gen_expr(node->inc);
        println("  jmp .L.begin.%d", c);
        println(".L.end.%d:", c);
        return;
    }
    case ND_BLOCK:
        for (Node *n = node->body; n; n= n->next)
            gen_stmt(n);
        return;
    case ND_RETURN:
        gen_expr(node->lhs);
        println("  jmp .L.return.%s", current_fn->name);
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

static void emit_data(Obj *prog) {
    for (Obj *var = prog; var; var = var->next) {
        if (var->is_function)
            continue;

        println("  .data");
        println("  .globl %s", var->name);
        println("%s:", var->name);

        if (var->init_data) {
            for (int i = 0; i < var->ty->size; i++)
                println("  .byte %d", var->init_data[i]);
        } else {
            println("  .zero %d", var->ty->size);
        }
    }
}

static void emit_text(Obj *prog) {
    for (Obj *fn = prog; fn; fn = fn->next) {
        if (!fn->is_function)
            continue;

        println("  .globl %s", fn->name);
        println("  .text");
        println("%s:", fn->name);
        current_fn = fn;
        
        // Prologue
        println("  push rbp");
        println("  mov rbp, rsp");
        println("  sub rsp, %d", fn->stack_size);
        
        int i = 0;
        for (Obj *var = fn->params; var; var = var->next) {
            if (var->ty->size == 1)
                println("  mov [rbp + %d], %s", var->offset, argreg8[i++]);
            else
                println("  mov [rbp + %d], %s", var->offset, argreg64[i++]);
        }

        gen_stmt(fn->body);
        assert(depth == 0);

        // Epilogue
        println(".L.return.%s:", fn->name);
        println("  mov rsp, rbp");
        println("  pop rbp");
        println("  ret");
    }
}

void codegen(Obj *prog) {
    assign_lvar_offsets(prog);

    println(".intel_syntax noprefix");

    emit_data(prog);
    emit_text(prog);
}
