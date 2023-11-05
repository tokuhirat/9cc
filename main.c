# include "9cc.h"

// 現在着目しているトークン
Token *token;

// 入力プログラム
char *user_input;

// stmt保管用
Node *code[100];

// ローカル変数
LVar *locals;

// ラベル通し番号
int lend_num = 0;

int main(int argc, char **argv) {
    if (argc != 2) {
        error("引数の個数が正しくありません");
        return 1;
    }

    // トークナイズしてパースする
    // 結果はcodeに保存される。
    user_input = argv[1];
    token = tokenize(user_input);
    program();

    // アセンブリの前半部分を出力
    printf(".intel_syntax noprefix\n");
    printf(".globl main\n");
    printf("main:\n");

    // プロローグ
    // 変数の個数分の領域を確保する
    int i = 0;
    for (LVar *var = locals; var; var = var->next)
        i++;
    printf("  push rbp\n");
    printf("  mov rbp, rsp\n");
    printf("  sub rsp, %d\n", i * 8);

    // 先頭の式から順にコード生成
    for (int i = 0; code[i]; i++){
        gen(code[i]);

        // 式の評価結果としてスタックに１つの値が残っている
        // はずなので、スタックが溢れないようにpopしておく
        printf("  pop rax\n");
    }

    // エピローグ
    // 最後の式の結果がRAXに残っているのでそれが返り値になる
    printf("  mov rsp, rbp\n");
    printf("  pop rbp\n");
    printf("  ret\n");
    return 0;
}
