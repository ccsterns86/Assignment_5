#include <stdarg.h>

#include "types.h"
#include "param.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "fs.h"
#include "file.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"
#include "proc.h"
#include "ioctl.h"

// For writing to null
int nullwrite(int user_src, uint64 src, int n) {
    return 0; //done
}

// For reading to null
int nullread(int user_dst, uint64 dst, int n) {
    return -1; //fail immediately
}

// For ioctl for null
int nullioctl(int user_dst, uint64 dst, int request) {
    return -1; //fail immediately
}

void nullinit(void) {
  devsw[NULL].read = nullread;
  devsw[NULL].write = nullwrite;
  devsw[NULL].ioctl = nullioctl;
}