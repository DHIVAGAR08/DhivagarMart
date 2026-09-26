-- db/migrations/V5__add_order_details_and_wishlist.sql
-- Adds payment method, payment status, contact, and address fields to orders.
-- Adds wishlist_items table for customer save-for-later functionality.

-- 1. Add order metadata columns
ALTER TABLE orders ADD COLUMN IF NOT EXISTS payment_method VARCHAR(50) DEFAULT 'CASH_ON_DELIVERY';
ALTER TABLE orders ADD COLUMN IF NOT EXISTS payment_status VARCHAR(50) DEFAULT 'PENDING';
ALTER TABLE orders ADD COLUMN IF NOT EXISTS delivery_address TEXT;
ALTER TABLE orders ADD COLUMN IF NOT EXISTS phone VARCHAR(50);
ALTER TABLE orders ADD COLUMN IF NOT EXISTS full_name VARCHAR(150);

-- 2. Update status constraint to include PROCESSING
ALTER TABLE orders DROP CONSTRAINT IF EXISTS orders_status_check;
ALTER TABLE orders ADD CONSTRAINT orders_status_check 
    CHECK (status::text = ANY (ARRAY['PENDING', 'CONFIRMED', 'PROCESSING', 'SHIPPED', 'DELIVERED', 'CANCELLED']));

-- 3. Backfill existing orders with default addresses
UPDATE orders 
SET payment_method = 'CASH_ON_DELIVERY',
    payment_status = CASE WHEN status = 'DELIVERED' THEN 'PAID' ELSE 'PENDING' END,
    delivery_address = '42 Anna Salai, Guindy, Chennai, Tamil Nadu - 600032',
    phone = '+91 98765 43210',
    full_name = 'John Buyer'
WHERE delivery_address IS NULL;

-- 4. Create wishlist_items table
CREATE TABLE IF NOT EXISTS wishlist_items (
    id SERIAL PRIMARY KEY,
    user_id INT NOT NULL REFERENCES users(id) ON DELETE CASCADE,
    product_id INT NOT NULL REFERENCES products(id) ON DELETE CASCADE,
    created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP,
    CONSTRAINT uq_user_product_wishlist UNIQUE (user_id, product_id)
);

CREATE INDEX IF NOT EXISTS idx_wishlist_user_id ON wishlist_items(user_id);
CREATE INDEX IF NOT EXISTS idx_wishlist_product_id ON wishlist_items(product_id);
