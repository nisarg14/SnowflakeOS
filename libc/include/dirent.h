#pragma once

#include <stdio.h>
#include <stdint.h>

typedef uint32_t ino_t;

typedef struct {
    int32_t fd;
    char name[MAX_PATH];
    FILE* stream;
} DIR;

struct dirent {
    ino_t d_ino;
    char d_name[MAX_PATH];
    uint32_t d_type;
};

#ifndef _KERNEL_
DIR* opendir(const char* path);
struct dirent* readdir(DIR* dir);
int closedir(DIR* dir);
#endif