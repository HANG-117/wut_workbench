#ifndef RATE_FILE_H
#define RATE_FILE_H
#include "model.h"

#define RATE_PATH "data/rate.ams"

// 保存费率到文件
int saveRate(const Rate* rate, const char* path);

// 从文件读取费率
int loadRate(Rate* rate, const char* path);

#endif
