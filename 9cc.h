#define _POSIX_C_SOURCE 200809L
#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Type Type;
typedef struct Node Node;

//
// tokenizer.c
//
// トークンの種類
typedef enum {
    TK_IDENT,    // 識別子
    TK_PUNCT,    // 記号
    TK_KEYWORD,  // キーワード
    TK_NUM,      // 整数トークン
    TK_EOF,      // 入力の終わりを表すトークン
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
bool consume(Token **rest, Token *tok, char *str);
Token *tokenize(char *input);

//
// parse.c
//

// ローカル変数の型
typedef struct  Obj Obj;
struct Obj {
    Obj *next;
    char *name;  // 変数の名前
    Type *ty;
    int offset;  // RBPからのオフセット
};

// Function
typedef struct Function Function;
struct Function {
    Function *next;
    char *name;
    Obj *params;

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
    ND_ADDR,       // unary &
    ND_DEREF,      // unary *
    ND_RETURN,     // return
    ND_IF,         // if
    ND_FOR,        // for or while
    ND_BLOCK,      // { ... }
    ND_FUNCALL,    // 関数呼び出し
    ND_EXPR_STMT,  // Expression statement
    ND_VAR,        // 変数
    ND_NUM,        // 整数
} NodeKind;

// 抽象構文木のノードの型
struct Node {
    NodeKind kind;  // ノードの型
    Node *next;     // next node
    Type *ty;       // Type
    Token *tok;     // 代表トークン

    Node *lhs;      // 左辺
    Node *rhs;      // 右辺

    // "if" or "for" statement
    Node *cond;
    Node *then;
    Node *els;
    Node *init;
    Node *inc;

    // Block
    Node *body;

    // 関数呼び出し
    char *funcname;
    Node *args;

    Obj *var;       // kind == ND_VARのとき使用
    int val;        // kind == ND_NUMのとき使用
};

Function *parse(Token *tok);


//
// type.c
//

typedef enum {
    TY_INT,
    TY_PTR,
    TY_FUNC,
    TY_ARRAY,
} TypeKind;

struct Type {
    TypeKind kind;
    int size;      // sizeof() value
    
    // Pointer-to or array-of
    Type *base;

    // Declaration
    Token *name;

    // Array
    int array_len;

    // Function type
    Type *return_ty;
    Type *params;
    Type *next;
};

extern Type *ty_int;

bool is_integer(Type *ty);
Type *copy_type(Type *ty);
Type *pointer_to(Type *base);
Type *func_type(Type *return_ty);
Type *array_of(Type *base, int len);
void add_type(Node *node);

//
// codegen.c
//

void codegen(Function *prog);
