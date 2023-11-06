#include "9cc.h"

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
void error_at(char *loc, char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);

    int pos = loc - user_input;
    fprintf(stderr, "%s\n", user_input);
    fprintf(stderr, "%*s", pos, " ");
    fprintf(stderr, "^ ");
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

// 次のトークンが期待している記号のときには、トークンを１つ読み進めて
// 真を返す。それ以外の場合には偽を返す。
bool consume(char *op) {
    if (token->kind != TK_RESERVED ||
        strlen(op) != token->len ||
        memcmp(token->str, op, token->len))
        return false;
    token = token->next;
    return true;
}

// 次のトークンが期待している記号のときには、真を返す。それ以外の場合には偽を返す。
// トークンは消費しない。
bool check_token(char *op) {
    if (token->kind != TK_RESERVED ||
        strlen(op) != token->len ||
        memcmp(token->str, op, token->len))
        return false;
    return true;
}

// 次のトークンが識別子のときには、識別子のトークンを返し、
// トークンを１つ読み進める。それ以外の場合にはfalseを返す。
Token *consume_ident() {
    if (token->kind != TK_IDENT){
        return false;
    }
    Token *tok = token;
    token = token->next;
    return tok;
}

// 次のトークンがreturnのときには、トークンを１つ読み進めて
// 真を返す。それ以外の場合には偽を返す。
bool consume_return() {
    if (token->kind != TK_RETURN){
        return false;
    }
    token = token->next;
    return true;
}

// 次のトークンがifのときには、トークンを１つ読み進めて
// 真を返す。それ以外の場合には偽を返す。
bool consume_if() {
    if (token->kind != TK_IF){
        return false;
    }
    token = token->next;
    return true;
}

// 次のトークンがelseのときには、トークンを１つ読み進めて
// 真を返す。それ以外の場合には偽を返す。
bool consume_else() {
    if (token->kind != TK_ELSE){
        return false;
    }
    token = token->next;
    return true;
}

// 次のトークンがwhileのときには、トークンを１つ読み進めて
// 真を返す。それ以外の場合には偽を返す。
bool consume_while() {
    if (token->kind != TK_WHILE){
        return false;
    }
    token = token->next;
    return true;
}

// 次のトークンがforのときには、トークンを１つ読み進めて
// 真を返す。それ以外の場合には偽を返す。
bool consume_for() {
    if (token->kind != TK_FOR){
        return false;
    }
    token = token->next;
    return true;
}

// 次のトークンが期待している記号の時にはトークンを１つ読み進める。
// それ以外の場合にはエラーを報告する。
void expect(char *op) {
    if (token->kind != TK_RESERVED ||
        strlen(op) != token->len ||
        memcmp(token->str, op, token->len))
        error_at(token->str, "'%s'ではありません", op);
    token = token->next;
}

// トークンが数値の場合、トークンを１つ読み進めてその数値を返す。
// それ以外の場合にはエラーを報告する。
int expect_number() {
    if (token->kind != TK_NUM)
        error_at(token->str, "数ではありません");
    int val = token->val;
    token = token->next;
    return val;
}

// 新しいトークンを作成してcurに繋げる
Token *new_token(TokenKind kind, Token *cur, char *str, int len) {
    Token *tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->str = str;
    tok->len = len;
    cur->next = tok;
    return tok;
}

bool startswith(char *p, char *q) {
    return memcmp(p, q, strlen(q)) == 0;
}

bool is_ident1(char c) {
    return (('a' <= c && c <= 'z') ||
        ('A' <= c && c <= 'Z')
        || c == '_');
}

bool is_ident2(char c) {
    return is_ident1(c) || ('0' <= c && c <= '9');
}

int is_alnum(char c) {
    return ('a' <= c && c <= 'z') ||
        ('A' <= c && c <= 'Z') ||
        ('0' <= c && c <= '9') ||
        (c == '_');
}

// 入力文字列pをトークナイズしてそれを返す
Token *tokenize(char *p) {
    Token head;
    head.next = NULL;
    Token *cur = &head;

    while (*p) {
        // 空白文字をスキップ
        if (isspace(*p)) {
            p++;
            continue;
        }

        if (strncmp(p, "return", 6) == 0 && !is_alnum(p[6])) {
            cur = new_token(TK_RETURN, cur, p, 6);
            p += 6;
            continue;
        }

        if (strncmp(p, "if", 2) == 0 && !is_alnum(p[2])) {
            cur = new_token(TK_IF, cur, p, 2);
            p += 2;
            continue;
        }

        if (strncmp(p, "else", 4) == 0 && !is_alnum(p[4])) {
            cur = new_token(TK_ELSE, cur, p, 4);
            p += 4;
            continue;
        }

        if (strncmp(p, "while", 5) == 0 && !is_alnum(p[5])) {
            cur = new_token(TK_WHILE, cur, p, 5);
            p += 5;
            continue;
        }

        if (strncmp(p, "for", 3) == 0 && !is_alnum(p[3])) {
            cur = new_token(TK_FOR, cur, p, 3);
            p += 3;
            continue;
        }

        if (startswith(p, "==") || startswith(p, "!=") ||
            startswith(p, "<=") || startswith(p, ">=")) {
            cur = new_token(TK_RESERVED, cur, p, 2);
            p += 2;
            continue;
        }

        if (strchr("+-*/(){}<>=;", *p)) {
            cur = new_token(TK_RESERVED, cur, p++, 1);
            continue;
        }

        if (isdigit(*p)) {
            cur = new_token(TK_NUM, cur, p, 0);
            char *q = p;
            cur->val = strtol(p, &p, 10);
            cur->len = p - q;
            continue;
        }

        if (is_ident1(*p)){
            char *q = p;
            do {
                p++;
            } while (is_ident2(*p));
            cur = new_token(TK_IDENT, cur, q, p - q);
            continue;
        }

        error_at(p, "トークナイズできません");
    }

    new_token(TK_EOF, cur, p, 0);
    return head.next;
}
