// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "base.h"
#include "dx11.h"
#include "engine.h"
#include <thread>

HMODULE g_module;
FILE* filePointer;
DWORD __stdcall EjectThread(LPVOID lpParameter)
{
    Sleep(100);
    FreeLibraryAndExitThread(g_module, 0);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        g_module = hModule;
        std::thread([] {
            // 打开控制台
            AllocConsole();
            freopen_s(&filePointer, "CONOUT$", "w", stdout);

            printf("welcome bf hack\n");

            EngineInit();

            DX11Hook();
            while (true)
            {
                if (GetAsyncKeyState(VK_END))
                    break;

                Sleep(200);
            }
            DX11UnHook();

            fclose(filePointer);
            FreeConsole();
            CreateThread(0, 0, EjectThread, 0, 0, 0);
            }).detach();
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

