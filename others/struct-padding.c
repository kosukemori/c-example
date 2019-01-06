// 構造体のパディングのテスト

// 今回わざとパディングが入った構造体でテストしているが、パディングが入らないように構造体を作るのがベター(?)
// (ただしパディングの入り方は処理系依存)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Foo {
    int8_t member1;
    int16_t member2;
};

int main() {
    uint8_t buffer[2000]; // 送受信やファイル書き込みで使うことをイメージした配列
    struct Foo foo;

    ////////////////////////////////////
    /// 構造体のパディングを実感する ///
    ////////////////////////////////////
    printf("%ld\n", sizeof(foo.member1)); // 1バイト
    printf("%ld\n", sizeof(foo.member2)); // 2バイト
    printf("%ld\n", sizeof(foo)); // 1+2=3バイト、ではなくパディングが間に1バイト入って合計4バイト
    printf("\n");


    ///////////////////////////////////////////////////////////////////////////////
    /// パディングが入ってしまった構造体の内容をuint8_t型の配列へコピーしてみる ///
    ///////////////////////////////////////////////////////////////////////////////
    foo.member1 = 'g';
    foo.member2 = 11;
    struct Foo foo_tmp;

    // 配列にパディングが入る例1: 構造体をそのまま配列にmemcpy()する
    memset(buffer, '0', sizeof(buffer));
    memcpy(buffer, &foo, sizeof(foo));
    memcpy(&foo_tmp.member1, buffer, sizeof(foo_tmp.member1));
    memcpy(&foo_tmp.member2, buffer + sizeof(foo_tmp.member1), sizeof(foo_tmp.member2));
    printf("%c\n", foo_tmp.member1); // ここは'g'と出るが、
    printf("%d\n", foo_tmp.member2); // ここはおかしな値になる
    // 試しに配列からmember2を読み込む際、1バイトのパディングの分だけずらしてみると……
    memcpy(&foo_tmp.member1, buffer, sizeof(foo_tmp.member1));
    memcpy(&foo_tmp.member2, buffer + sizeof(foo_tmp.member1) + 1, sizeof(foo_tmp.member2));
    printf("%c\n", foo_tmp.member1); // ここは'g'と出て、
    printf("%d\n", foo_tmp.member2); // こっちもちゃんと11と出る
    printf("\n");

    // 配列にパディングが入る例2: 配列の先頭ポインタを構造体のポインタ型にキャストして、そのメンバに代入する
    // (結果は構造体をそのまま配列にmemcpy()したときと同じ)
    memset(buffer, '0', sizeof(buffer));
    struct Foo *foo_pt = (struct Foo *)buffer;
    foo_pt->member1 = foo.member1;
    foo_pt->member2 = foo.member2;
    memcpy(&foo_tmp.member1, buffer, sizeof(foo_tmp.member1));
    memcpy(&foo_tmp.member2, buffer + sizeof(foo_tmp.member1), sizeof(foo_tmp.member2));
    printf("%c\n", foo_tmp.member1); // ここは'g'と出るが、
    printf("%d\n", foo_tmp.member2); // ここはおかしな値になる
    memcpy(&foo_tmp.member1, buffer, sizeof(foo_tmp.member1));
    memcpy(&foo_tmp.member2, buffer + sizeof(foo_tmp.member1) + 1, sizeof(foo_tmp.member2));
    printf("%c\n", foo_tmp.member1); // ここは'g'と出て、
    printf("%d\n", foo_tmp.member2); // こっちもちゃんと11と出る

    // 配列にパディングが入らない例: 構造体のメンバをひとつひとつ配列にmemcpy()する
    memset(buffer, '0', sizeof(buffer));
    memcpy(buffer, &foo.member1, sizeof(foo.member1));
    memcpy(buffer + sizeof(foo.member1), &foo.member2, sizeof(foo.member2));
    memcpy(&foo_tmp.member1, buffer, sizeof(foo_tmp.member1));
    memcpy(&foo_tmp.member2, buffer + sizeof(foo_tmp.member1), sizeof(foo_tmp.member2));
    printf("%c\n", foo_tmp.member1); // ちゃんと'g'と出る
    printf("%d\n", foo_tmp.member2); // 同じく、ちゃんと11と出る
    printf("\n");

    return 0;
}
