# include "9cc.h"

Node *stmt(Token **rest, Token *tok);
Node *expr(Token **rest, Token *tok);
Node *assign(Token **rest, Token *tok);
Node *equality(Token **rest, Token *tok);
Node *relational(Token **rest, Token *tok);
Node *add(Token **rest, Token *tok);
Node *mul(Token **rest, Token *tok);
Node *unary(Token **rest, Token *tok);
Node *primary(Token **rest, Token *tok);

bool at_eof(Token *tok) {
    return tok->kind == TK_EOF;
}

Node *new_node(NodeKind kind) {
    Node *node = calloc(1, sizeof(Node));
    node->kind = kind;
    return node;
}

Node *new_binary(NodeKind kind, Node *lhs, Node *rhs) {
    Node *node = new_node(kind);
    node->lhs = lhs;
    node->rhs = rhs;
    return node;
}

Node *new_num(int val) {
    Node *node= new_node(ND_NUM);
    node->val = val;
    return node;
}

LVar *find_lvar(Token *tok) {
    for (LVar *var = locals; var; var = var->next) {
        if (var->len == tok->len && !memcmp(tok->loc, var->name, var->len))
            return var;
    }
    return NULL;
}

void program(Token *tok) {
    LVar head;
    head.next = NULL;
    locals = &head;

    int i = 0;
    while (!at_eof(tok)){
        code[i++] = stmt(&tok, tok);
    }
    code[i] = NULL;
}

Node *stmt(Token **rest, Token *tok) {
    Node *node;
    
    if (equal(tok, "return")) {
        // return文の場合
        node = calloc(1, sizeof(Node));
        node->kind = ND_RETURN;
        node->lhs = expr(&tok, tok->next);
        
        *rest = skip(tok, ";");
        
        return node;
    } else if (equal(tok, "if")) {
        // if文の場合
        tok = skip(tok->next, "(");
        Node *cond_node = expr(&tok, tok);
        tok = skip(tok, ")");
        Node *stmt_node = stmt(&tok, tok);
        node = calloc(1, sizeof(Node));
        node->kind = ND_IF;
        node->lhs = stmt_node;  // lhsをthen節とする
        node->cond = cond_node;
        // else節がある場合
        if (equal(tok, "else")) {
            node->kind = ND_IFELSE;
            Node *else_stmt_node = stmt(&tok, tok->next);
            node->rhs = else_stmt_node;  // rhsをelsen節とする
        }
        *rest = tok;
        return node;
    } else if (equal(tok, "while")) {
        // while文の場合
        tok = skip(tok->next, "(");
        Node *cond_node = expr(&tok, tok);
        tok = skip(tok, ")");
        Node *stmt_node = stmt(rest, tok);
        node = calloc(1, sizeof(Node));
        node->kind = ND_WHILE;
        node->lhs = stmt_node;
        node->cond = cond_node;
        return node;
    } else if (equal(tok, "for")) {
        // for文の場合
        node = calloc(1, sizeof(Node));
        node->kind = ND_FOR;
        tok = skip(tok->next, "(");
        if (!equal(tok, ";")) {
            node->for_init = expr(&tok, tok);
        }
        tok = skip(tok, ";");
        if (!equal(tok, ";")) {
            node->cond = expr(&tok, tok);  // 終了判定部分
        }
        tok = skip(tok, ";");
        if (!equal(tok, ")")) {
            node->lhs = expr(&tok, tok);  // 更新部分
        }
        tok = skip(tok, ")");
        node->rhs = stmt(rest, tok);  // 本体
        return node;
    } else if (equal(tok, "{")) {
        // 複文の場合
        Node head = {};
        Node *cur = &head;
        tok = tok->next;
        while (!equal(tok, "}")) {
            cur->next = stmt(&tok, tok);
            cur = cur->next;
        }
        *rest = tok->next;
        node = calloc(1, sizeof(Node));
        node->kind = ND_BLOCK;
        node->body = head.next;
        return node;
    } else if (equal(tok, ";")) {
        node = calloc(1, sizeof(Node));
        node->kind = ND_BLOCK;
        *rest = tok->next;
        return node;
    } else {
        node = expr(&tok, tok);
        *rest = skip(tok, ";");
        return node;
    }
}

