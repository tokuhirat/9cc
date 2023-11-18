#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
// tokenize.c
//
// トークンの種類
typedef enum {
    TK_PUNCT,  // 記号など
    TK_KEYWORD,  // キーワード
    TK_IDENT,  // 識別子
    TK_NUM,  // 整数トークン
    TK_EOF,  // 入力の終わりを表すトークン
} TokenKind;

// トークン型
typedef struct Token Token;
struct Token {
    TokenKind kind;  // トークンの型
    Token *next;  // 次の入力トークン
    int val;  // kindがTK_NUMの場合、その数値
    char *loc;  // トークンの位置
    int len;  // トークンの長さ
};

// ローカル変数の型
typedef struct  LVar LVar;
struct LVar {
    LVar *next;  // 次の変数かNULL
    char *name;  // 変数の名前
    int len;  // 名前の長さ
    int offset;  // RBPからのオフセット
};

void error(char *fmt, ...);
void error_at(char *loc, char *fmt, ...);
void error_tok(Token *tok, char *fmt, ...);
bool equal(Token *tok, char *op);
Token *skip(Token *tok, char *op);
Token *tokenize(char *p);

//
// parse.c
//
// 抽象構文木のノードの種類
typedef enum {
    ND_ADD,  // +
    ND_SUB,  // -
    ND_MUL,  // *
    ND_DIV,  // +
    ND_NUM,  // 整数
    ND_EQ,  // ==
    ND_NE,  // !=
    ND_LT,  // <
    ND_LE,  // <=
    ND_ASSIGN,  // =
    ND_LVAR,  // ローカル変数
    ND_RETURN,  // return
    ND_IF,  // if
    ND_IFELSE,  // if else
    ND_WHILE,  // while
    ND_FOR,  // for
    ND_BLOCK,  // 複文
} NodeKind;

// 抽象構文木のノードの型
typedef struct Node Node;
struct Node {
    NodeKind kind;  // ノードの型
    Node *lhs;  // 左辺
    Node *rhs;  // 右辺
    int val;  // kindがND_NUMの場合のみ使う
    int offset;  // kindがND_LVARの場合のみ使う
    Node *cond;  // kindがND_IF, ND_IFELSE, ND_WHILE, ND_FORの場合のみ使う
    Node *for_init;  // kindがND_FORの場合のみ使う
    Node *next;  // 次のノードが必要な場合に使う
    Node *body;  // kindがND_BLOCKの場合にのみ使う
};

void program();

//
// codegen.c
//
void gen(Node *node);

//
// main.c
//
// 現在着目しているトークン
extern Token *token;

// 入力プログラム
extern char *user_input;

// stmt保管用
extern Node *code[100];

// ローカル変数
extern LVar *locals;

// ラベル通し番号
extern int label_num;
