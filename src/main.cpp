#include "memorymanager.h"
#include "offsets.h"
#include "features/fglow.h"
#include "interfaces/iclient.h"
#include "sdk/cbaseentity.h"
#include "sdk/cglowobjectmanager.h"
#include "sdk/types.h"

#include <chrono>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <thread>

#include <signal.h>
#include <unistd.h>

bool shouldQuit = false;

#define LOG(X) std::cout << X << std::flush

void exitHandle(int s)
{
    std::cout << "\nCaught signal: " << s << '\n';
    shouldQuit = true;
}

int main()
{
    if (getuid() != 0) {
        LOG("This program must be ran as root.\n");
        return 0;
    }
    
    LOG("Attaching interrupt signal handler...");
    
    struct sigaction ccHandle;
    ccHandle.sa_handler = exitHandle;
    sigemptyset(&ccHandle.sa_mask);
    ccHandle.sa_flags = 0;
    sigaction(SIGINT, &ccHandle, NULL);
    
    LOG("Done.\n");

    MemoryManager mem("csgo_linux64");
    
    LOG("Waiting for process...");
    
    while (!mem.Attach() && !shouldQuit) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    if (shouldQuit) {
        return 0;
    }

    LOG("Done.\n");
    LOG("Waiting for client library...");

    while (!shouldQuit) {
        mem.ParseModules();
        if (mem.GetModuleStart("client_panorama_client.so") != 0) {
            break;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    if (shouldQuit) {
        return 0;
    }

    LOG("Done.\n");
    LOG("Finding offset addresses...");
    
    IClient client(mem);
    
    LOG("Done.\n");
    LOG("> Main Start <\n");

    FGlow tGlow;
    bool tRunning = false;

    while (!shouldQuit) {
        if (!mem.IsValid()) {
            shouldQuit = true;
            LOG("Lost connection to process... Exiting\n");
            break;
        }
        if (client.IsConnected() && !tRunning) {
            tRunning = true;
            std::thread th1([&]() { tGlow.Run(mem, client); });
            while (client.IsConnected() && tRunning && !shouldQuit) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            tRunning = false;
            tGlow.Stop();
            th1.join();
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;
}