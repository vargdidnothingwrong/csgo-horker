#include "helper.h"
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/keysymdef.h>
#include <cstdio>

static Display* m_dpy = nullptr;
bool Helper::Init()
{
    if (m_dpy == nullptr) {
        XInitThreads();
        m_dpy = XOpenDisplay(NULL);
    }
    return (m_dpy != NULL);
}

void Helper::Finalize()
{
    XLockDisplay(m_dpy);
    if (m_dpy != NULL) {
        XCloseDisplay(m_dpy);
    }
    XUnlockDisplay(m_dpy);
}

bool Helper::IsKeyDown(int key)
{
    XLockDisplay(m_dpy);
    char keys[32];
    XQueryKeymap(m_dpy, keys);
    XUnlockDisplay(m_dpy);
    return (keys[key/8] & (1<<(key%8)));
}

bool Helper::IsMouseDown(unsigned int buttonMask)
{
    XLockDisplay(m_dpy);
    Window root = RootWindow(m_dpy, 0);
    Window rootWindow, childWindow;
    int rootX, rootY, winX, winY;
    unsigned int pointerMask;
    bool result = XQueryPointer(m_dpy, root,
            &rootWindow, &childWindow,
            &rootX, &rootY,
            &winX, &winY,
            &pointerMask);
    XUnlockDisplay(m_dpy);
    return (result && (pointerMask & buttonMask));
}

int Helper::StringToKeycode(std::string keyString)
{
    XLockDisplay(m_dpy);
    KeySym ks = XStringToKeysym(keyString.data());
    int keycode = XKeysymToKeycode(m_dpy, ks);
    XUnlockDisplay(m_dpy);
    return keycode;
}

int Helper::KeysymToKeycode(int key)
{
    XLockDisplay(m_dpy);
    int keycode = XKeysymToKeycode(m_dpy, key);
    XUnlockDisplay(m_dpy);
    return keycode;
}

unsigned int Helper::StringToMouseMask(std::string buttonString)
{
    if (buttonString == "Mouse1")
        return Button1Mask;
    if (buttonString == "Mouse2")
        return Button2Mask;
    if (buttonString == "Mouse3")
        return Button3Mask;
    return 0;
}
