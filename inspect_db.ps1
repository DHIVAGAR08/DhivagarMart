# Dhivagar Mart - Live Database Inspector
Write-Host '============================================================' -ForegroundColor Cyan
Write-Host '       📊 DHIVAGAR MART - LIVE DATABASE INSPECTOR' -ForegroundColor Cyan
Write-Host '============================================================' -ForegroundColor Cyan

$psql = 'C:\Program Files\PostgreSQL\18\bin\psql.exe'

Write-Host ''
Write-Host '--- 1. Applied Schema Migrations ---' -ForegroundColor Yellow
& $psql -U postgres -p 5433 -d dhivagarmart -c 'SELECT version, name, applied_at FROM schema_migrations ORDER BY version;'

Write-Host ''
Write-Host '--- 2. Registered Users (Role-Based Access Control) ---' -ForegroundColor Yellow
& $psql -U postgres -p 5433 -d dhivagarmart -c 'SELECT id, name, email, role, created_at FROM users ORDER BY id;'

Write-Host ''
Write-Host '--- 3. Product Inventory & Stock Counts ---' -ForegroundColor Yellow
& $psql -U postgres -p 5433 -d dhivagarmart -c 'SELECT id, name, category, price_cents, stock_qty FROM products WHERE id <= 6 ORDER BY id;'

Write-Host ''
Write-Host '--- 4. Placed Orders & Real-Time Status ---' -ForegroundColor Yellow
& $psql -U postgres -p 5433 -d dhivagarmart -c 'SELECT o.id, u.name AS buyer_name, o.status, o.total_amount_cents, o.created_at FROM orders o JOIN users u ON o.buyer_id = u.id ORDER BY o.id DESC LIMIT 5;'

Write-Host ''
Write-Host '--- 5. Verified Customer Reviews ---' -ForegroundColor Yellow
& $psql -U postgres -p 5433 -d dhivagarmart -c 'SELECT r.id, p.name AS product, u.name AS reviewer, r.rating, r.comment FROM reviews r JOIN products p ON r.product_id = p.id JOIN users u ON r.user_id = u.id ORDER BY r.id DESC;'

Write-Host ''
Write-Host '============================================================' -ForegroundColor Cyan
Write-Host '       DATABASE INSPECTION COMPLETED (PORT 5433)' -ForegroundColor Green
Write-Host '============================================================' -ForegroundColor Cyan
