#include <fstream>
#include <string>
#include <unistd.h> // for sleep
#include <dirent.h> // for directory traversal
#include <vector>
#include <filesystem>
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


int getPowerPercent() {
    const std::string power = "/sys/class/power_supply/";
    DIR *dir = opendir(power.c_str());
    if (!dir) {
        return 0; // 无法打开目录时返回0
    }

    struct dirent *entry;
    std::filesystem::path filePath;
    while ((entry = readdir(dir)) != nullptr) {
        if (entry->d_name[0] != '.') { // 忽略隐藏文件和上级目录链接
            filePath = power + std::string(entry->d_name) + "/capacity";
            if (std::filesystem::exists(filePath)) {
                break;
            }
        }
    }

    closedir(dir);
    return static_cast<int>(readFileValue(filePath));

}

long double getMemoryUsage() {
    std::ifstream info("/proc/meminfo");
    if (!info) {
        return -1;
    }

    std::string line;
    long double total_memory = 0;
    long double free_memory = 0;

    while (std::getline(info, line)) {
        std::istringstream iss(line);
        std::string key;
        long long value;
        std::string unit;
        iss >> key >> value >> unit;

        if (key == "MemTotal:") {
            total_memory = value;
        } else if (key == "MemFree:" || key == "Buffers:" || key == "Cached:") {
            free_memory += value;
        }
    }

    long double used_memory = total_memory - free_memory;
    long double memory_usage = (used_memory / total_memory) * 100.0;

    return memory_usage;
}

