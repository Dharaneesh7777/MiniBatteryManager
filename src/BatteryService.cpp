
#include <windows.h>

SERVICE_STATUS status = {};
SERVICE_STATUS_HANDLE statusHandle = nullptr;
HANDLE stopEvent = nullptr;

void ReportStatus(DWORD state)
{
    status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    status.dwCurrentState = state;
    status.dwControlsAccepted =
        state == SERVICE_RUNNING
        ? SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN
        : 0;

    SetServiceStatus(statusHandle, &status);
}

void WINAPI ServiceHandler(DWORD control)
{
    if (control == SERVICE_CONTROL_STOP ||
        control == SERVICE_CONTROL_SHUTDOWN)
    {
        ReportStatus(SERVICE_STOP_PENDING);
        SetEvent(stopEvent);
    }
}

void WINAPI ServiceMain(DWORD argc, LPWSTR* argv)
{
    statusHandle = RegisterServiceCtrlHandlerW(
        L"MiniBatteryService", ServiceHandler);

    if (!statusHandle)
        return;

    ReportStatus(SERVICE_START_PENDING);

    stopEvent = CreateEventW(
        nullptr, TRUE, FALSE, nullptr);

    if (!stopEvent)
    {
        status.dwWin32ExitCode = GetLastError();
        ReportStatus(SERVICE_STOPPED);
        return;
    }

    ReportStatus(SERVICE_RUNNING);

    WaitForSingleObject(stopEvent, INFINITE);

    CloseHandle(stopEvent);
    ReportStatus(SERVICE_STOPPED);
}

int main()
{
    SERVICE_TABLE_ENTRYW table[] =
    {
        { (LPWSTR)L"MiniBatteryService", ServiceMain },
        { nullptr, nullptr }
    };

    return StartServiceCtrlDispatcherW(table) ? 0 : 1;
}
