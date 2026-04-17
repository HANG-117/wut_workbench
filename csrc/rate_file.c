#include "rate_file.h"
#include <stdio.h>
#include <stdlib.h>

// 保存费率到文件
int saveRate(const Rate* rate, const char* path)
{
    FILE* fp = fopen(path, "wb");
    if (fp == NULL)
    {
        return 0;
    }

    size_t written = fwrite(rate, sizeof(Rate), 1, fp);
    fclose(fp);

    return (written == 1) ? 1 : 0;
}

// 从文件读取费率
int loadRate(Rate* rate, const char* path)
{
    FILE* fp = fopen(path, "rb");
    if (fp == NULL)
    {
        return 0;
    }

    size_t read = fread(rate, sizeof(Rate), 1, fp);
    fclose(fp);

    return (read == 1) ? 1 : 0;
}
