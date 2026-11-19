#Requires -RunAsAdministrator

$ErrorActionPreference = "Stop"

$root = Split-Path -Parent $PSScriptRoot
$dest = "$env:ProgramFiles\MiniBatteryManager"

Write-Host "Installing MiniBatteryManager..." -ForegroundColor Cyan

if (!(Test-Path "$root\bin\battery.exe")) {
    throw "battery.exe not found."
}

& "$root\bin\battery.exe" status

if ($LASTEXITCODE -ne 0) {
    throw "Compatible Lenovo driver not accessible."
}

New-Item -ItemType Directory -Path $dest -Force | Out-Null

Copy-Item "$root\bin\battery.exe" "$dest\battery.exe" -Force

$path = [Environment]::GetEnvironmentVariable("Path", "Machine")

if (($path -split ";") -notcontains $dest) {
    [Environment]::SetEnvironmentVariable(
        "Path", "$path;$dest", "Machine"
    )
}

Write-Host ""
Write-Host "Installation successful!" -ForegroundColor Green
Write-Host "Open a new CMD or PowerShell window."
Write-Host ""
Write-Host "Available commands:"
Write-Host "battery n mode"
Write-Host "battery cnv mode"
Write-Host "battery status"
