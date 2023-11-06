# include "9cc.h"

Node *stmt();
Node *expr();
Node *assign();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *primary();

bool at_eof() {
    return token->kind == TK_EOF;
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
        if (var->len == tok->len && !memcmp(tok->str, var->name, var->len))
            return var;
    }
    return NULL;
}

void program() {
    LVar head;
    head.next = NULL;
    locals = &head;

    int i = 0;
    while (!at_eof()){
        code[i++] = stmt();
    }
    code[i] = NULL;
}

Node *stmt() {
    Node *node;
    
    if (consume_return()) {
        // return文の場合
        node = calloc(1, sizeof(Node));
        node->kind = ND_RETURN;
        node->lhs = expr();
        expect(";");
        return node;
    } else if (consume_if()) {
        // if文の場合
        expect("(");
        Node *cond_node = expr();
        expect(")");
        Node *stmt_node = stmt();
        node = calloc(1, sizeof(Node));
        node->kind = ND_IF;
        node->lhs = stmt_node;  // lhsをthen節とする
        node->cond = cond_node;
        // else節がある場合
        if (consume_else()) {
            node->kind = ND_IFELSE;
            Node *else_stmt_node = stmt();
            node->rhs = else_stmt_node;  // rhsをelsen節とする
        }
        return node;
    } else if (consume_while()) {
        // while文の場合
        expect("(");
        Node *cond_node = expr();
        expect(")");
        Node *stmt_node = stmt();
        node = calloc(1, sizeof(Node));
        node->kind = ND_WHILE;
        node->lhs = stmt_node;
        node->cond = cond_node;
        return node;
    } else if (consume_for()) {
        // for文の場合
        node = calloc(1, sizeof(Node));
        node->kind = ND_FOR;
        expect("(");
        if (!check_token(";")) {
            node->for_init = expr();
        }
        expect(";");
        if (!check_token(";")) {
            node->cond = expr();  // 終了判定部分
        }
        expect(";");
        if (!consume(")")) {
            node->lhs = expr();  // 更新部分
            expect(")");
        }
        node->rhs = stmt();  // 本体
        return node;
    } else if (consume("{")) {
        // 複文の場合
        Node head = {};
        Node *cur = &head;
        while (!consume("}")) {
            cur->next = stmt();
            cur = cur->next;
        }
        node = calloc(1, sizeof(Node));
        node->kind = ND_BLOCK;
        node->body = head.next;
        return node;
    } else {
        node = expr();
        expect(";");
        return node;
    }
}

Node *expr() {
    return assign();
}

Node *assign() {
    Node *node = equality();

    if (consume("="))
        node = new_binary(ND_ASSIGN, node, assign());
    return node;
}

Node *equality() {
    Node *node = relational();

    for (;;) {
        if (consume("=="))
            node = new_binary(ND_EQ, node, relational());
        else if (consume("!="))
            node = new_binary(ND_NE, node, relational());
        else
            return node;
    }
}

Node *relational() {
    Node *node = add();

    for (;;) {
        if (consume("<"))
            node = new_binary(ND_LT, node, add());
        else if (consume("<="))
            node = new_binary(ND_LE, node, add());
        else if (consume(">"))
            node = new_binary(ND_LT, add(), node);
        else if (consume(">="))
            node = new_binary(ND_LE, add(), node);
        else
            return node;
    }
}

Node *add() {
    Node *node = mul();

    for (;;) {
        if (consume("+"))
            node = new_binary(ND_ADD, node, mul());
        else if (consume("-"))
            node = new_binary(ND_SUB, node, mul());
        else
            return node;
    }
}

Node *mul() {
    Node *node = unary();

    for (;;) {
        if (consume("*"))
            node = new_binary(ND_MUL, node, unary());
        else if (consume("/"))
            node = new_binary(ND_DIV, node, unary());
        else
            return node;
    }
}

Node *unary() {
    if (consume("+"))
        return unary();
    if (consume("-"))
        return new_binary(ND_SUB, new_num(0), unary());
    return primary();
}

Node *primary() {
    // 次のトークンが"("なら、"(" expr ")"のはず
    if (consume("(")) {
        Node *node = expr();
        expect(")");
        return node;
    }

    // 次のトークンが識別子の場合
    Token *tok = consume_ident();
    if (tok) {
        Node *node = calloc(1, sizeof(Node));
        node->kind = ND_LVAR;

        LVar *lvar = find_lvar(tok);
        if (lvar) {
            node->offset = lvar->offset;
        } else {
            lvar = calloc(1, sizeof(LVar));
            lvar->next = locals;
            lvar->name = tok->str;
            lvar->len = tok->len;
            lvar->offset = locals->offset + 8;
            node->offset = lvar->offset;
            locals = lvar;
        }
        return node;
    }

    // そうでなければ数値のはず
    return new_num(expect_number());
}