Node *expr(Token **rest, Token *tok) {
    return assign(rest, tok);
}

Node *assign(Token **rest, Token *tok) {    
    Node *node = equality(&tok, tok);

    if (equal(tok, "=")) {
        node = new_binary(ND_ASSIGN, node, assign(&tok, tok->next));
    }
    *rest = tok;
    return node;
}

Node *equality(Token **rest, Token *tok) {
    Node *node = relational(&tok, tok);

    for (;;) {
        if (equal(tok, "==")) {
            node = new_binary(ND_EQ, node, relational(&tok, tok->next));
            continue;
        }
        if (equal(tok, "!=")) {
            node = new_binary(ND_NE, node, relational(&tok, tok->next));
            continue;
        }
        *rest = tok;
        return node;
    }
}

Node *relational(Token **rest, Token *tok) {
    Node *node = add(&tok, tok);

    for (;;) {
        if (equal(tok, "<")) {
            node = new_binary(ND_LT, node, add(&tok, tok->next));
            continue;
        }
        if (equal(tok, "<=")) {
            node = new_binary(ND_LE, node, add(&tok, tok->next));
            continue;
        }
        if (equal(tok, ">")) {
            node = new_binary(ND_LT, add(&tok, tok->next), node);
            continue;
        }
        if (equal(tok, ">=")) {
            node = new_binary(ND_LE, add(&tok, tok->next), node);
            continue;
        }
        *rest = tok;
        return node;
    }
}

Node *add(Token **rest, Token *tok) {
    Node *node = mul(&tok, tok);

    for (;;) {
        if (equal(tok, "+")) {
            node = new_binary(ND_ADD, node, mul(&tok, tok->next));
            continue;
        }
        if (equal(tok, "-")) {
            node = new_binary(ND_SUB, node, mul(&tok, tok->next));
            continue;
        }
        *rest = tok;
        return node;
    }
}

Node *mul(Token **rest, Token *tok) {
    Node *node = unary(&tok, tok);

    for (;;) {
        if (equal(tok, "*")) {
            node = new_binary(ND_MUL, node, unary(&tok, tok->next));
            continue;
        }
        if (equal(tok, "/")) {
            node = new_binary(ND_DIV, node, unary(&tok, tok->next));
            continue;
        }
        *rest = tok;
        return node;
    }
}

Node *unary(Token **rest, Token *tok) {
    if (equal(tok, "+")) {
        return unary(rest, tok->next);
    }
    if (equal(tok, "-")) {
        return new_binary(ND_SUB, new_num(0), unary(rest, tok->next));
    }
    return primary(rest, tok);
}

Node *primary(Token **rest, Token *tok) {
    // 次のトークンが"("なら、"(" expr ")"のはず
    if (equal(tok, "(")) {
        Node *node = expr(&tok, tok->next);
        tok = skip(tok, ")");
        *rest = tok;
        return node;
    }

    // 次のトークンが識別子の場合
    if (tok->kind == TK_IDENT) {

        Node *node = calloc(1, sizeof(Node));
        node->kind = ND_LVAR;

        LVar *lvar = find_lvar(tok);
        if (lvar) {
            node->offset = lvar->offset;
        } else {
            lvar = calloc(1, sizeof(LVar));
            lvar->next = locals;
            lvar->name = tok->loc;
            lvar->len = tok->len;
            lvar->offset = locals->offset + 8;
            node->offset = lvar->offset;
            locals = lvar;
        }
        *rest = tok->next;
        return node;
    }

    // そうでなければ数値のはず
    if (tok->kind == TK_NUM) {
        
        Node *node = new_num(tok->val);
        *rest = tok->next;
        return node;
    }

    error_tok(tok, "expected an expression");
}
