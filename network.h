//
// Created by kagg886 on 2024/1/13.
//

#ifndef UNTITLED_NETWORK_H
#define UNTITLED_NETWORK_H

#endif //UNTITLED_NETWORK_H

#include <string>

struct NetworkSpeed {
    long long downloadSpeed; // 单位：字节/秒
    long long uploadSpeed;   // 单位：字节/秒
};


NetworkSpeed getTotalNetworkSpeed();
int getPowerPercent();
long double getMemoryUsage();