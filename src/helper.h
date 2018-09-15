#pragma once

#include <string>
#include <mutex>
class Helper {
    public:
        static bool Init();
        static void Finalize();
        static bool IsKeyDown(int key);
        static bool IsMouseDown(unsigned int mask);
        static int StringToKeycode(std::string keyString);
        static int KeysymToKeycode(int key);
        static unsigned int StringToMouseMask(std::string buttonString);
    private:
        static std::mutex m_mutex;
};

