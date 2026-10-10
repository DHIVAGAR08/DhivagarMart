-- db/migrations/V6__realistic_electronics_pricing.sql
-- Transition product catalog to realistic Indian high-value electronics retail pricing.
-- Prices are stored in integer minor units (paise: ₹1 = 100 paise).
-- Non-destructive: preserves existing products, prices, stock, seller IDs, and relationships.

DO $$
DECLARE
    v_seller_id INT;
    v_seq TEXT;
BEGIN
    SELECT id INTO v_seller_id FROM users WHERE role = 'SELLER' ORDER BY id ASC LIMIT 1;
    IF v_seller_id IS NULL THEN
        SELECT id INTO v_seller_id FROM users WHERE id = 2;
    END IF;
    IF v_seller_id IS NULL THEN
        SELECT id INTO v_seller_id FROM users ORDER BY id ASC LIMIT 1;
    END IF;
    IF v_seller_id IS NULL THEN
        v_seller_id := 1;
    END IF;

    -- 1. Ensure realistic pricing on canonical products ONLY if price_cents is 0 or NULL
    -- This prevents products from displaying as free while preserving any existing custom prices.
    UPDATE products SET price_cents = 12999900 WHERE id = 1 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 13490000 WHERE id = 2 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 6499900 WHERE id = 3 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 2999900 WHERE id = 4 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 2499900 WHERE id = 5 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 6899000 WHERE id = 6 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 5499900 WHERE id = 7 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 4999900 WHERE id = 8 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 6499900 WHERE id = 9 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 5199000 WHERE id = 10 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 4299900 WHERE id = 11 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 149900 WHERE id = 12 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 349900 WHERE id = 13 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 249900 WHERE id = 14 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 3899000 WHERE id = 15 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 449900 WHERE id = 16 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 299900 WHERE id = 17 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 649900 WHERE id = 18 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 899900 WHERE id = 19 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 499900 WHERE id = 20 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 349900 WHERE id = 21 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 599900 WHERE id = 22 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 189900 WHERE id = 23 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 349900 WHERE id = 24 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 279900 WHERE id = 25 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 299900 WHERE id = 26 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 5699000 WHERE id = 27 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 1499900 WHERE id = 28 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 229900 WHERE id = 29 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 49900 WHERE id = 30 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 189900 WHERE id = 31 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 149900 WHERE id = 32 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 79900 WHERE id = 33 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 129900 WHERE id = 34 AND (price_cents IS NULL OR price_cents = 0);

    -- 2. Additional Market Products for Complete Price Spectrum Coverage (IDs 35-44)
    -- Insert only if not already present, preserving existing products and dynamic seller
    INSERT INTO products (id, seller_id, name, description, price_cents, stock_qty, category, image_url, created_at) VALUES
    (35, v_seller_id, 'Redmi 13C 5G (Starshine Green, 128GB)', 'MediaTek Dimensity 6100+ 5G processor, 50MP AI dual camera, and 5000mAh battery.', 1099900, 80, 'Mobiles', 'https://images.unsplash.com/photo-1511707171634-5f897ff02aa9?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
    (36, v_seller_id, 'Apple iPhone 14 (128GB, Midnight)', '6.1-inch Super Retina XDR display, advanced dual-camera system, cinematic mode in 4K Dolby Vision.', 5999900, 35, 'Mobiles', 'https://images.unsplash.com/photo-1695048133142-1a20484d2569?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
    (37, v_seller_id, 'ASUS ROG Strix G16 Gaming Laptop (Intel i7 13th Gen, RTX 4060, 16GB, 1TB SSD)', '16-inch FHD+ 165Hz ROG Nebula display, Tri-Fan thermal cooling, and Aura Sync per-key RGB.', 11999900, 20, 'Laptops', 'https://images.unsplash.com/photo-1603302576837-37561b2e2302?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
    (38, v_seller_id, 'Apple MacBook Pro 16-inch (M3 Pro chip, 18GB Unified Memory, 512GB SSD)', 'Extreme dynamic range Liquid Retina XDR display, 22 hours battery life, ProRes hardware acceleration.', 19999900, 15, 'Laptops', 'https://images.unsplash.com/photo-1517336714731-489689fd1ca8?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
    (39, v_seller_id, 'Samsung 253L 3-Star Inverter Frost Free Double Door Refrigerator', 'Digital inverter technology, all-around cooling, toughened glass shelves, and stabilizer free operation.', 2699000, 25, 'Electrical', 'https://images.unsplash.com/photo-1584269600464-37b1b58a9fe7?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
    (40, v_seller_id, 'LG 7.0 Kg 5-Star Smart Inverter Fully-Automatic Front Load Washing Machine', '6 Motion Direct Drive for optimal fabric wash, Steam allergy care, and touch panel control.', 3199000, 20, 'Electrical', 'https://images.unsplash.com/photo-1621905251189-08b45d6a269e?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
    (41, v_seller_id, 'boAt Airdopes 141 True Wireless Earbuds (42H Playtime, ENx Tech)', '8mm drivers, ASAP charge (5 mins = 75 mins), IPX4 water resistance, and low-latency BEAST mode.', 129900, 150, 'Audio', 'https://images.unsplash.com/photo-1590658268037-6bf12165a8df?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
    (42, v_seller_id, 'Apple Watch Series 9 GPS 45mm (Midnight Aluminum Case)', 'S9 SiP with Double Tap gesture, brighter Always-On display, precision finding for iPhone, and ECG app.', 4490000, 25, 'Smart Watches', 'https://images.unsplash.com/photo-1508685096489-7aacd43bd3b1?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
    (43, v_seller_id, 'LG UltraGear 27-inch QHD 165Hz IPS Gaming Monitor (1ms, HDR10)', 'Nano IPS technology, NVIDIA G-SYNC compatible, AMD FreeSync Premium, and borderless design.', 2199900, 30, 'Gaming', 'https://images.unsplash.com/photo-1546435770-a3e426bf472b?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
    (44, v_seller_id, 'Canon EOS R6 Mark II Full-Frame Professional Mirrorless Camera (Body Only)', '24.2MP full-frame CMOS sensor, up to 40 fps electronic shutter, 6K oversampled 4K 60p, Dual Pixel CMOS AF II.', 21599900, 10, 'Cameras', 'https://images.unsplash.com/photo-1516035069371-29a1b244cc32?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP)
    ON CONFLICT (id) DO NOTHING;

    -- Update products sequence
    v_seq := pg_get_serial_sequence('products', 'id');
    IF v_seq IS NOT NULL THEN
        EXECUTE 'SELECT setval(' || quote_literal(v_seq) || ', (SELECT GREATEST(COALESCE(MAX(id), 0), 44) FROM products))';
    ELSE
        PERFORM setval('products_id_seq', (SELECT GREATEST(COALESCE(MAX(id), 0), 44) FROM products));
    END IF;
END $$;
