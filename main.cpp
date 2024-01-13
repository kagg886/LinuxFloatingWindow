#include <thread>
#include <atomic>
#include <QApplication>
#include <iostream>
#include "MainWindow.h"
#include "network.h"
#include "looper.h"

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

        char a[50]= "";
        sprintf(a,"%.2Lf",speed);
        p.append("Upload: ").append(string(a)).append(str[offset]);

        offset = 0;
        speed = s.downloadSpeed;
        while (speed / kb > 1) {
            speed /= kb;
            offset++;
        }

        sprintf(a,"%.2Lf",speed);
        p.append("\nDownload: ").append(string(a)).append(str[offset]);
        w.updateLabel("network",p);
    });

    int result = QApplication::exec();

    looper.waitToStop();

    return result;
}
