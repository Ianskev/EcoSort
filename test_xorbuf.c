#include <stdio.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>

#define SYS_XORBUF 548

int main() {
    char buf[] = "Hola kernel";
    unsigned char key = 0x42;
    long ret;

    printf("Original:    %s\n", buf);

    ret = syscall(SYS_XORBUF, buf, strlen(buf), key);
    printf("XOR ret:     %ld\n", ret);
    printf("Encriptado:  ");
    for (int i = 0; i < (int)strlen("Hola kernel"); i++)
        printf("%02x ", (unsigned char)buf[i]);
    printf("\n");

    ret = syscall(SYS_XORBUF, buf, ret, key);
    printf("Restaurado:  %s\n", buf);

    return 0;
}
