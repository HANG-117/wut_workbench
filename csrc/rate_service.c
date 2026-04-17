#include "rate_service.h"
#include "rate_file.h"
#include <stdio.h>
#include <time.h>

// 设置费率
int setRate(float fUnit, float fCharge)
{
    if (fUnit <= 0 || fCharge <= 0)
    {
        printf("费率设置失败：计费单位和收费金额必须大于0！\n");
        return 0;
    }

    Rate rate;
    rate.fUnit = fUnit;
    rate.fCharge = fCharge;
    rate.tLastUpdate = time(NULL);

    if (saveRate(&rate, RATE_PATH) == 1)
    {
        printf("费率设置成功！\n");
        printf("计费单位：%.0f 分钟\n", fUnit);
        printf("每单位收费：%.2f 元\n", fCharge);
        printf("更新时间：%s", ctime(&rate.tLastUpdate));
        return 1;
    }
    else
    {
        printf("费率设置失败：保存文件错误！\n");
        return 0;
    }
}

// 获取当前费率
int getCurrentRate(Rate* rate)
{
    if (loadRate(rate, RATE_PATH) == 1)
    {
        return 1;
    }
    else
    {
        printf("获取费率失败：读取文件错误！\n");
        return 0;
    }
}

// 初始化费率（如果文件不存在则创建默认费率）
int initRate()
{
    Rate rate;
    if (loadRate(&rate, RATE_PATH) == 1)
    {
        // 文件存在，更新全局费率常量
        return 1;
    }
    else
    {
        // 文件不存在，创建默认费率
        rate.fUnit = UNIT;
        rate.fCharge = CHARGE;
        rate.tLastUpdate = time(NULL);

        if (saveRate(&rate, RATE_PATH) == 1)
        {
            return 1;
        }
        else
        {
            printf("初始化费率失败！\n");
            return 0;
        }
    }
}
