// FaultManager.h

#pragma once

#include <queue>
#include <thread>
#include <iostream>
#include "threadsafequeue.h"

/* Declerations for FPS and FDT */
#define MAX_FPS_THRESHOLD       60
#define MIN_FPS_THRESHOLD       15
#define MAX_FDT_THRESHOLD       99
#define MIN_FDT_THRESHOLD       60

/* Declerations for max velocity and steering */
#define MAX_VELOCITY_THRESHOLD  220
#define MAX_STEERING_THRESHOLD  540

class FaultManager
{
public:
    FaultManager(ThreadSafeQueue<std::string>& commandsQueue,ThreadSafeQueue<std::string>& faultsQueue);
    ~FaultManager();

    void faultstart();
    void faultstop();
    void faulthandling(const std::string& fault);

private:
    ThreadSafeQueue<std::string>& commandsQueue;
    ThreadSafeQueue<std::string>& faultsQueue;
    std::thread faultsthread;
    bool running;
    void faultfind();
};


