# Dhivagar Mart - Automated Runner
# Starts PostgreSQL (if not running), ensures build is present, starts DhivagarMart server, and opens browser.

$ErrorActionPreference = "Stop"

Write-Host "============================================================" -ForegroundColor Cyan
Write-Host "         🛒 DHIVAGAR MART - APPLICATION RUNNER" -ForegroundColor Cyan
Write-Host "============================================================" -ForegroundColor Cyan

# 1. Setup Environment PATH
$env:Path = "$PSScriptRoot\build;$env:USERPROFILE\scoop\shims;$env:USERPROFILE\scoop\apps\gcc\current\bin;C:\Program Files\PostgreSQL\18\bin;$env:Path"

# 2. Check PostgreSQL Daemon
$pgCheck = Get-Process -Name "postgres" -ErrorAction SilentlyContinue
if (-not $pgCheck) {
    Write-Host "[1/3] Starting PostgreSQL service on port 5433..." -ForegroundColor Yellow
    Start-Process -FilePath "C:\Program Files\PostgreSQL\18\bin\postgres.exe" -ArgumentList "-D", "$env:USERPROFILE\dhivagar_pgdata" -WindowStyle Hidden
    Start-Sleep -Seconds 2
} else {
    Write-Host "[1/3] PostgreSQL service is already running on port 5433." -ForegroundColor Green
}

# 3. Check / Build Executable
if (-not (Test-Path "$PSScriptRoot\build\DhivagarMart.exe")) {
    Write-Host "[2/3] Compiling DhivagarMart with Ninja and GCC C++20..." -ForegroundColor Yellow
    cmake --build "$PSScriptRoot\build"
} else {
    Write-Host "[2/3] Executable DhivagarMart.exe is ready." -ForegroundColor Green
}

# 4. Check if server is already running on port 8080
$serverRunning = $false
try {
    $resp = Invoke-RestMethod -Uri "http://localhost:8080/api/v1/health" -TimeoutSec 1 -ErrorAction SilentlyContinue
    if ($resp.status -eq "UP") { $serverRunning = $true }
} catch {}

if ($serverRunning) {
    Write-Host "[3/3] Dhivagar Mart server is already active on http://localhost:8080" -ForegroundColor Green
} else {
    Write-Host "[3/3] Starting Dhivagar Mart server on http://localhost:8080..." -ForegroundColor Cyan
    Start-Process -FilePath "$PSScriptRoot\build\DhivagarMart.exe" -WorkingDirectory "$PSScriptRoot"
    Start-Sleep -Seconds 2
}

# 5. Open browser
Write-Host "Opening Dhivagar Mart Storefront in default browser..." -ForegroundColor Green
Start-Process "http://localhost:8080"
Write-Host "============================================================" -ForegroundColor Cyan
Write-Host "Dhivagar Mart is active at http://localhost:8080" -ForegroundColor White
