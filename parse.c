#include "9cc.h"

Obj *locals;

static Node *compound_stmt(Token **rest, Token *tok);
static Node *expr_stmt(Token **rest, Token *tok);
static Node *expr(Token **rest, Token *tok);
static Node *assign(Token **rest, Token *tok);
static Node *equality(Token **rest, Token *tok);
static Node *relational(Token **rest, Token *tok);
static Node *add(Token **rest, Token *tok);
static Node *mul(Token **rest, Token *tok);
static Node *unary(Token **rest, Token *tok);
static Node *primary(Token **rest, Token *tok);

// ローカル変数を名前で探す
static Obj *find_var(Token *tok) {
    for (Obj *var = locals; var; var = var->next)
        if (strlen(var->name) == tok->len && !strncmp(tok->loc, var->name, tok->len))
            return var;
    return NULL;
}

static Node *new_node(NodeKind kind, Token *tok) {
    Node *node = calloc(1, sizeof(Node));
    node->kind = kind;
    node->tok = tok;
    return node;
}

static Node *new_binary(NodeKind kind, Node *lhs, Node *rhs, Token *tok) {
    Node *node = new_node(kind, tok);
    node->lhs = lhs;
    node->rhs = rhs;
    return node;
}

static Node *new_unary(NodeKind kind, Node *expr, Token *tok) {
    Node *node = new_node(kind, tok);
    node->lhs = expr;
    return node;
}

static Node *new_num(int val, Token *tok) {
    Node *node = new_node(ND_NUM, tok);
    node->val = val;
    return node;
}

static Node *new_var_node(Obj *var, Token *tok) {
    Node *node = new_node(ND_VAR, tok);
    node->var = var;
    return node;
}

static Obj *new_lvar(char *name) {
    Obj *var = calloc(1, sizeof(Obj));
    var->name = name;
    var->next = locals;
    locals = var;
    return var;
}

// stmt = "return" expr ";"
//      | "if" "(" expr ")" stmt ("else" stmt)?
//      | "for" "(" expr-stmt expr? ";" expr? ")" stmt
//      | "while" "(" expr ")" stmt
//      | "{" compound-stmt
//      | expr-stmt
static Node *stmt(Token **rest, Token *tok) {
    if (equal(tok, "return")) {
        Node *node = new_node(ND_RETURN, tok);
        node->lhs = expr(&tok, tok->next);
        *rest = skip(tok, ";");
        return node;
    }

    if (equal(tok, "if")) {
        Node *node = new_node(ND_IF, tok);
        tok = skip(tok->next, "(");
        node->cond = expr(&tok, tok);
        tok = skip(tok, ")");
        node->then = stmt(&tok, tok);
        if (equal(tok, "else"))
            node->els = stmt(&tok, tok->next);
        *rest = tok;
        return node;
    }

    if (equal(tok, "for")) {
        Node *node = new_node(ND_FOR, tok);
        tok = skip(tok->next, "(");

        node->init = expr_stmt(&tok, tok);

        if (!equal(tok, ";"))
            node->cond = expr(&tok, tok);
        tok = skip(tok, ";");

        if (!equal(tok, ")"))
            node->inc = expr(&tok, tok);
        tok = skip(tok, ")");

        node->then = stmt(rest, tok);
        return node;
    }

    if (equal(tok, "while")) {
        Node *node = new_node(ND_FOR, tok);
        tok = skip(tok->next, "(");
        node->cond = expr(&tok, tok);
        tok = skip(tok, ")");
        node->then = stmt(rest, tok);
        return node;
    }
    if (equal(tok, "{"))
        return compound_stmt(rest, tok->next);

    return expr_stmt(rest, tok);
}

// compound-stmt = stmt* "}"
static Node *compound_stmt(Token **rest, Token *tok) {
    Node *node = new_node(ND_BLOCK, tok);

    Node head = {};
    Node *cur = &head;
    while (!equal(tok, "}"))
        cur = cur->next = stmt(&tok, tok);
    
    node->body = head.next;
    *rest = tok->next;
    return node;
}

// expr-stmt = expr? ";"
static Node *expr_stmt(Token **rest, Token *tok) {
    if (equal(tok, ";")) {
        *rest = tok->next;
        return new_node(ND_BLOCK, tok);
    }

    Node *node = new_node(ND_EXPR_STMT, tok);
    node->lhs = expr(&tok, tok);
    *rest = skip(tok, ";");
    return node;
}

// expr = assign
static Node *expr(Token **rest, Token *tok) {
    return assign(rest, tok);
}

// assign = equality ("=" assign)?
static Node *assign(Token **rest, Token *tok) {
    Node *node = equality(&tok, tok);

    if (equal(tok, "="))
        node = new_binary(ND_ASSIGN, node, assign(&tok, tok->next), tok);
    *rest = tok;
    return node;
}

// equality = relational ("==" relational | "!=" relational)*
static Node *equality(Token **rest, Token *tok) {
    Node *node =relational(&tok, tok);

    for (;;) {
        Token *start = tok;

        if (equal(tok, "==")) {
            node = new_binary(ND_EQ, node, relational(&tok, tok->next), start);
            continue;
        }
        if (equal(tok, "!=")) {
            node = new_binary(ND_NE, node, relational(&tok, tok->next), start);
            continue;
        }
        *rest = tok;
        return node;
            
    }
}

