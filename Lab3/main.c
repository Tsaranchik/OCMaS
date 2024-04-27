#include <stdio.h>

int main() {
    int num1, num2, base;
    scanf("%d %d %d", &num1, &num2, &base);

    int quotient = num1 / num2;
    int remainder = num1 % num2;

    char quotient_str[50], remainder_str[50];

    char *quotient_ptr = quotient_str, *remainder_ptr = remainder_str;

    do {
        int q_remainder = quotient % base;
        if (q_remainder < 10)
            *quotient_ptr++ = q_remainder + '0';
        else
            *quotient_ptr++ = q_remainder - 10 + 'A';
        quotient /= base;
    }while (quotient != 0);
    *quotient_ptr = '\0';

    do {
        int r_remainder = remainder % base;
        if (r_remainder < 10)
            *remainder_ptr++ = r_remainder + '0';
        else
            *remainder_ptr++ = r_remainder - 10 + 'A';
        remainder /= base;
    }while (remainder != 0);
    *remainder_ptr = '\0';

    for (int i = 0, j = quotient_ptr - quotient_str - 1; i < j; i++, j--) {
        char temp = quotient_str[i];
        quotient_str[i] = quotient_str[j];
        quotient_str[j] = temp;
    }

    for (int i = 0, j = remainder_ptr - remainder_str - 1; i < j; i++, j--) {
        char temp = remainder_str[i];
        remainder_str[i] = remainder_str[j];
        remainder_str[j] = temp;
    }

    printf("%s\n%s\n", quotient_str, remainder_str);
}
