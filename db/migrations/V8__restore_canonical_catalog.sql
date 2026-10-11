-- db/migrations/V8__restore_canonical_catalog.sql
-- Restoration migration for Dhivagar Mart 44-product electronics catalog.
-- Non-destructive: preserves all existing users, orders, reviews, carts, wishlists, and live products (IDs 1-6).

DO $$
DECLARE
    v_seller_id INT;
    v_seq TEXT;
    r RECORD;
BEGIN
    -- 1. Safely normalize categories on verified live products to align with frontend filter logic
    -- Preserves existing live product names, prices, stock, seller ownership, and IDs
    UPDATE products SET category = 'Mobiles' WHERE id IN (1, 4) AND category = 'Smartphones';
    UPDATE products SET category = 'Smart Watches' WHERE id = 3 AND category = 'Wearables';
    UPDATE products SET category = 'Electrical' WHERE id = 6 AND category = 'Televisions';

    -- 2. Dynamically resolve a verified seller ID from existing records with foreign-key and role checks
    -- Priority 1: Use existing seller_id from live products 1-6 if the user exists and has SELLER role
    SELECT u.id INTO v_seller_id
    FROM products p
    JOIN users u ON p.seller_id = u.id
    WHERE u.role = 'SELLER'
    ORDER BY p.id ASC
    LIMIT 1;

    -- Priority 2: Fallback to any verified user with role = 'SELLER'
    IF v_seller_id IS NULL THEN
        SELECT id INTO v_seller_id FROM users WHERE role = 'SELLER' ORDER BY id ASC LIMIT 1;
    END IF;

    -- Priority 3: Fallback to existing seller_id of products (ensuring foreign-key constraint is met)
    IF v_seller_id IS NULL THEN
        SELECT p.seller_id INTO v_seller_id 
        FROM products p 
        JOIN users u ON p.seller_id = u.id 
        ORDER BY p.id ASC 
        LIMIT 1;
    END IF;

    -- Priority 4: Fallback to lowest-ID ADMIN user
    IF v_seller_id IS NULL THEN
        SELECT id INTO v_seller_id FROM users WHERE role = 'ADMIN' ORDER BY id ASC LIMIT 1;
    END IF;

    IF v_seller_id IS NULL THEN
        RAISE EXCEPTION 'Cannot restore catalog: no verified seller or user found to satisfy seller_id foreign-key constraint';
    END IF;

    -- 3. Restore canonical catalog products (IDs 7 to 44) from db/seed.sql without overwriting existing products
    INSERT INTO products (id, seller_id, name, description, price_cents, stock_qty, category, image_url, created_at)
    SELECT v.id, v_seller_id, v.name, v.description, v.price_cents, v.stock_qty, v.category, v.image_url, CURRENT_TIMESTAMP
    FROM (VALUES
        -- Laptops
        (7, 'HP Pavilion 15 Laptop (AMD Ryzen 7, 16GB RAM, 1TB SSD)', 'Sleek aluminum body laptop featuring Audio by B&O, micro-edge IPS anti-glare display, and fast charging.', 5499900::bigint, 40, 'Laptops', 'https://images.unsplash.com/photo-1544244015-0df4b3ffc6b0?w=600&auto=format&fit=crop&q=80'),
        (8, 'Lenovo IdeaPad Slim 3 (Intel Core i5 12th Gen, 16GB, FHD IPS)', 'Thin and light laptop with military-grade durability, privacy shutter webcam, and rapid charge boost.', 4999900::bigint, 50, 'Laptops', 'https://images.unsplash.com/photo-1603302576837-37561b2e2302?w=600&auto=format&fit=crop&q=80'),
        (9, 'ASUS Vivobook 16X (Intel Core i5, RTX 3050 4GB, 16GB RAM)', 'Creator and gaming laptop with 16-inch 120Hz display, ErgoSense keyboard, and IceCool thermal technology.', 6499900::bigint, 30, 'Laptops', 'https://images.unsplash.com/photo-1496181133206-80ce9b88a853?w=600&auto=format&fit=crop&q=80'),
        (10, 'Acer Aspire 5 Laptop (13th Gen Intel Core i5, 16GB RAM, 512GB SSD)', 'Versatile multi-tasking laptop with elevated hinge design for cooling, Wi-Fi 6E, and dual stereo speakers.', 5199000::bigint, 45, 'Laptops', 'https://images.unsplash.com/photo-1517336714731-489689fd1ca8?w=600&auto=format&fit=crop&q=80'),
        (37, 'ASUS ROG Strix G16 Gaming Laptop (Intel i7 13th Gen, RTX 4060, 16GB, 1TB SSD)', '16-inch FHD+ 165Hz ROG Nebula display, Tri-Fan thermal cooling, and Aura Sync per-key RGB.', 11999900::bigint, 20, 'Laptops', 'https://images.unsplash.com/photo-1603302576837-37561b2e2302?w=600&auto=format&fit=crop&q=80'),
        (38, 'Apple MacBook Pro 16-inch (M3 Pro chip, 18GB Unified Memory, 512GB SSD)', 'Extreme dynamic range Liquid Retina XDR display, 22 hours battery life, ProRes hardware acceleration.', 19999900::bigint, 15, 'Laptops', 'https://images.unsplash.com/photo-1517336714731-489689fd1ca8?w=600&auto=format&fit=crop&q=80'),

        -- Electrical
        (11, '55-inch 4K Ultra HD Smart LED TV (Dolby Vision & Atmos)', 'Bezel-less cinema display with Google TV OS, MEMC motion smoothing, and 30W high-fidelity speakers.', 4299900::bigint, 25, 'Electrical', 'https://images.unsplash.com/photo-1593359677879-a4bb92f829d1?w=600&auto=format&fit=crop&q=80'),
        (12, 'Stainless Steel 1.8L Fast-Boil Electric Kettle', '1500W rapid boiling element with auto shut-off, boil-dry protection, and 360-degree swivel base.', 149900::bigint, 120, 'Electrical', 'https://images.unsplash.com/photo-1570222094114-d054a817e56b?w=600&auto=format&fit=crop&q=80'),
        (13, 'Heavy-Duty 750W 4-Jar Kitchen Mixer Grinder', '100% copper motor with overload protection, stainless steel blades, and dedicated juicer extractor jar.', 349900::bigint, 80, 'Electrical', 'https://images.unsplash.com/photo-1584269600464-37b1b58a9fe7?w=600&auto=format&fit=crop&q=80'),
        (14, 'High-Speed Silent Aerodynamic 1200mm Ceiling Fan', 'Energy-efficient BLDC motor with RF remote control, sleep timer, and anti-dust coating.', 249900::bigint, 90, 'Electrical', 'https://images.unsplash.com/photo-1585338107529-13afc5f02586?w=600&auto=format&fit=crop&q=80'),
        (15, '1.5 Ton 5-Star Inverter Split Air Conditioner with Wi-Fi', 'Dual inverter compressor, 4-in-1 convertible cooling, PM 2.5 air purification filter, and voice control.', 3899000::bigint, 20, 'Electrical', 'https://images.unsplash.com/photo-1621905251189-08b45d6a269e?w=600&auto=format&fit=crop&q=80'),
        (39, 'Samsung 253L 3-Star Inverter Frost Free Double Door Refrigerator', 'Digital inverter technology, all-around cooling, toughened glass shelves, and stabilizer free operation.', 2699000::bigint, 25, 'Electrical', 'https://images.unsplash.com/photo-1584269600464-37b1b58a9fe7?w=600&auto=format&fit=crop&q=80'),
        (40, 'LG 7.0 Kg 5-Star Smart Inverter Fully-Automatic Front Load Washing Machine', '6 Motion Direct Drive for optimal fabric wash, Steam allergy care, and touch panel control.', 3199000::bigint, 20, 'Electrical', 'https://images.unsplash.com/photo-1621905251189-08b45d6a269e?w=600&auto=format&fit=crop&q=80'),

        -- Audio
        (16, 'Active Noise Cancelling True Wireless Earbuds (ANC 45dB)', 'High-res spatial audio, dual transparency mode, 40 hours total battery life with wireless charging case.', 449900::bigint, 110, 'Audio', 'https://images.unsplash.com/photo-1590658268037-6bf12165a8df?w=600&auto=format&fit=crop&q=80'),
        (17, 'Waterproof 20W Portable Bluetooth Speaker with Bass Boost', 'IPX7 waterproof rating, 360-degree punchy stereo sound, RGB beat lighting, and 16-hour playtime.', 299900::bigint, 95, 'Audio', 'https://images.unsplash.com/photo-1608043152269-423dbba4e7e1?w=600&auto=format&fit=crop&q=80'),
        (18, 'Sony WH-1000XM5 Wireless Noise Cancelling Headphones', 'Industry leading noise cancellation, Auto NC Optimizer, 30-hour battery life, and crystal clear hands-free calling.', 2499900::bigint, 70, 'Audio', 'https://images.unsplash.com/photo-1505740420928-5e560c06d30e?w=600&auto=format&fit=crop&q=80'),
        (19, '2.1 Channel 160W Soundbar with Wireless Subwoofer', 'Dolby Audio processing, HDMI ARC & optical connectivity, dedicated equalizer presets for movies and music.', 899900::bigint, 40, 'Audio', 'https://images.unsplash.com/photo-1545454675-3531b543be5d?w=600&auto=format&fit=crop&q=80'),
        (41, 'boAt Airdopes 141 True Wireless Earbuds (42H Playtime, ENx Tech)', '8mm drivers, ASAP charge (5 mins = 75 mins), IPX4 water resistance, and low-latency BEAST mode.', 129900::bigint, 150, 'Audio', 'https://images.unsplash.com/photo-1590658268037-6bf12165a8df?w=600&auto=format&fit=crop&q=80'),

        -- Smart Watches
        (20, 'Waterproof GPS Fitness Smart Watch with Heart Rate Monitor', 'Precision dual-band GPS, 120+ sport modes, all-day stress and sleep tracking, with 10-day battery life.', 499900::bigint, 85, 'Smart Watches', 'https://images.unsplash.com/photo-1579586337278-3befd40fd17a?w=600&auto=format&fit=crop&q=80'),
        (21, 'Bluetooth Calling Smart Watch with AI Voice Assistant', 'Built-in mic and speaker for HD calls, 1.85-inch vibrant HD display, weather updates, and music control.', 299900::bigint, 90, 'Smart Watches', 'https://images.unsplash.com/photo-1523275335684-37898b6baf30?w=600&auto=format&fit=crop&q=80'),
        (22, 'AMOLED Display Luxury Smart Watch with Stainless Steel Strap', '1.43-inch Always-On AMOLED screen with 1000 nits brightness, sapphire glass protection, and premium rotating crown.', 799900::bigint, 60, 'Smart Watches', 'https://images.unsplash.com/photo-1508685096489-7aacd43bd3b1?w=600&auto=format&fit=crop&q=80'),
        (42, 'Apple Watch Series 9 GPS 45mm (Midnight Aluminum Case)', 'S9 SiP with Double Tap gesture, brighter Always-On display, precision finding for iPhone, and ECG app.', 4490000::bigint, 25, 'Smart Watches', 'https://images.unsplash.com/photo-1508685096489-7aacd43bd3b1?w=600&auto=format&fit=crop&q=80'),

        -- Gaming
        (23, 'Precision 16000 DPI RGB Optical Gaming Mouse', 'Ultra-lightweight ergonomic honeycomb shell, 6 programmable macro buttons, and PTFE smooth glide skates.', 189900::bigint, 115, 'Gaming', 'https://images.unsplash.com/photo-1615663245857-ac93bb7c39e7?w=600&auto=format&fit=crop&q=80'),
        (24, 'Tenkeyless RGB Mechanical Gaming Keyboard (Tactile Switches)', 'Aircraft-grade aluminum top plate, per-key RGB backlighting, full N-key rollover, and detachable USB-C braided cable.', 349900::bigint, 75, 'Gaming', 'https://images.unsplash.com/photo-1587829741301-dc798b83add3?w=600&auto=format&fit=crop&q=80'),
        (25, 'Wireless Vibration Feedback Game Controller for PC & Console', 'Hall-effect anti-drift analog triggers, dual rumble haptic motors, textured grip, and rechargeable battery.', 279900::bigint, 80, 'Gaming', 'https://images.unsplash.com/photo-1600080972464-8e5f35f63d08?w=600&auto=format&fit=crop&q=80'),
        (26, '7.1 Surround Sound Immersive Gaming Headset with Mic', '50mm neodymium audio drivers, memory foam breathable earcups, broadcast-quality detachable mic, and RGB lighting.', 299900::bigint, 70, 'Gaming', 'https://images.unsplash.com/photo-1546435770-a3e426bf472b?w=600&auto=format&fit=crop&q=80'),
        (43, 'LG UltraGear 27-inch QHD 165Hz IPS Gaming Monitor (1ms, HDR10)', 'Nano IPS technology, NVIDIA G-SYNC compatible, AMD FreeSync Premium, and borderless design.', 2199900::bigint, 30, 'Gaming', 'https://images.unsplash.com/photo-1546435770-a3e426bf472b?w=600&auto=format&fit=crop&q=80'),

        -- Cameras
        (27, 'Sony Alpha ILCE-6400L 24.2MP Mirrorless Digital Camera (16-50mm Lens)', '24.2MP APS-C sensor, world-fastest 0.02s AF with Real-Time Eye AF & Tracking, 4K HDR video, and 180-degree tiltable LCD.', 7999000::bigint, 25, 'Cameras', 'https://images.unsplash.com/photo-1516035069371-29a1b244cc32?w=600&auto=format&fit=crop&q=80'),
        (28, '4K 60FPS Waterproof Dual-Screen Action Camera', 'Electronic image stabilization 4.0, 10m native waterproof housing, 170-degree ultra-wide angle, and Wi-Fi transfer.', 1699900::bigint, 50, 'Cameras', 'https://images.unsplash.com/photo-1526170375885-4d8ecf77b99f?w=600&auto=format&fit=crop&q=80'),
        (29, '1080P Full HD Streaming Webcam with Privacy Cover', 'Auto light correction, dual stereo noise-cancelling mics, 90-degree field of view, and tripod-ready universal clip.', 249900::bigint, 90, 'Cameras', 'https://images.unsplash.com/photo-1587826080692-f439cd0b70da?w=600&auto=format&fit=crop&q=80'),
        (44, 'Canon EOS R6 Mark II Full-Frame Professional Mirrorless Camera (Body Only)', '24.2MP full-frame CMOS sensor, up to 40 fps electronic shutter, 6K oversampled 4K 60p, Dual Pixel CMOS AF II.', 21599900::bigint, 10, 'Cameras', 'https://images.unsplash.com/photo-1516035069371-29a1b244cc32?w=600&auto=format&fit=crop&q=80'),

        -- Accessories
        (30, '100W Braided USB-C to USB-C Fast Charging Cable (2m)', 'E-Marker smart chip, 480Mbps data sync, heavy-duty nylon braided jacket with 25000+ bend lifespan.', 49900::bigint, 200, 'Accessories', 'https://images.unsplash.com/photo-1610492461129-9e8a5b28d7d9?w=600&auto=format&fit=crop&q=80'),
        (31, '20000mAh 22.5W Fast Charging Power Bank with LED Display', 'Triple output ports (2x USB-A, 1x USB-C PD), multi-protection safety system, and accurate digital percentage read-out.', 189900::bigint, 120, 'Accessories', 'https://images.unsplash.com/photo-1609592424364-7f15998f45f9?w=600&auto=format&fit=crop&q=80'),
        (32, 'Water-Resistant Padded Laptop Backpack (Fits up to 15.6 inch)', 'Shockproof velvet-lined laptop compartment, anti-theft hidden pocket, luggage strap, and USB charging port pass-through.', 149900::bigint, 85, 'Accessories', 'https://images.unsplash.com/photo-1553062407-98eeb64c6a62?w=600&auto=format&fit=crop&q=80'),
        (33, 'Silent 2.4GHz Ergonomic Rechargeable Wireless Mouse', '90% reduced click noise, 3 adjustable DPI levels (800/1200/1600), USB nano receiver, and auto sleep power saving.', 79900::bigint, 150, 'Accessories', 'https://images.unsplash.com/photo-1527864550417-7fd91fc51a46?w=600&auto=format&fit=crop&q=80'),
        (34, 'Slim Multi-Device Wireless Bluetooth Keyboard', 'Scissor-switch low-profile keys, switch seamlessly across 3 devices (laptop, tablet, phone), with long-lasting battery.', 129900::bigint, 95, 'Accessories', 'https://images.unsplash.com/photo-1587829741301-dc798b83add3?w=600&auto=format&fit=crop&q=80'),

        -- Mobiles
        (35, 'Redmi 13C 5G (Starshine Green, 128GB)', 'MediaTek Dimensity 6100+ 5G processor, 50MP AI dual camera, and 5000mAh battery.', 1099900::bigint, 80, 'Mobiles', 'https://images.unsplash.com/photo-1511707171634-5f897ff02aa9?w=600&auto=format&fit=crop&q=80'),
        (36, 'Apple iPhone 14 (128GB, Midnight)', '6.1-inch Super Retina XDR display, advanced dual-camera system, cinematic mode in 4K Dolby Vision.', 5999900::bigint, 35, 'Mobiles', 'https://images.unsplash.com/photo-1695048133142-1a20484d2569?w=600&auto=format&fit=crop&q=80')
    ) AS v(id, name, description, price_cents, stock_qty, category, image_url)
    WHERE NOT EXISTS (
        SELECT 1 FROM products p 
        WHERE p.id = v.id OR LOWER(p.name) = LOWER(v.name)
    );

    -- 4. Safely synchronize products sequence to highest ID
    v_seq := pg_get_serial_sequence('products', 'id');
    IF v_seq IS NOT NULL THEN
        EXECUTE 'SELECT setval(' || quote_literal(v_seq) || ', (SELECT GREATEST(COALESCE(MAX(id), 0), 44) FROM products), true)';
    END IF;

    -- 5. Assert complete and exact catalog restoration (must be exactly 44 products)
    IF (SELECT COUNT(*) FROM products) != 44 THEN
        RAISE EXCEPTION 'Catalog restoration assertion failed: expected exactly 44 products, but found %', (SELECT COUNT(*) FROM products);
    END IF;
END $$;
