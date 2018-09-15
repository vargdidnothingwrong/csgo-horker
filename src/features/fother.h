#pragma once

#include "../process.h"
#include "fbase.h"

class FOther final : public FBase {
    Process &m_mem;
    public:
        FOther(Process &mem) : m_mem(mem) {};
    protected:
        void Run();
    private:
        void BHop(uintptr_t localPlayer);
};
