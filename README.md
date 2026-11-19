# MiniBatteryManager

A lightweight, native C++ command-line utility for controlling Lenovo laptop battery conservation mode without Lenovo Vantage.

## Features

- Enable and disable Conservation Mode
- Automatic 60% charging limit on the tested laptop
- Native Windows API integration
- No graphical user interface
- No .NET runtime dependency
- No Lenovo Vantage dependency
- No continuous background polling
- Concurrent command protection
- Lightweight executable

## Commands

| Command | Description |
|---|---|
| `battery n mode` | Enable normal charging |
| `battery cnv mode` | Enable Conservation Mode |
| `battery status` | Display current charging mode |

## Performance

| Component | Measured value |
|---|---|
| Original native CLI size | 39.5 KB |
| Experimental background service size | 13 KB |
| Experimental service RAM usage | 4.89 MB |

The background service is experimental and is not required for battery control.

## Installation

Download and extract the release ZIP.

Open PowerShell as Administrator and run:

```powershell
powershell -ExecutionPolicy Bypass -File .\scripts\install.ps1
```

Open a new terminal and run:

```cmd
battery status
```

## Uninstallation

```powershell
powershell -ExecutionPolicy Bypass -File .\scripts\uninstall.ps1
```

## Hardware Compatibility

Tested on:

- Lenovo IdeaPad Gaming 3 15ACH6
- AMD Ryzen 5 5500H
- Windows x64
- Lenovo Energy Management Driver

Other Lenovo models have not been tested.

The charging threshold is controlled by Lenovo firmware.
Custom charging percentages are not supported.

## Build From Source

Requirements:

- MSYS2 UCRT64
- MinGW-w64 GCC
- Windows PowerShell

```powershell
powershell -ExecutionPolicy Bypass -File .\scripts\build.ps1
```

## Technology

- C++
- Win32 API
- DeviceIoControl
- Lenovo EnergyDrv interface
- Windows Mutex
- PowerShell

## Credits

Battery control implementation was developed with reference to Lenovo Legion Toolkit.

https://github.com/LenovoLegionToolkit-Team/LenovoLegionToolkit

## Disclaimer

This is an unofficial third-party utility and is not affiliated with Lenovo.

The Lenovo driver interface is undocumented. Compatibility may vary by hardware and firmware version.

## License

GNU General Public License v3.0
