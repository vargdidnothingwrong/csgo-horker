#pragma once

#define PAT_LOCALPLAYER "48 89 E5 74 0E 48 8D 05"
#define PAT_LOCALPLAYER_OFF 0x8

#define PAT_GLOWFUNCALL "E8 ?? ?? ?? ?? 48 8B 3D ?? ?? ?? ?? BE 01 00 00 00 C7"
#define PAT_GLOWFUNCALL_OFF 0x1

#define PAT_CONNECTEDMOV "48 8B 05 ?? ?? ?? ?? C6 ?? ?? ?? ?? ?? 00 48 8B 10"
#define PAT_CONNECTEDMOV_OFF 0x9
