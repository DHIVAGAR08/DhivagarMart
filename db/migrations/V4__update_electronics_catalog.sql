-- db/migrations/V4__update_electronics_catalog.sql
-- Ensure required schema columns exist on products and users tables.
-- Non-destructive: preserves all existing products, prices, stock, seller IDs, users, and relational integrity.

-- 1. Ensure required columns exist on products and users tables before any query uses them
ALTER TABLE products ADD COLUMN IF NOT EXISTS price_cents BIGINT DEFAULT 0;
ALTER TABLE products ADD COLUMN IF NOT EXISTS stock_qty INT DEFAULT 0;
ALTER TABLE products ADD COLUMN IF NOT EXISTS image_url VARCHAR(500) DEFAULT '';
ALTER TABLE products ADD COLUMN IF NOT EXISTS description TEXT DEFAULT '';
ALTER TABLE products ADD COLUMN IF NOT EXISTS category VARCHAR(100) DEFAULT 'General';
ALTER TABLE products ADD COLUMN IF NOT EXISTS created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP;

ALTER TABLE users ADD COLUMN IF NOT EXISTS created_at TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP;

-- 2. Safely backfill legacy columns if they exist
-- Preserves existing nonzero price_cents values and uses a verified numeric conversion rule
DO $$
BEGIN
    IF EXISTS (
        SELECT 1 FROM information_schema.columns 
        WHERE table_schema = current_schema() AND table_name = 'products' AND column_name = 'price'
    ) THEN
        EXECUTE '
            UPDATE products 
            SET price_cents = CASE 
                WHEN price::text ~ ''^[0-9]+(\.[0-9]+)?$'' AND price::numeric >= 1000000 THEN price::BIGINT
                WHEN price::text ~ ''^[0-9]+(\.[0-9]+)?$'' THEN ROUND((price::numeric) * 100)::BIGINT
                ELSE 0
            END
            WHERE (price_cents IS NULL OR price_cents = 0) 
              AND price IS NOT NULL;
        ';
    END IF;

    IF EXISTS (
        SELECT 1 FROM information_schema.columns 
        WHERE table_schema = current_schema() AND table_name = 'products' AND column_name = 'stock'
    ) THEN
        EXECUTE '
            UPDATE products 
            SET stock_qty = stock::INT 
            WHERE (stock_qty IS NULL OR stock_qty = 0) 
              AND stock IS NOT NULL 
              AND stock::text ~ ''^[0-9]+$'';
        ';
    END IF;

    IF EXISTS (
        SELECT 1 FROM information_schema.columns 
        WHERE table_schema = current_schema() AND table_name = 'products' AND column_name = 'image'
    ) THEN
        EXECUTE '
            UPDATE products 
            SET image_url = image::VARCHAR 
            WHERE (image_url IS NULL OR image_url = ''''::VARCHAR) 
              AND image IS NOT NULL;
        ';
    END IF;
END $$;
