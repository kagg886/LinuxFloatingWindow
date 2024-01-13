//
// Created by kagg886 on 2024/1/13.
//

#include "looper.h"
#include <thread>
#include <functional>

void looper::waitToStop() {
    running = false;
    if (thread->joinable()) {
        thread->join();
    }
}

looper::looper(std::function<void()> func) {
    running = true;

    thread = new std::thread([this,&func]() {
        while (running) {
            func();
        }
    });
}
