#include <thread>
#include <atomic>
#include <QApplication>
#include <iostream>
#include "MainWindow.h"
#include "network.h"
#include "looper.h"
#include "Ini.h"

using namespace std;

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();


    looper looper([&w]() {
        NetworkSpeed s = getTotalNetworkSpeed();


        QString p;
        long double kb = 1024.0;

        const string str[] = {"B/s", "KB/s", "MB/s", "GB/s"};
        int offset = 0;
        long double speed = s.uploadSpeed;
        while (speed / kb > 1) {
            speed /= kb;
            offset++;
        }

        char a[50] = "";
        sprintf(a, "%.2Lf", speed);
        p.append("Upload: ").append(a).append(str[offset]);
        offset = 0;
        speed = s.downloadSpeed;
        while (speed / kb > 1) {
            speed /= kb;
            offset++;
        }
        sprintf(a, "%.2Lf", speed);
        p.append("\nDownload: ").append(a).append(str[offset]);

        int pre = getPowerPercent();
        p.append("\nPower: ").append(std::to_string(pre)).append("%");

        long double pre1 = getMemoryUsage();
        sprintf(a, "%.2Lf", pre1);
        p.append("\nUsage: ").append(a).append("%");

        w.updateLabel("cool", p);
    });

    int result = QApplication::exec();

    looper.waitToStop();

    return result;
}
