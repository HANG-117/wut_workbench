#ifndef RATE_SERVICE_H
#define RATE_SERVICE_H
#include "model.h"
#include "global.h"

// 设置费率
int setRate(float fUnit, float fCharge);

// 获取当前费率
int getCurrentRate(Rate* rate);

// 初始化费率（如果文件不存在则创建默认费率）
int initRate();

#endif
