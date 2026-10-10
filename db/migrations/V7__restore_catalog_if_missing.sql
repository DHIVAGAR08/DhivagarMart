-- db/migrations/V7__restore_catalog_if_missing.sql
-- Idempotent Catalog & User Restoration Migration for Dhivagar Mart
-- Safely guarantees all 44 canonical electronics products and platform users are present
-- Uses ON CONFLICT (id) DO NOTHING to preserve all existing users, products, orders, and data.

-- 1. Ensure baseline platform users exist
INSERT INTO users (id, name, email, password_hash, role, created_at) VALUES
(1, 'System Administrator', 'admin@dhivagarmart.com', '$argon2id$v=19$m=65536,t=2,p=1$/gMIaFVSkxkVrUEcRmk4Fw$dXrXmSmKg/CLNNwhD5urIu+zrSJTQQ9KXqkCwwjplgI', 'ADMIN', CURRENT_TIMESTAMP),
(2, 'Fresh Farms Seller', 'seller@dhivagarmart.com', '$argon2id$v=19$m=65536,t=2,p=1$60XS1/dAcJXeCGW9QH49HQ$Qc+9gXEL0gw5j9/pWApfCOo09ypVuWEE/VE5Ix/h9BA', 'SELLER', CURRENT_TIMESTAMP),
(3, 'John Buyer', 'buyer@dhivagarmart.com', '$argon2id$v=19$m=65536,t=2,p=1$Ch4z8sIA7Rv4pqAbb5ncnA$sVJSho07stk6d6fWPtpA9VfBM8tvj4Td41CLva3usgA', 'BUYER', CURRENT_TIMESTAMP)
ON CONFLICT (id) DO NOTHING;

DO $$
DECLARE
    v_useq text;
BEGIN
    v_useq := pg_get_serial_sequence('users', 'id');
    IF v_useq IS NOT NULL THEN
        EXECUTE 'SELECT setval(' || quote_literal(v_useq) || ', (SELECT GREATEST(COALESCE(MAX(id), 0), 3) FROM users))';
    ELSE
        PERFORM setval('users_id_seq', (SELECT GREATEST(COALESCE(MAX(id), 0), 3) FROM users));
    END IF;
END $$;

