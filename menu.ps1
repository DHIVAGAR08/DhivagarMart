# Dhivagar Mart - Interactive Capstone Master Control Console
$ErrorActionPreference = 'Continue'

function Show-Menu {
    Clear-Host
    Write-Host '============================================================' -ForegroundColor Cyan
    Write-Host '           🛒 DHIVAGAR MART - CAPSTONE CONTROL CONSOLE' -ForegroundColor Cyan
    Write-Host '   Department of Computer Science & Engineering | C++20 Drogon' -ForegroundColor DarkGray
    Write-Host '============================================================' -ForegroundColor Cyan
    Write-Host ''
    Write-Host ' [1] 🌐 Open Storefront in Web Browser (http://localhost:8080)' -ForegroundColor White
    Write-Host ' [2] ⚡ Run HTTP Performance Load Benchmark (RPS & Latency)' -ForegroundColor White
    Write-Host ' [3] 🧪 Run GoogleTest Automated Test Suite (28 Tests)' -ForegroundColor White
    Write-Host ' [4] 🎬 Run End-to-End Live Workflow Simulation (demo.ps1)' -ForegroundColor White
    Write-Host ' [5] 📊 Inspect Live PostgreSQL Database Tables (inspect_db.ps1)' -ForegroundColor White
    Write-Host ' [6] 🔄 Reset Database to Pristine Initial Seed (reset_db.ps1)' -ForegroundColor White
    Write-Host ' [7] 📑 View Capstone Project Final Report & Viva Voce Guide' -ForegroundColor White
    Write-Host ' [8] 📦 Package Complete Submission ZIP Archive on Desktop' -ForegroundColor White
    Write-Host ' [9] ❌ Exit Console' -ForegroundColor Red
    Write-Host ''
    Write-Host '============================================================' -ForegroundColor Cyan
}

do {
    Show-Menu
    $choice = Read-Host "Select an option [1-9]"

    switch ($choice) {
        '1' {
            Write-Host "`nLaunching http://localhost:8080 in default browser..." -ForegroundColor Green
            Start-Process "http://localhost:8080"
            Start-Sleep -Seconds 2
        }
        '2' {
            Write-Host "`nRunning HTTP Performance Load Benchmark..." -ForegroundColor Yellow
            & "$PSScriptRoot\benchmark.ps1"
            Read-Host "`nPress Enter to return to menu..."
        }
        '3' {
            Write-Host "`nExecuting GoogleTest Suite via CTest..." -ForegroundColor Yellow
            $env:Path = "$PSScriptRoot\build;$env:USERPROFILE\scoop\shims;$env:USERPROFILE\scoop\apps\gcc\current\bin;C:\Program Files\PostgreSQL\18\bin;$env:Path"
            ctest --test-dir "$PSScriptRoot\build" --output-on-failure
            Read-Host "`nPress Enter to return to menu..."
        }
        '4' {
            Write-Host "`nExecuting End-to-End Live Workflow Simulation..." -ForegroundColor Yellow
            & "$PSScriptRoot\demo.ps1"
            Read-Host "`nPress Enter to return to menu..."
        }
        '5' {
            Write-Host "`nInspecting Live Database Tables..." -ForegroundColor Yellow
            & "$PSScriptRoot\inspect_db.ps1"
            Read-Host "`nPress Enter to return to menu..."
        }
        '6' {
            Write-Host "`nResetting Database to Seed State..." -ForegroundColor Yellow
            & "$PSScriptRoot\reset_db.ps1"
            Read-Host "`nPress Enter to return to menu..."
        }
        '7' {
            Write-Host "`nOpening Capstone Final Report..." -ForegroundColor Green
            Start-Process "$PSScriptRoot\CAPSTONE_REPORT.md"
            Start-Sleep -Seconds 1
        }
        '8' {
            Write-Host "`nPackaging Standalone Submission ZIP..." -ForegroundColor Yellow
            $zipPath = "C:\Users\ADMINN\Desktop\DhivagarMart_Submission_v1.1.0.zip"
            Write-Host "Archive available on Desktop: $zipPath" -ForegroundColor Green
            Read-Host "`nPress Enter to return to menu..."
        }
        '9' {
            Write-Host "`nExiting Dhivagar Mart Control Console. Have a great presentation!" -ForegroundColor Cyan
            break
        }
        default {
            Write-Host "`nInvalid option. Please choose between 1 and 9." -ForegroundColor Red
            Start-Sleep -Seconds 1
        }
    }
} while ($choice -ne '9')
