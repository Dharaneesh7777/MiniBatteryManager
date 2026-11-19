
#include <windows.h>
#include <cstdio>
#include <cstring>

constexpr DWORD IOCTL = 0x831020F8;

bool Send(HANDLE driver, DWORD command, DWORD& result)
{
    DWORD bytes = 0;

    BOOL success = DeviceIoControl(
        driver, IOCTL,
        &command, sizeof(command),
        &result, sizeof(result),
        &bytes, nullptr
    );

    return success &&
           (command != 0xFF || bytes >= sizeof(result));
}

const char* GetMode(DWORD state)
{
    if (state & 0x20)
        return "Conservation Mode";

    if (state & 0x04)
        return "Rapid Charge";

    return "Normal Mode";
}

int main(int argc, char* argv[])
{
    bool normal = argc == 3 &&
        _stricmp(argv[1], "n") == 0 &&
        _stricmp(argv[2], "mode") == 0;

    bool conservation = argc == 3 &&
        _stricmp(argv[1], "cnv") == 0 &&
        _stricmp(argv[2], "mode") == 0;

    bool check = argc == 2 &&
        _stricmp(argv[1], "status") == 0;

    if (!normal && !conservation && !check)
    {
        printf("MiniBatteryManager\n");
        printf("battery n mode\n");
        printf("battery cnv mode\n");
        printf("battery status\n");
        return 1;
    }

    HANDLE mutex = CreateMutexW(
        nullptr, FALSE, L"Global\\MiniBatteryManagerLock");

    if (!mutex)
    {
        printf("[ERROR] Unable to create mutex.\n");
        return 1;
    }

    DWORD wait = WaitForSingleObject(mutex, 5000);

    if (wait != WAIT_OBJECT_0 && wait != WAIT_ABANDONED)
    {
        CloseHandle(mutex);
        printf("[ERROR] Another battery operation is running.\n");
        return 1;
    }

    HANDLE driver = CreateFileW(
        L"\\\\.\\EnergyDrv",
        FILE_READ_DATA | FILE_WRITE_DATA,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        nullptr, OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL, nullptr
    );

    if (driver == INVALID_HANDLE_VALUE)
    {
        printf("[ERROR] Driver unavailable. Windows error: %lu\n", GetLastError());
        return 1;
    }

    DWORD result = 0;
    int exitCode = 1;

    if (check)
    {
        if (Send(driver, 0xFF, result))
        {
            printf("[STATUS] %s\n", GetMode(result));
            exitCode = 0;
        }
    }
    else
    {
        DWORD first = conservation ? 0x08 : 0x05;
        DWORD second = conservation ? 0x03 : 0x08;

        if (Send(driver, first, result))
        {
            Sleep(100);

            if (Send(driver, second, result))
            {
                for (int i = 0; i < 10; i++)
                {
                    Sleep(100);

                    if (!Send(driver, 0xFF, result))
                        continue;

                    bool verified = conservation
                        ? (result & 0x20) != 0
                        : (result & 0x24) == 0;

                    if (verified)
                    {
                        printf("[SUCCESS] Conservation Mode %s\n",
                            conservation ? "Enabled" : "Disabled");

                        exitCode = 0;
                        break;
                    }
                }
            }
        }
    }

    if (exitCode != 0)
        printf("[ERROR] Operation could not be verified.\n");

    CloseHandle(driver);
    ReleaseMutex(mutex);
    CloseHandle(mutex);
    return exitCode;
}