-- 2. Dynamically resolve seller foreign key and ensure all 44 canonical products exist safely
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

    INSERT INTO products (id, seller_id, name, description, price_cents, stock_qty, category, image_url, created_at) VALUES
    -- Mobiles
    (1, v_seller_id, 'Samsung Galaxy S24 Ultra 5G (512GB, Titanium Gray)', 'Flagship smartphone featuring Galaxy AI, 200MP quad camera, Snapdragon 8 Gen 3, and integrated S-Pen.', 12999900, 45, 'Mobiles', 'https://images.unsplash.com/photo-1610945415295-d9bbf067e59c?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
    (2, v_seller_id, 'Apple iPhone 15 Pro Max (256GB, Natural Titanium)', 'Aerospace-grade titanium design, A17 Pro chip, 48MP main camera with 5x optical telephoto, and Action button.', 13490000, 30, 'Mobiles', 'https://images.unsplash.com/photo-1695048133142-1a20484d2569?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
    (3, v_seller_id, 'OnePlus 12 5G (16GB RAM, 512GB Storage, Silky Black)', '4th Gen Hasselblad Camera System, 5400mAh battery with 100W SUPERVOOC charging, and 2K 120Hz ProXDR display.', 6499900, 50, 'Mobiles', 'https://images.unsplash.com/photo-1598327105666-5b89351aff97?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
    (4, v_seller_id, 'Redmi Note 13 Pro+ 5G (Fusion Purple, 256GB)', '200MP ultra-clear camera with OIS, 120Hz 3D curved AMOLED display, and 120W HyperCharge.', 2999900, 75, 'Mobiles', 'https://images.unsplash.com/photo-1511707171634-5f897ff02aa9?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
    (5, v_seller_id, 'Realme 12 Pro+ 5G (Submarine Blue, 256GB)', 'Periscope portrait camera, luxury watch design by Ollivier Saveo, and 120Hz curved vision display.', 2499900, 60, 'Mobiles', 'https://images.unsplash.com/photo-1565849904461-04a58ad377e0?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
    (35, v_seller_id, 'Redmi 13C 5G (Starshine Green, 128GB)', 'MediaTek Dimensity 6100+ 5G processor, 50MP AI dual camera, and 5000mAh battery.', 1099900, 80, 'Mobiles', 'https://images.unsplash.com/photo-1511707171634-5f897ff02aa9?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
    (36, v_seller_id, 'Apple iPhone 14 (128GB, Midnight)', '6.1-inch Super Retina XDR display, advanced dual-camera system, cinematic mode in 4K Dolby Vision.', 5999900, 35, 'Mobiles', 'https://images.unsplash.com/photo-1695048133142-1a20484d2569?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),

    -- Laptops
    (6, v_seller_id, 'Dell Inspiron 15 Laptop (Intel Core i7 13th Gen, 16GB, 512GB SSD)', 'High-productivity everyday laptop with 15.6-inch FHD 120Hz display, backlit keyboard, and Windows 11.', 6899000, 35, 'Laptops', 'https://images.unsplash.com/photo-1588872657578-7efd1f1555ed?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
    (7, v_seller_id, 'HP Pavilion 15 Laptop (AMD Ryzen 7, 16GB RAM, 1TB SSD)', 'Sleek aluminum body laptop featuring Audio by B&O, micro-edge IPS anti-glare display, and fast charging.', 5499900, 40, 'Laptops', 'https://images.unsplash.com/photo-1544244015-0df4b3ffc6b0?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
    (8, v_seller_id, 'Lenovo IdeaPad Slim 3 (Intel Core i5 12th Gen, 16GB, FHD IPS)', 'Thin and light laptop with military-grade durability, privacy shutter webcam, and rapid charge boost.', 4999900, 50, 'Laptops', 'https://images.unsplash.com/photo-1603302576837-37561b2e2302?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
    (9, v_seller_id, 'ASUS Vivobook 16X (Intel Core i5, RTX 3050 4GB, 16GB RAM)', 'Creator and gaming laptop with 16-inch 120Hz display, ErgoSense keyboard, and IceCool thermal technology.', 6499900, 30, 'Laptops', 'https://images.unsplash.com/photo-1496181133206-80ce9b88a853?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
    (10, v_seller_id, 'Acer Aspire 5 Laptop (13th Gen Intel Core i5, 16GB RAM, 512GB SSD)', 'Versatile multi-tasking laptop with elevated hinge design for cooling, Wi-Fi 6E, and dual stereo speakers.', 5199000, 45, 'Laptops', 'https://images.unsplash.com/photo-1517336714731-489689fd1ca8?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
    (37, v_seller_id, 'ASUS ROG Strix G16 Gaming Laptop (Intel i7 13th Gen, RTX 4060, 16GB, 1TB SSD)', '16-inch FHD+ 165Hz ROG Nebula display, Tri-Fan thermal cooling, and Aura Sync per-key RGB.', 11999900, 20, 'Laptops', 'https://images.unsplash.com/photo-1603302576837-37561b2e2302?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
    (38, v_seller_id, 'Apple MacBook Pro 16-inch (M3 Pro chip, 18GB Unified Memory, 512GB SSD)', 'Extreme dynamic range Liquid Retina XDR display, 22 hours battery life, ProRes hardware acceleration.', 19999900, 15, 'Laptops', 'https://images.unsplash.com/photo-1517336714731-489689fd1ca8?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),

    -- Electrical
    (11, v_seller_id, '55-inch 4K Ultra HD Smart LED TV (Dolby Vision & Atmos)', 'Bezel-less cinema display with Google TV OS, MEMC motion smoothing, and 30W high-fidelity speakers.', 4299900, 25, 'Electrical', 'https://images.unsplash.com/photo-1593359677879-a4bb92f829d1?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
    (12, v_seller_id, 'Stainless Steel 1.8L Fast-Boil Electric Kettle', '1500W rapid boiling element with auto shut-off, boil-dry protection, and 360-degree swivel base.', 149900, 120, 'Electrical', 'https://images.unsplash.com/photo-1570222094114-d054a817e56b?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
    (13, v_seller_id, 'Heavy-Duty 750W 4-Jar Kitchen Mixer Grinder', '100% copper motor with overload protection, stainless steel blades, and dedicated juicer extractor jar.', 349900, 80, 'Electrical', 'https://images.unsplash.com/photo-1584269600464-37b1b58a9fe7?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
    (14, v_seller_id, 'High-Speed Silent Aerodynamic 1200mm Ceiling Fan', 'Energy-efficient BLDC motor with RF remote control, sleep timer, and anti-dust coating.', 249900, 90, 'Electrical', 'https://images.unsplash.com/photo-1585338107529-13afc5f02586?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
    (15, v_seller_id, '1.5 Ton 5-Star Inverter Split Air Conditioner with Wi-Fi', 'Dual inverter compressor, 4-in-1 convertible cooling, PM 2.5 air purification filter, and voice control.', 3899000, 20, 'Electrical', 'https://images.unsplash.com/photo-1621905251189-08b45d6a269e?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
    (39, v_seller_id, 'Samsung 253L 3-Star Inverter Frost Free Double Door Refrigerator', 'Digital inverter technology, all-around cooling, toughened glass shelves, and stabilizer free operation.', 2699000, 25, 'Electrical', 'https://images.unsplash.com/photo-1584269600464-37b1b58a9fe7?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
    (40, v_seller_id, 'LG 7.0 Kg 5-Star Smart Inverter Fully-Automatic Front Load Washing Machine', '6 Motion Direct Drive for optimal fabric wash, Steam allergy care, and touch panel control.', 3199000, 20, 'Electrical', 'https://images.unsplash.com/photo-1621905251189-08b45d6a269e?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),

    -- Audio
    (16, v_seller_id, 'Active Noise Cancelling True Wireless Earbuds (ANC 45dB)', 'High-res spatial audio, dual transparency mode, 40 hours total battery life with wireless charging case.', 449900, 110, 'Audio', 'https://images.unsplash.com/photo-1590658268037-6bf12165a8df?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
    (17, v_seller_id, 'Waterproof 20W Portable Bluetooth Speaker with Bass Boost', 'IPX7 waterproof rating, 360-degree punchy stereo sound, RGB beat lighting, and 16-hour playtime.', 299900, 95, 'Audio', 'https://images.unsplash.com/photo-1608043152269-423dbba4e7e1?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
    (18, v_seller_id, 'Sony WH-1000XM5 Wireless Noise Cancelling Headphones', 'Industry leading noise cancellation, Auto NC Optimizer, 30-hour battery life, and crystal clear hands-free calling.', 2499900, 70, 'Audio', 'https://images.unsplash.com/photo-1505740420928-5e560c06d30e?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
    (19, v_seller_id, '2.1 Channel 160W Soundbar with Wireless Subwoofer', 'Dolby Audio processing, HDMI ARC & optical connectivity, dedicated equalizer presets for movies and music.', 899900, 40, 'Audio', 'https://images.unsplash.com/photo-1545454675-3531b543be5d?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
    (41, v_seller_id, 'boAt Airdopes 141 True Wireless Earbuds (42H Playtime, ENx Tech)', '8mm drivers, ASAP charge (5 mins = 75 mins), IPX4 water resistance, and low-latency BEAST mode.', 129900, 150, 'Audio', 'https://images.unsplash.com/photo-1590658268037-6bf12165a8df?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),

    -- Smart Watches
    (20, v_seller_id, 'Waterproof GPS Fitness Smart Watch with Heart Rate Monitor', 'Precision dual-band GPS, 120+ sport modes, all-day stress and sleep tracking, with 10-day battery life.', 499900, 85, 'Smart Watches', 'https://images.unsplash.com/photo-1579586337278-3befd40fd17a?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
    (21, v_seller_id, 'Bluetooth Calling Smart Watch with AI Voice Assistant', 'Built-in mic and speaker for HD calls, 1.85-inch vibrant HD display, weather updates, and music control.', 299900, 90, 'Smart Watches', 'https://images.unsplash.com/photo-1523275335684-37898b6baf30?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
    (22, v_seller_id, 'AMOLED Display Luxury Smart Watch with Stainless Steel Strap', '1.43-inch Always-On AMOLED screen with 1000 nits brightness, sapphire glass protection, and premium rotating crown.', 799900, 60, 'Smart Watches', 'https://images.unsplash.com/photo-1508685096489-7aacd43bd3b1?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
    (42, v_seller_id, 'Apple Watch Series 9 GPS 45mm (Midnight Aluminum Case)', 'S9 SiP with Double Tap gesture, brighter Always-On display, precision finding for iPhone, and ECG app.', 4490000, 25, 'Smart Watches', 'https://images.unsplash.com/photo-1508685096489-7aacd43bd3b1?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),

    -- Gaming
    (23, v_seller_id, 'Precision 16000 DPI RGB Optical Gaming Mouse', 'Ultra-lightweight ergonomic honeycomb shell, 6 programmable macro buttons, and PTFE smooth glide skates.', 189900, 115, 'Gaming', 'https://images.unsplash.com/photo-1615663245857-ac93bb7c39e7?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
    (24, v_seller_id, 'Tenkeyless RGB Mechanical Gaming Keyboard (Tactile Switches)', 'Aircraft-grade aluminum top plate, per-key RGB backlighting, full N-key rollover, and detachable USB-C braided cable.', 349900, 75, 'Gaming', 'https://images.unsplash.com/photo-1587829741301-dc798b83add3?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
    (25, v_seller_id, 'Wireless Vibration Feedback Game Controller for PC & Console', 'Hall-effect anti-drift analog triggers, dual rumble haptic motors, textured grip, and rechargeable battery.', 279900, 80, 'Gaming', 'https://images.unsplash.com/photo-1600080972464-8e5f35f63d08?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
    (26, v_seller_id, '7.1 Surround Sound Immersive Gaming Headset with Mic', '50mm neodymium audio drivers, memory foam breathable earcups, broadcast-quality detachable mic, and RGB lighting.', 299900, 70, 'Gaming', 'https://images.unsplash.com/photo-1546435770-a3e426bf472b?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
    (43, v_seller_id, 'LG UltraGear 27-inch QHD 165Hz IPS Gaming Monitor (1ms, HDR10)', 'Nano IPS technology, NVIDIA G-SYNC compatible, AMD FreeSync Premium, and borderless design.', 2199900, 30, 'Gaming', 'https://images.unsplash.com/photo-1546435770-a3e426bf472b?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),

    -- Cameras
    (27, v_seller_id, 'Sony Alpha ILCE-6400L 24.2MP Mirrorless Digital Camera (16-50mm Lens)', '24.2MP APS-C sensor, world-fastest 0.02s AF with Real-Time Eye AF & Tracking, 4K HDR video, and 180-degree tiltable LCD.', 7999000, 25, 'Cameras', 'https://images.unsplash.com/photo-1516035069371-29a1b244cc32?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
    (28, v_seller_id, '4K 60FPS Waterproof Dual-Screen Action Camera', 'Electronic image stabilization 4.0, 10m native waterproof housing, 170-degree ultra-wide angle, and Wi-Fi transfer.', 1699900, 50, 'Cameras', 'https://images.unsplash.com/photo-1526170375885-4d8ecf77b99f?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
    (29, v_seller_id, '1080P Full HD Streaming Webcam with Privacy Cover', 'Auto light correction, dual stereo noise-cancelling mics, 90-degree field of view, and tripod-ready universal clip.', 249900, 90, 'Cameras', 'https://images.unsplash.com/photo-1587826080692-f439cd0b70da?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
    (44, v_seller_id, 'Canon EOS R6 Mark II Full-Frame Professional Mirrorless Camera (Body Only)', '24.2MP full-frame CMOS sensor, up to 40 fps electronic shutter, 6K oversampled 4K 60p, Dual Pixel CMOS AF II.', 21599900, 10, 'Cameras', 'https://images.unsplash.com/photo-1516035069371-29a1b244cc32?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),

    -- Accessories
    (30, v_seller_id, '100W Braided USB-C to USB-C Fast Charging Cable (2m)', 'E-Marker smart chip, 480Mbps data sync, heavy-duty nylon braided jacket with 25000+ bend lifespan.', 49900, 200, 'Accessories', 'https://images.unsplash.com/photo-1610492461129-9e8a5b28d7d9?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
    (31, v_seller_id, '20000mAh 22.5W Fast Charging Power Bank with LED Display', 'Triple output ports (2x USB-A, 1x USB-C PD), multi-protection safety system, and accurate digital percentage read-out.', 189900, 120, 'Accessories', 'https://images.unsplash.com/photo-1609592424364-7f15998f45f9?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
    (32, v_seller_id, 'Water-Resistant Padded Laptop Backpack (Fits up to 15.6 inch)', 'Shockproof velvet-lined laptop compartment, anti-theft hidden pocket, luggage strap, and USB charging port pass-through.', 149900, 85, 'Accessories', 'https://images.unsplash.com/photo-1553062407-98eeb64c6a62?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
    (33, v_seller_id, 'Silent 2.4GHz Ergonomic Rechargeable Wireless Mouse', '90% reduced click noise, 3 adjustable DPI levels (800/1200/1600), USB nano receiver, and auto sleep power saving.', 79900, 150, 'Accessories', 'https://images.unsplash.com/photo-1527864550417-7fd91fc51a46?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
    (34, v_seller_id, 'Slim Multi-Device Wireless Bluetooth Keyboard', 'Scissor-switch low-profile keys, switch seamlessly across 3 devices (laptop, tablet, phone), with long-lasting battery.', 129900, 95, 'Accessories', 'https://images.unsplash.com/photo-1587829741301-dc798b83add3?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP)
    ON CONFLICT (id) DO NOTHING;

    -- 3. Safeguard: Ensure canonical products never display as free if they pre-existed with price_cents = 0
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
    UPDATE products SET price_cents = 2499900 WHERE id = 18 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 899900 WHERE id = 19 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 499900 WHERE id = 20 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 299900 WHERE id = 21 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 799900 WHERE id = 22 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 189900 WHERE id = 23 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 349900 WHERE id = 24 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 279900 WHERE id = 25 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 299900 WHERE id = 26 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 7999000 WHERE id = 27 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 1699900 WHERE id = 28 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 249900 WHERE id = 29 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 49900 WHERE id = 30 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 189900 WHERE id = 31 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 149900 WHERE id = 32 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 79900 WHERE id = 33 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 129900 WHERE id = 34 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 1099900 WHERE id = 35 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 5999900 WHERE id = 36 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 11999900 WHERE id = 37 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 19999900 WHERE id = 38 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 2699000 WHERE id = 39 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 3199000 WHERE id = 40 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 129900 WHERE id = 41 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 4490000 WHERE id = 42 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 2199900 WHERE id = 43 AND (price_cents IS NULL OR price_cents = 0);
    UPDATE products SET price_cents = 21599900 WHERE id = 44 AND (price_cents IS NULL OR price_cents = 0);

    -- Reset sequence to maximum product ID
    v_seq := pg_get_serial_sequence('products', 'id');
    IF v_seq IS NOT NULL THEN
        EXECUTE 'SELECT setval(' || quote_literal(v_seq) || ', (SELECT GREATEST(COALESCE(MAX(id), 0), 44) FROM products))';
    ELSE
        PERFORM setval('products_id_seq', (SELECT GREATEST(COALESCE(MAX(id), 0), 44) FROM products));
    END IF;
END $$;
