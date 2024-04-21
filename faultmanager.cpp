// FaultManager.cpp

#include "faultmanager.h"


//constructor
FaultManager::FaultManager(ThreadSafeQueue<std::string>& commandsQueue,ThreadSafeQueue<std::string>& faultsQueue) 
: commandsQueue(commandsQueue), faultsQueue(faultsQueue), running(false){}

//destructor
FaultManager::~FaultManager() {
    faultstop();
}

//starting the fault manager thread
void FaultManager::faultstart()
{
    if (running) {
        std::cerr << "Fault manager is already running." << std::endl;
        return;
    }
    running = true;
    faultsthread = std::thread(&FaultManager::faultfind, this);
}


//function to release the thread ( add any cleanup needed )
void FaultManager::faultstop() {
    running = false;
    if (faultsthread.joinable()) {
        faultsthread.join();
    }
}


//function to poll on queue to check for any faults sent
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


//function to identify the fault sent
void FaultManager::faulthandling(const std::string &fault)
{

    std::string command;
    //Camera can't connect
    if (fault.find("Camera_disconnected") != std::string::npos)
    {
        command = "Read_video";         //Read from video file
        std::cout<<"Live feed not found, connecting to video"<<std::endl;
        commandsQueue.push(command);
    }

    //FPS more than max threshold or less than min threshold
    else if (fault.find("SET_FPS") != std::string::npos)
    {
        std::cout << "Incorrect FPS sent" << std::endl;
        command = "SET_FPS:" + fault.substr(8);
        commandsQueue.push(command);
    }

    //FDT more than max threshold or less than min threshold
    else if (fault.find("SET_FDT:") != std::string::npos)
    {
        std::cout << "Incorrect FDT sent" << std::endl;
        command = "SET_FDT:" + fault.substr(8);
        commandsQueue.push(command);
    }

    //Fault in the preprocessing component
    else if (fault.find("PrePrc_fault") != std::string::npos)
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
