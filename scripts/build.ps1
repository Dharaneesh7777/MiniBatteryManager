$ErrorActionPreference = "Stop"

$root = Split-Path -Parent $PSScriptRoot
$env:Path = "C:\msys64\ucrt64\bin;$env:Path"

New-Item -ItemType Directory -Path "$root\bin" -Force | Out-Null

Write-Host "Building MiniBatteryManager..." -ForegroundColor Cyan

g++ "$root\src\battery.cpp" -o "$root\bin\battery.exe" -Os -s -static

if ($LASTEXITCODE -ne 0) {
    throw "CLI build failed."
}

g++ "$root\src\BatteryService.cpp" -o "$root\bin\BatteryService.exe" -Os -s -static -ladvapi32

if ($LASTEXITCODE -ne 0) {
    throw "Service build failed."
}

Write-Host "Build completed successfully!" -ForegroundColor Green
