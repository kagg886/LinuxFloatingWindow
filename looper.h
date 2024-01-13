//
// Created by kagg886 on 2024/1/13.
//

#ifndef UNTITLED_LOOPER_H
#define UNTITLED_LOOPER_H


#include <thread>
#include <functional>
#include <atomic>

class looper {
private:
    std::atomic<bool> running;
    std::thread *thread;
public:
    looper(std::function<void()> func);
    void waitToStop();
};


#endif //UNTITLED_LOOPER_H
