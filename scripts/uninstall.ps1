#Requires -RunAsAdministrator

$ErrorActionPreference = "Stop"

$dest = "$env:ProgramFiles\MiniBatteryManager"

Write-Host "Uninstalling MiniBatteryManager..." -ForegroundColor Cyan

$path = [Environment]::GetEnvironmentVariable("Path", "Machine")

$newPath = ($path -split ";" | Where-Object {
    $_.TrimEnd('\') -ne $dest.TrimEnd('\')
}) -join ";"

[Environment]::SetEnvironmentVariable(
    "Path", $newPath, "Machine"
)

if (Test-Path "$dest\battery.exe") {
    Remove-Item "$dest\battery.exe" -Force
}

if ((Test-Path $dest) -and !(Get-ChildItem $dest -Force)) {
    Remove-Item $dest -Force
}

Write-Host "Uninstallation successful!" -ForegroundColor Green
Write-Host "Open a new CMD or PowerShell window."
