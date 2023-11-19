#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// トークンの種類
typedef enum {
    TK_PUNCT,  // 記号
    TK_NUM,    // 整数トークン
    TK_EOF,    // 入力の終わりを表すトークン
} TokenKind;

// トークン型
typedef struct Token Token;
struct Token {
    TokenKind kind;  // トークンの型
    Token *next;     // 次の入力トークン
    int val;         // kindがTK_NUMの場合、その数値
    char *loc;       // トークン位置
    int len;         // トークン長さ
};

char *current_input;

// エラーを報告するための関数
// printfと同じ引数を取る
void error(char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

// エラー箇所を報告する
void verror_at(char *loc, char *fmt, va_list ap) {
    int pos = loc - current_input;
    fprintf(stderr, "%s\n", current_input);
    fprintf(stderr, "%*s", pos, " ");
    fprintf(stderr, "^ ");
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

void error_at(char *loc, char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    verror_at(loc, fmt, ap);
}


void error_tok(Token *tok, char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    verror_at(tok->loc, fmt, ap);
}

// トークンが期待している記号か確認
bool equal(Token *tok, char *op) {
    return memcmp(tok->loc, op, tok->len) == 0 && op[tok->len] == '\0';
}

// トークンが期待している記号の時にはトークンを１つ読み進める。
// それ以外の場合にはエラーを報告する。
Token *skip(Token *tok, char *s) {
    if (!equal(tok, s))
        error_tok(tok, "'%s'ではありません", s);
    return tok->next;
}

// トークンがTK_NUMのときその数値を返す。
// それ以外の場合にはエラーを報告する。
int get_number(Token *tok) {
    if (tok->kind != TK_NUM)
        error_tok(tok, "expected a number");
    return tok->val;
}

// 新しいトークンを作成する
Token *new_token(TokenKind kind, char *start, char *end) {
    Token *tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->loc = start;
    tok->len = end - start;
    return tok;
}

// 入力文字列pをトークナイズしてそれを返す
Token *tokenize(char *p) {
    Token head = {};
    Token *cur = &head;

    while (*p) {
        // 空白文字をスキップ
        if (isspace(*p)) {
            p++;
            continue;
        }

        // 数値の場合
        if (isdigit(*p)) {
            cur = cur->next = new_token(TK_NUM, p, p);
            char *q = p;
            cur->val = strtoul(p, &p, 10);
            cur->len = p - q;
            continue;
        }

        // Punctuator
        if (strchr("+-*/()", *p)) {
        // if (ispunct(*p)) {
            cur = cur->next = new_token(TK_PUNCT, p, p + 1);
            p++;
            continue;
        }

        error_at(p, "トークナイズできません");
    }

    cur = cur->next = new_token(TK_EOF, p, p);
    return head.next;
}

// 抽象構文木のノードの種類
typedef enum {
    ND_ADD,  // +
    ND_SUB,  // -
    ND_MUL,  // *
    ND_DIV,  // +
    ND_NUM,  // 整数
} NodeKind;

typedef struct Node Node;

// 抽象構文木のノードの型
struct Node {
    NodeKind kind;  // ノードの型
    Node *lhs;  // 左辺
    Node *rhs;  // 右辺
    int val;  // kindがND_NUMの場合のみ使う
};

Node *new_node(NodeKind kind, Node *lhs, Node *rhs) {
    Node *node = calloc(1, sizeof(Node));
    node->kind = kind;
    node->lhs = lhs;
    node->rhs = rhs;
    return node;
}

Node *new_node_num(int val) {
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_NUM;
    node->val = val;
    return node;
}

Node *expr(Token **rest, Token *tok);
Node *mul(Token **rest, Token *tok);
Node *primary(Token **rest, Token *tok);

Node *expr(Token **rest, Token *tok) {
    Node *node = mul(&tok, tok);

    for (;;) {
        if (equal(tok, "+")) {
            node = new_node(ND_ADD, node, mul(&tok, tok->next));
            continue;
        }
            
        if (equal(tok, "-")) {
            node = new_node(ND_SUB, node, mul(&tok, tok->next));
            continue;
        }
        *rest = tok;
        return node;
    }
}

Node *mul(Token **rest, Token *tok) {
    Node *node = primary(&tok, tok);

    for (;;) {
        if (equal(tok, "*")) {
            node = new_node(ND_MUL, node, primary(&tok, tok->next));
            continue;
        }
            
        if (equal(tok, "/")) {
            node = new_node(ND_DIV, node, primary(&tok, tok->next));
            continue;
        }
        *rest = tok;            
        return node;
    }
}

Node *primary(Token **rest, Token *tok) {
    // 次のトークンが"("なら、"(" expr ")"のはず
    if (equal(tok, "(")) {
        Node *node = expr(&tok, tok->next);
        *rest = skip(tok, ")");
        return node;
    }

    // そうでなければ数値のはず
    int val = get_number(tok);
    *rest = tok->next;
    return new_node_num(val);
}

void gen(Node *node) {
    if (node->kind == ND_NUM) {
        printf("  push %d\n", node->val);
        return;
    }

    gen(node->lhs);
    gen(node->rhs);

    printf("  pop rdi\n");
    printf("  pop rax\n");

    switch (node->kind) {
    case ND_ADD:
        printf("  add rax, rdi\n");
        break;
    case ND_SUB:
        printf("  sub rax, rdi\n");
        break;
    case ND_MUL:
        printf("  imul rax, rdi\n");
        break;
    case ND_DIV:
        printf("  cqo\n");
        printf("  idiv rdi\n");
        break;
    }

    printf("  push rax\n");
}


int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "引数の個数が正しくありません\n");
        return 1;
    }

    // トークナイズする
    current_input = argv[1];
    Token *tok = tokenize(current_input);
    Node *node = expr(&tok, tok);

    // アセンブリの前半部分を出力
    printf(".intel_syntax noprefix\n");
    printf(".globl main\n");
    printf("main:\n");

    // 抽象構文木を下りながらコード生成
    gen(node);

    // スタックトップに式全体の値が残っているはずなので
    // それをRAXにロードして関数からの返り値とする
    printf("  pop rax\n");
    printf("  ret\n");
    return 0;
}