// relational = add ("<" add | "<=" add | ">" add | ">=" add)*
static Node *relational(Token **rest, Token *tok) {
    Node *node = add(&tok, tok);

    for (;;) {
        Token *start = tok;

        if (equal(tok, "<")) {
            node = new_binary(ND_LT, node, add(&tok, tok->next), start);
            continue;
        }
        if (equal(tok, "<=")) {
            node = new_binary(ND_LE, node, add(&tok, tok->next), start);
            continue;
        }
        if (equal(tok, ">")) {
            node = new_binary(ND_LT, add(&tok, tok->next), node, start);
            continue;
        }
        if (equal(tok, ">=")) {
            node = new_binary(ND_LE, add(&tok, tok->next), node, start);
            continue;
        }
            
        *rest = tok;
        return node;
    }
}

// add = mul ("+" mul | "-" mul)*
static Node *add(Token **rest, Token *tok) {
    Node *node = mul(&tok, tok);

    for (;;) {
        Token *start = tok;

        if (equal(tok, "+")) {
            node = new_binary(ND_ADD, node, mul(&tok, tok->next), start);
            continue;
        }
            
        if (equal(tok, "-")) {
            node = new_binary(ND_SUB, node, mul(&tok, tok->next), start);
            continue;
        }
        *rest = tok;
        return node;
    }
}

// mul = unary ("*" unary | "/" unary)*
static Node *mul(Token **rest, Token *tok) {
    Node *node = unary(&tok, tok);

    for (;;) {
        Token *start = tok;

        if (equal(tok, "*")) {
            node = new_binary(ND_MUL, node, unary(&tok, tok->next), start);
            continue;
        }
            
        if (equal(tok, "/")) {
            node = new_binary(ND_DIV, node, unary(&tok, tok->next), start);
            continue;
        }
        *rest = tok;            
        return node;
    }
}

// unary = ("+" | "-" | "*" | "&") unary
//       | primary
static Node *unary(Token **rest, Token *tok) {
    if (equal(tok, "+"))
        return unary(rest, tok->next);
    if (equal(tok, "-"))
        return new_unary(ND_NEG, unary(rest, tok->next), tok);
    if (equal(tok, "&"))
        return new_unary(ND_ADDR, unary(rest, tok->next), tok);
    if (equal(tok, "*"))
        return new_unary(ND_DEREF, unary(rest, tok->next), tok);
    return primary(rest, tok);
}

// funcall = ident "(" (assign ("," assign)*)? ")"
static Node *funcall(Token **rest, Token *tok) {
    Token *start = tok;
    tok = tok->next->next;

    Node head = {};
    Node *cur = &head;

    while (!equal(tok, ")")) {
        if (cur != &head)
            tok = skip(tok, ",");
        cur = cur->next = assign(&tok, tok);
    }

    *rest = skip(tok, ")");

    Node *node = new_node(ND_FUNCALL, start);
    node->funcname = strndup(start->loc, start->len);
    node->args = head.next;
    return node;
}

// primary = "(" expr ")" | ident func-args? | num
static Node *primary(Token **rest, Token *tok) {
    // 次のトークンが"("なら、"(" expr ")"のはず
    if (equal(tok, "(")) {
        Node *node = expr(&tok, tok->next);
        *rest = skip(tok, ")");
        return node;
    }

    if (tok->kind == TK_IDENT) {
        // "("があれば関数呼び出し
        if (equal(tok->next, "("))
            return funcall(rest, tok);
        
        // 変数
        Obj *var = find_var(tok);
        if (!var)
            var = new_lvar(strndup(tok->loc, tok->len));
        *rest = tok->next;
        return new_var_node(var, tok);
    }

    if (tok->kind == TK_NUM) {
        Node *node = new_num(tok->val, tok);
        *rest = tok->next;
        return node;
    }

    error_tok(tok, "expected an expression");
}

static Function* function(Token **rest, Token *tok) {
    locals = NULL;

    Function *fn = calloc(1, sizeof(Function));
    fn->name = strndup(tok->loc, tok->len);
    tok = skip(tok->next, "(");

    char *p[6] = {};
    int i = 0;
    while (!equal(tok, ")")) {
        p[i] = strndup(tok->loc, tok->len);
        i++;
        tok = tok->next;
        if (equal(tok, ","))
            tok = skip(tok, ",");
    }

    for (int i = 5; i >= 0; i--)
        if (p[i])
            new_lvar(p[i]);
    
    fn->params = locals;

    tok = skip(tok, ")");
    tok = skip(tok, "{");
    fn->body = compound_stmt(rest, tok);
    fn->locals = locals;
    return fn;
}

// program = stmt*
Function *parse(Token *tok) {
    Function head = {};
    Function *cur = &head;

    while (tok->kind != TK_EOF)
        cur = cur->next = function(&tok, tok);
    return head.next;
}
