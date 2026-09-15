/*
 * 2026-09-03 printf/scanf 系统补漏 demo
 * 编译: gcc -Wall -o printf_demo printf_scanf_demo.c
 * 运行: ./printf_demo.exe
 * 注意: 输出字符串全用英文（终端 GBK 与文件 UTF-8 的坑）
 */
#include <stdio.h>

int main(void)
{
    /* ===== 第 1 部分: printf 格式符全家桶 ===== */
    int      a = 255;          /* int:   有符号, 32 位, 范围 ±21 亿 */
    unsigned int b = 255;      /* uint:  无符号, 只存正数 */
    char     c = 'A';          /* char:  1 字节, 存 ASCII 码 65 */
    float    f = 25.1234;      /* float: 浮点, 有小数 */
    char     s[] = "hello";    /* 字符串 = 字符数组, 末尾自带 '\0' */

    printf("%d\n", a);         /* 255       十进制(有符号) */
    printf("%u\n", b);         /* 255       十进制(无符号) */
    printf("%X\n", b);         /* FF        十六进制大写 */
    printf("%02X\n", b);       /* FF        两位补零大写 */
    printf("%04X\n", a);       /* 00FF      四位补零大写(看 uint16 整字) */
    printf("%f\n", f);         /* 25.123400 浮点, 默认 6 位小数 */
    printf("%.1f\n", f);       /* 25.1      保留 1 位小数 */
    printf("%c\n", c);         /* A         单个字符 */
    printf("%s\n", s);         /* hello     字符串(以 '\0' 结尾) */

    /* 取字节(CRC 验证常客): crc & 0xFF = 低字节, crc >> 8 = 高字节 */
    unsigned short crc = 0x0B7E;
    printf("crc low:  %02X\n", crc & 0xFF);   /* 7E */
    printf("crc high: %02X\n", crc >> 8);     /* 0B */

    /* ===== 第 2 部分: scanf 为什么必须 & ===== */
    int x = 0;
    printf("input a number: ");
    scanf("%d", &x);          /* &x = 变量 x 的地址; scanf 把键盘值写进那个地址 */
    printf("you typed: %d\n", x);

    return 0;
}
