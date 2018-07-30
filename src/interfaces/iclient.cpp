#include "iclient.h"
#include "../patterns.h"
#include "../sdk/cglowobjectmanager.h"

IClient::IClient(MemoryManager &mem) : IBase(mem)
{
    constexpr char sClient[] = "client_panorama_client.so";

    uintptr_t localPlayerLea = mem.FindInModule(sClient,
        PAT_LOCALPLAYER, PAT_LOCALPLAYER_OFF);
    m_aLocalPlayer = mem.GetCallAddress(localPlayerLea);

    uintptr_t glowFunCall = mem.FindInModule(sClient,
        PAT_GLOWFUNCALL, PAT_GLOWFUNCALL_OFF);
    uintptr_t glowFun = mem.GetCallAddress(glowFunCall);
    m_aGlowPtr = mem.GetCallAddress(glowFun + 0xC);

    uintptr_t playerResourcesMov = mem.FindInModule(sClient,
        "48 8B 05 ?? ?? ?? ?? 55 48 89 E5 48 85 C0 74 10 48", 0x3);
    m_aPlayerResource = mem.GetCallAddress(playerResourcesMov);

    uintptr_t foundAttackMov = mem.FindInModule(sClient,
        "F7 ?? 83 ?? ?? 45 ?? ?? 74", 0xE);
    m_aForceAttack = mem.GetCallAddress(foundAttackMov);

    constexpr char sEngine[] = "engine_client.so";

    uintptr_t connectedMov = m_mem.FindInModule(sEngine,
        PAT_CONNECTEDMOV, PAT_CONNECTEDMOV_OFF);
    m_aConnected = m_mem.GetCallAddress(connectedMov) + 1;
}

bool IClient::GetLocalPlayer(uintptr_t &out)
{
    if (!m_mem.Read(m_aLocalPlayer, out)) {
        return false;
    }
    return true;
}

bool IClient::GetGlowManager(CGlowObjectManager &out)
{
    if (!m_mem.Read(m_aGlowPtr, out)) {
        return false;
    }
    return true;
}

bool IClient::IsConnected()
{
    bool connected = false;
    if (!m_mem.Read(m_aConnected, connected)) {
        return false;
    }
    return connected;
}