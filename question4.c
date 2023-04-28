// Cassandra Sterns       SID: 22209739
// Ethan Doyle            SID: 22210635

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

struct superblock {
  uint magic;        // Must be FSMAGIC
  uint size;         // Size of file system image (blocks)
  uint nblocks;      // Number of data blocks
  uint ninodes;      // Number of inodes.
  uint nlog;         // Number of log blocks
  uint logstart;     // Block number of first log block
  uint inodestart;   // Block number of first inode block
  uint bmapstart;    // Block number of first free map block
};

#define BSIZE 1024  // block size

int main(int argc, char* argv) {
    int fileP;
    char buf[BSIZE];
    size_t bytesRead = 0;

    fileP = open("fs.img", O_RDONLY);
    if (fileP < 0) {
        printf("Failure opening fs.img.\n");
        return -1;
    }
    // Always starts with 1 boot block and 1 super block
    printf("BS");

    // Go to location of Superblock
    if(lseek(fileP, BSIZE, 0) != BSIZE) {
        printf("problem finding the start of the superblock...\n");
        return -1;
    }
    // Read superblock
    struct superblock sb;
    if(read(fileP, buf, BSIZE) != BSIZE) {
        printf("failed to read the entire superblock.\n");
        return -1;
    }
    //move the address stored at block 1 (the superblock) to be assigned to the struct superblock
    memmove(&sb, buf, sizeof(sb));
    int numLogs = sb.nlog;
    int numInode = sb.bmapstart - sb.inodestart;
    int numData = sb.nblocks;
    int numBitmap = sb.size - 2 - numLogs - numInode - numData;
    // printf("Number of log blocks: %d\n", numLogs);
    // printf("Number of i-nodes blocks: %d\n", numInode);
    // printf("Number of bitmap blocks: %d\n", numBitmap);
    // printf("Number of data blocks: %d\n", numData);
    for (int i = 0; i < numLogs; i++)
        printf("L");
    for (int i = 0; i < numInode; i++)
        printf("I");
    for (int i = 0; i < numBitmap; i++)
        printf("T");
    for (int i = 0; i < numData; i++)
        printf("D");

    printf("\n");
    close(fileP);

    return 0;
}