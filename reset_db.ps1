# Dhivagar Mart - Database Reset & Re-Seed Tool
Write-Host '============================================================' -ForegroundColor Cyan
Write-Host '       🔄 DHIVAGAR MART - DATABASE RESET & RE-SEED' -ForegroundColor Cyan
Write-Host '============================================================' -ForegroundColor Cyan

$psql = 'C:\Program Files\PostgreSQL\18\bin\psql.exe'
$seedFile = Join-Path $PSScriptRoot 'db\seed.sql'

Write-Host 'Resetting database to pristine seeded state...' -ForegroundColor Yellow
& $psql -U postgres -p 5433 -d dhivagarmart -f $seedFile

Write-Host ''
Write-Host '--- Verifying Reset State ---' -ForegroundColor Yellow
& $psql -U postgres -p 5433 -d dhivagarmart -c 'SELECT id, name, category, price_cents, stock_qty FROM products WHERE id <= 3 ORDER BY id;'
& $psql -U postgres -p 5433 -d dhivagarmart -c 'SELECT COUNT(*) AS total_orders FROM orders;'
& $psql -U postgres -p 5433 -d dhivagarmart -c 'SELECT COUNT(*) AS total_reviews FROM reviews;'

Write-Host ''
Write-Host '============================================================' -ForegroundColor Cyan
Write-Host '       ✅ DATABASE RESET COMPLETED: STORE IS PRISTINE' -ForegroundColor Green
Write-Host '============================================================' -ForegroundColor Cyan
