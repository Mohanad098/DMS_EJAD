#ifndef FAULTMANAGER_H
#define FAULTMANAGER_H
#include <queue>
#include <thread>
#include <iostream>
#include "threadsafequeue.h"

class FaultManager
{
public:
    FaultManager(ThreadSafeQueue<std::string>& commandsQueue,ThreadSafeQueue<std::string>& faultsQueue);
    ~FaultManager();
    void faultstart();
    void faultfind();
    void faulthandling(const std::string& fault);

private:
    ThreadSafeQueue<std::string>& commandsQueue;
    ThreadSafeQueue<std::string>& faultsQueue;
    std::thread faultsthread;
    bool running;
};

#endif // FAULTMANAGER_H
