-- db/migrations/V5__add_order_details_and_wishlist.sql
-- Adds payment method, payment status, contact, and address fields to orders.
-- Adds wishlist_items table for customer save-for-later functionality.
-- Non-destructive: preserves all existing orders, statuses, customers, and delivery data.

-- 1. Add order metadata columns with safe defaults
ALTER TABLE orders ADD COLUMN IF NOT EXISTS payment_method VARCHAR(50) DEFAULT 'CASH_ON_DELIVERY';
ALTER TABLE orders ADD COLUMN IF NOT EXISTS payment_status VARCHAR(50) DEFAULT 'PENDING';
ALTER TABLE orders ADD COLUMN IF NOT EXISTS delivery_address TEXT;
ALTER TABLE orders ADD COLUMN IF NOT EXISTS phone VARCHAR(50);
ALTER TABLE orders ADD COLUMN IF NOT EXISTS full_name VARCHAR(150);

-- 2. Backfill only missing metadata with safe defaults without fake addresses or status changes
UPDATE orders 
SET payment_method = 'CASH_ON_DELIVERY' 
WHERE payment_method IS NULL;

UPDATE orders 
SET payment_status = 'PENDING' 
WHERE payment_status IS NULL;

-- Backfill customer name from users table only if null or blank
UPDATE orders o 
SET full_name = u.name 
FROM users u 
WHERE o.buyer_id = u.id 
  AND (o.full_name IS NULL OR o.full_name = '');

-- 3. Create wishlist_items table
CREATE TABLE IF NOT EXISTS wishlist_items (
    id SERIAL PRIMARY KEY,
    user_id INT NOT NULL REFERENCES users(id) ON DELETE CASCADE,
    product_id INT NOT NULL REFERENCES products(id) ON DELETE CASCADE,
    created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP,
    CONSTRAINT uq_user_product_wishlist UNIQUE (user_id, product_id)
);

CREATE INDEX IF NOT EXISTS idx_wishlist_user_id ON wishlist_items(user_id);
CREATE INDEX IF NOT EXISTS idx_wishlist_product_id ON wishlist_items(product_id);
