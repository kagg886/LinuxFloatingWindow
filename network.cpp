#include <fstream>
#include <string>
#include <unistd.h> // for sleep
#include <dirent.h> // for directory traversal
#include <vector>
#include "network.h"

// 读取指定文件的第一行并返回其数值
long long readFileValue(const std::string &file) {
    std::ifstream inFile(file);
    long long value;
    if (inFile >> value) {
        return value;
    }
    return 0;
}

// 获取所有网络接口的总速度
NetworkSpeed getTotalNetworkSpeed() {
    const std::string netDir = "/sys/class/net/";
    DIR *dir = opendir(netDir.c_str());
    if (!dir) {
        return NetworkSpeed{0, 0}; // 无法打开目录时返回0
    }

    struct dirent *entry;
    std::vector<std::string> interfaces;
    while ((entry = readdir(dir)) != nullptr) {
        if (entry->d_name[0] != '.') { // 忽略隐藏文件和上级目录链接
            interfaces.push_back(entry->d_name);
        }
    }
    closedir(dir);

    long long totalRxBytes1 = 0, totalTxBytes1 = 0;
    for (const auto &iface : interfaces) {
        totalRxBytes1 += readFileValue(netDir + iface + "/statistics/rx_bytes");
        totalTxBytes1 += readFileValue(netDir + iface + "/statistics/tx_bytes");
    }

    sleep(1); // 等待一秒

    long long totalRxBytes2 = 0, totalTxBytes2 = 0;
    for (const auto &iface : interfaces) {
        totalRxBytes2 += readFileValue(netDir + iface + "/statistics/rx_bytes");
        totalTxBytes2 += readFileValue(netDir + iface + "/statistics/tx_bytes");
    }

    return NetworkSpeed{
            totalRxBytes2 - totalRxBytes1, // 总下载速度
            totalTxBytes2 - totalTxBytes1  // 总上传速度
    };
}
