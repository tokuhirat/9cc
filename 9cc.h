#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
// tokenizer.c
//
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

void error(char *fmt, ...);
void error_at(char *loc, char *fmt, ...);
void error_tok(Token *tok, char *fmt, ...);
bool equal(Token *tok, char *op);
Token *skip(Token *tok, char *op);
Token *tokenize(char *input);

//
// parse.c
//
// 抽象構文木のノードの種類
typedef enum {
    ND_ADD,  // +
    ND_SUB,  // -
    ND_MUL,  // *
    ND_DIV,  // /
    ND_NEG,  // unary -
    ND_NUM,  // 整数
    ND_EQ,   // ==
    ND_NE,   // !=
    ND_LT,   // <
    ND_LE,   // <=
} NodeKind;

// 抽象構文木のノードの型
typedef struct Node Node;
struct Node {
    NodeKind kind;  // ノードの型
    Node *lhs;  // 左辺
    Node *rhs;  // 右辺
    int val;  // kindがND_NUMの場合のみ使う
};

Node *parse(Token *tok);

//
// codegen.c
//

void codegen(Node *node);
