#include "faultmanager.h"

FaultManager::FaultManager(ThreadSafeQueue<std::string> &commandsQueue,ThreadSafeQueue<std::string> &faultsQueue) {}

void FaultManager::faultstart()
{
    if (running) {
        std::cerr << "Fault manager is already running." << std::endl;
        return;
    }
    running = true;
    faultsthread = std::thread(&FaultManager::faultfind, this);
}



void FaultManager::faultfind()
{
    std::string fault;
    while (running)
    {
        if (faultsQueue.tryPop(fault))
        {
            std::cout << "Received fault in the fault manager "<< fault << std::endl;
            this->faulthandling(fault);
        }
    }
}

void FaultManager::faulthandling(const std::string &fault)
{

    std::string command;
    //Fault in the preprocessing component
    if (fault.find("PrePrc_fault") != std::string::npos)
    {
        command = "Close_PrePro";
        commandsQueue.push(command);
    }

    //Velocity in vehicle state fault
    else if (fault.find("Velocity_fault") != std::string::npos)
    {
        command = "Velocity_default";
        commandsQueue.push(command);
    }

    // Handle unknown fault
    else
    {
        std::cerr << "Unknown fault: " << fault << std::endl;
    }
}
