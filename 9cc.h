#define _POSIX_C_SOURCE 200809L
#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node Node;

//
// tokenizer.c
//
// トークンの種類
typedef enum {
    TK_IDENT,  // 識別子
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

// ローカル変数の型
typedef struct  Obj Obj;
struct Obj {
    Obj *next;
    char *name;  // 変数の名前
    int offset;  // RBPからのオフセット
};

// Function
typedef struct Function Function;
struct Function {
    Node *body;
    Obj *locals;
    int stack_size;
};

// 抽象構文木のノードの種類
typedef enum {
    ND_ADD,        // +
    ND_SUB,        // -
    ND_MUL,        // *
    ND_DIV,        // /
    ND_NEG,        // unary -
    ND_EQ,         // ==
    ND_NE,         // !=
    ND_LT,         // <
    ND_LE,         // <=
    ND_ASSIGN,     // =
    ND_EXPR_STMT,  // Expression statement
    ND_VAR,        // 変数
    ND_NUM,        // 整数
} NodeKind;

// 抽象構文木のノードの型
struct Node {
    NodeKind kind;  // ノードの型
    Node *next;     // next node
    Node *lhs;      // 左辺
    Node *rhs;      // 右辺
    Obj *var;       // kind == ND_VARのとき使用
    int val;        // kind == ND_NUMのとき使用
};

Function *parse(Token *tok);

//
// codegen.c
//

void codegen(Function *prog);
