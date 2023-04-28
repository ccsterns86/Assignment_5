#include <stdio.h>

int main(int argc, char* argv) {
    FILE *fileP;
    char buf[512];
    int bufSize = sizeof(buf);
    size_t bytesRead = 0;

    fileP = fopen("fs.img", "rb");
    if (fileP == NULL) {
        printf("Failure opening fs.img.\n");
        return -1;
    }
    // Read the Boot Block
    size_t result = fread(buf, sizeof(char), sizeof(buf), fileP);
    printf("B");

    // Read the Superblock
    fread(buf, bufSize, 1, fileP);
    printf("S");

    int counter = 2;
    while ((bytesRead = fread(buf, sizeof(char), sizeof(buf), fileP)) > 0) {
        counter++;
    }
    printf("\nNumber of 512 blocks: %d\n", counter);
    // int sizeSWBlock = 10; //1024;
    // for (int i = 2; i < sizeSWBlock; i++) {
    //     fread(buf, 1, sizeof(buf), fileP);
    //     printf("Block %d: starts with %d.\n", i, buf[0]);
    // }
    printf("\n");
    fclose(fileP);

    return 0;
}