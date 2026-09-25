-- db/migrations/V4__update_electronics_catalog.sql
-- Transition marketplace catalog from grocery domain to high-performance Electronics & Technology domain.
-- Preserves all relational constraints, users, and audit records.

-- Update existing product rows (1-16) to Electronics products
UPDATE products SET 
    name = 'Samsung Galaxy S24 Ultra 5G (512GB, Titanium Gray)',
    description = 'Flagship smartphone featuring Galaxy AI, 200MP quad camera, Snapdragon 8 Gen 3, and integrated S-Pen.',
    price_cents = 12999900,
    stock_qty = 45,
    category = 'Mobiles',
    image_url = 'https://images.unsplash.com/photo-1610945415295-d9bbf067e59c?w=600&auto=format&fit=crop&q=80'
WHERE id = 1;

UPDATE products SET 
    name = 'Apple iPhone 15 Pro Max (256GB, Natural Titanium)',
    description = 'Aerospace-grade titanium design, A17 Pro chip, 48MP main camera with 5x optical telephoto, and Action button.',
    price_cents = 13490000,
    stock_qty = 30,
    category = 'Mobiles',
    image_url = 'https://images.unsplash.com/photo-1695048133142-1a20484d2569?w=600&auto=format&fit=crop&q=80'
WHERE id = 2;

UPDATE products SET 
    name = 'OnePlus 12 5G (16GB RAM, 512GB Storage, Silky Black)',
    description = '4th Gen Hasselblad Camera System, 5400mAh battery with 100W SUPERVOOC charging, and 2K 120Hz ProXDR display.',
    price_cents = 6499900,
    stock_qty = 50,
    category = 'Mobiles',
    image_url = 'https://images.unsplash.com/photo-1598327105666-5b89351aff97?w=600&auto=format&fit=crop&q=80'
WHERE id = 3;

UPDATE products SET 
    name = 'Redmi Note 13 Pro+ 5G (Fusion Purple, 256GB)',
    description = '200MP ultra-clear camera with OIS, 120Hz 3D curved AMOLED display, and 120W HyperCharge.',
    price_cents = 2999900,
    stock_qty = 75,
    category = 'Mobiles',
    image_url = 'https://images.unsplash.com/photo-1511707171634-5f897ff02aa9?w=600&auto=format&fit=crop&q=80'
WHERE id = 4;

UPDATE products SET 
    name = 'Realme 12 Pro+ 5G (Submarine Blue, 256GB)',
    description = 'Periscope portrait camera, luxury watch design by Ollivier Saveo, and 120Hz curved vision display.',
    price_cents = 2999900,
    stock_qty = 60,
    category = 'Mobiles',
    image_url = 'https://images.unsplash.com/photo-1565849904461-04a58ad377e0?w=600&auto=format&fit=crop&q=80'
WHERE id = 5;

UPDATE products SET 
    name = 'Dell Inspiron 15 Laptop (Intel Core i7 13th Gen, 16GB, 512GB SSD)',
    description = 'High-productivity everyday laptop with 15.6-inch FHD 120Hz display, backlit keyboard, and Windows 11.',
    price_cents = 6899000,
    stock_qty = 35,
    category = 'Laptops',
    image_url = 'https://images.unsplash.com/photo-1588872657578-7efd1f1555ed?w=600&auto=format&fit=crop&q=80'
WHERE id = 6;

UPDATE products SET 
    name = 'HP Pavilion 15 Laptop (AMD Ryzen 7, 16GB RAM, 1TB SSD)',
    description = 'Sleek aluminum body laptop featuring Audio by B&O, micro-edge IPS anti-glare display, and fast charging.',
    price_cents = 6449000,
    stock_qty = 40,
    category = 'Laptops',
    image_url = 'https://images.unsplash.com/photo-1544244015-0df4b3ffc6b0?w=600&auto=format&fit=crop&q=80'
WHERE id = 7;

UPDATE products SET 
    name = 'Lenovo IdeaPad Slim 3 (Intel Core i5 12th Gen, 16GB, FHD IPS)',
    description = 'Thin and light laptop with military-grade durability, privacy shutter webcam, and rapid charge boost.',
    price_cents = 4999000,
    stock_qty = 50,
    category = 'Laptops',
    image_url = 'https://images.unsplash.com/photo-1603302576837-37561b2e2302?w=600&auto=format&fit=crop&q=80'
WHERE id = 8;

UPDATE products SET 
    name = 'ASUS Vivobook 16X (Intel Core i5, RTX 3050 4GB, 16GB RAM)',
    description = 'Creator and gaming laptop with 16-inch 120Hz display, ErgoSense keyboard, and IceCool thermal technology.',
    price_cents = 6299000,
    stock_qty = 30,
    category = 'Laptops',
    image_url = 'https://images.unsplash.com/photo-1496181133206-80ce9b88a853?w=600&auto=format&fit=crop&q=80'
WHERE id = 9;

UPDATE products SET 
    name = 'Acer Aspire 5 Laptop (13th Gen Intel Core i5, 16GB RAM, 512GB SSD)',
    description = 'Versatile multi-tasking laptop with elevated hinge design for cooling, Wi-Fi 6E, and dual stereo speakers.',
    price_cents = 5199000,
    stock_qty = 45,
    category = 'Laptops',
    image_url = 'https://images.unsplash.com/photo-1517336714731-489689fd1ca8?w=600&auto=format&fit=crop&q=80'
WHERE id = 10;

UPDATE products SET 
    name = '55-inch 4K Ultra HD Smart LED TV (Dolby Vision & Atmos)',
    description = 'Bezel-less cinema display with Google TV OS, MEMC motion smoothing, and 30W high-fidelity speakers.',
    price_cents = 4299900,
    stock_qty = 25,
    category = 'Electrical',
    image_url = 'https://images.unsplash.com/photo-1593359677879-a4bb92f829d1?w=600&auto=format&fit=crop&q=80'
WHERE id = 11;

UPDATE products SET 
    name = 'Stainless Steel 1.8L Fast-Boil Electric Kettle',
    description = '1500W rapid boiling element with auto shut-off, boil-dry protection, and 360-degree swivel base.',
    price_cents = 149900,
    stock_qty = 120,
    category = 'Electrical',
    image_url = 'https://images.unsplash.com/photo-1570222094114-d054a817e56b?w=600&auto=format&fit=crop&q=80'
WHERE id = 12;

UPDATE products SET 
    name = 'Heavy-Duty 750W 4-Jar Kitchen Mixer Grinder',
    description = '100% copper motor with overload protection, stainless steel blades, and dedicated juicer extractor jar.',
    price_cents = 329900,
    stock_qty = 80,
    category = 'Electrical',
    image_url = 'https://images.unsplash.com/photo-1584269600464-37b1b58a9fe7?w=600&auto=format&fit=crop&q=80'
WHERE id = 13;

UPDATE products SET 
    name = 'High-Speed Silent Aerodynamic 1200mm Ceiling Fan',
    description = 'Energy-efficient BLDC motor with RF remote control, sleep timer, and anti-dust coating.',
    price_cents = 239900,
    stock_qty = 90,
    category = 'Electrical',
    image_url = 'https://images.unsplash.com/photo-1585338107529-13afc5f02586?w=600&auto=format&fit=crop&q=80'
WHERE id = 14;

UPDATE products SET 
    name = '1.5 Ton 5-Star Inverter Split Air Conditioner with Wi-Fi',
    description = 'Dual inverter compressor, 4-in-1 convertible cooling, PM 2.5 air purification filter, and voice control.',
    price_cents = 3899000,
    stock_qty = 20,
    category = 'Electrical',
    image_url = 'https://images.unsplash.com/photo-1621905251189-08b45d6a269e?w=600&auto=format&fit=crop&q=80'
WHERE id = 15;

UPDATE products SET 
    name = 'Active Noise Cancelling True Wireless Earbuds (ANC 45dB)',
    description = 'High-res spatial audio, dual transparency mode, 40 hours total battery life with wireless charging case.',
    price_cents = 449900,
    stock_qty = 110,
    category = 'Audio',
    image_url = 'https://images.unsplash.com/photo-1590658268037-6bf12165a8df?w=600&auto=format&fit=crop&q=80'
WHERE id = 16;

-- Insert remaining new electronics products (IDs 17 to 34) if not already inserted
INSERT INTO products (id, seller_id, name, description, price_cents, stock_qty, category, image_url)
VALUES
(17, 2, 'Waterproof 20W Portable Bluetooth Speaker with Bass Boost', 'IPX7 waterproof rating, 360-degree punchy stereo sound, RGB beat lighting, and 16-hour playtime.', 299900, 95, 'Audio', 'https://images.unsplash.com/photo-1608043152269-423dbba4e7e1?w=600&auto=format&fit=crop&q=80'),
(18, 2, 'Over-Ear Wireless Studio Headphones with Hi-Res Audio', 'Plush memory foam earcups, 40mm titanium drivers, multipoint Bluetooth 5.3 pairing, and 60h playback.', 649900, 70, 'Audio', 'https://images.unsplash.com/photo-1505740420928-5e560c06d30e?w=600&auto=format&fit=crop&q=80'),
(19, 2, '2.1 Channel 160W Soundbar with Wireless Subwoofer', 'Dolby Audio processing, HDMI ARC & optical connectivity, dedicated equalizer presets for movies and music.', 899900, 40, 'Audio', 'https://images.unsplash.com/photo-1545454675-3531b543be5d?w=600&auto=format&fit=crop&q=80'),
(20, 2, 'Waterproof GPS Fitness Smart Watch with Heart Rate Monitor', 'Precision dual-band GPS, 120+ sport modes, all-day stress and sleep tracking, with 10-day battery life.', 499900, 85, 'Smart Watches', 'https://images.unsplash.com/photo-1579586337278-3befd40fd17a?w=600&auto=format&fit=crop&q=80'),
(21, 2, 'Bluetooth Calling Smart Watch with AI Voice Assistant', 'Built-in mic and speaker for HD calls, 1.85-inch vibrant HD display, weather updates, and music control.', 349900, 90, 'Smart Watches', 'https://images.unsplash.com/photo-1523275335684-37898b6baf30?w=600&auto=format&fit=crop&q=80'),
(22, 2, 'AMOLED Display Luxury Smart Watch with Stainless Steel Strap', '1.43-inch Always-On AMOLED screen with 1000 nits brightness, sapphire glass protection, and premium rotating crown.', 599900, 60, 'Smart Watches', 'https://images.unsplash.com/photo-1508685096489-7aacd43bd3b1?w=600&auto=format&fit=crop&q=80'),
(23, 2, 'Precision 16000 DPI RGB Optical Gaming Mouse', 'Ultra-lightweight ergonomic honeycomb shell, 6 programmable macro buttons, and PTFE smooth glide skates.', 189900, 115, 'Gaming', 'https://images.unsplash.com/photo-1615663245857-ac93bb7c39e7?w=600&auto=format&fit=crop&q=80'),
(24, 2, 'Tenkeyless RGB Mechanical Gaming Keyboard (Tactile Switches)', 'Aircraft-grade aluminum top plate, per-key RGB backlighting, full N-key rollover, and detachable USB-C braided cable.', 349900, 75, 'Gaming', 'https://images.unsplash.com/photo-1587829741301-dc798b83add3?w=600&auto=format&fit=crop&q=80'),
(25, 2, 'Wireless Vibration Feedback Game Controller for PC & Console', 'Hall-effect anti-drift analog triggers, dual rumble haptic motors, textured grip, and rechargeable battery.', 279900, 80, 'Gaming', 'https://images.unsplash.com/photo-1600080972464-8e5f35f63d08?w=600&auto=format&fit=crop&q=80'),
(26, 2, '7.1 Surround Sound Immersive Gaming Headset with Mic', '50mm neodymium audio drivers, memory foam breathable earcups, broadcast-quality detachable mic, and RGB lighting.', 299900, 70, 'Gaming', 'https://images.unsplash.com/photo-1546435770-a3e426bf472b?w=600&auto=format&fit=crop&q=80'),
(27, 2, '4K Mirrorless Digital Camera with 18-55mm Lens Kit', '24.2MP APS-C sensor, fast hybrid autofocus with eye tracking, 4K HDR recording, and 3-inch flip touchscreen.', 5699000, 25, 'Cameras', 'https://images.unsplash.com/photo-1516035069371-29a1b244cc32?w=600&auto=format&fit=crop&q=80'),
(28, 2, '4K 60FPS Waterproof Dual-Screen Action Camera', 'Electronic image stabilization 4.0, 10m native waterproof housing, 170-degree ultra-wide angle, and Wi-Fi transfer.', 1499900, 50, 'Cameras', 'https://images.unsplash.com/photo-1526170375885-4d8ecf77b99f?w=600&auto=format&fit=crop&q=80'),
(29, 2, '1080P Full HD Streaming Webcam with Privacy Cover', 'Auto light correction, dual stereo noise-cancelling mics, 90-degree field of view, and tripod-ready universal clip.', 229900, 90, 'Cameras', 'https://images.unsplash.com/photo-1587826080692-f439cd0b70da?w=600&auto=format&fit=crop&q=80'),
(30, 2, '100W Braided USB-C to USB-C Fast Charging Cable (2m)', 'E-Marker smart chip, 480Mbps data sync, heavy-duty nylon braided jacket with 25000+ bend lifespan.', 49900, 200, 'Accessories', 'https://images.unsplash.com/photo-1610492461129-9e8a5b28d7d9?w=600&auto=format&fit=crop&q=80'),
(31, 2, '20000mAh 22.5W Fast Charging Power Bank with LED Display', 'Triple output ports (2x USB-A, 1x USB-C PD), multi-protection safety system, and accurate digital percentage read-out.', 189900, 120, 'Accessories', 'https://images.unsplash.com/photo-1609592424364-7f15998f45f9?w=600&auto=format&fit=crop&q=80'),
(32, 2, 'Water-Resistant Padded Laptop Backpack (Fits up to 15.6 inch)', 'Shockproof velvet-lined laptop compartment, anti-theft hidden pocket, luggage strap, and USB charging port pass-through.', 149900, 85, 'Accessories', 'https://images.unsplash.com/photo-1553062407-98eeb64c6a62?w=600&auto=format&fit=crop&q=80'),
(33, 2, 'Silent 2.4GHz Ergonomic Rechargeable Wireless Mouse', '90% reduced click noise, 3 adjustable DPI levels (800/1200/1600), USB nano receiver, and auto sleep power saving.', 79900, 150, 'Accessories', 'https://images.unsplash.com/photo-1527864550417-7fd91fc51a46?w=600&auto=format&fit=crop&q=80'),
(34, 2, 'Slim Multi-Device Wireless Bluetooth Keyboard', 'Scissor-switch low-profile keys, switch seamlessly across 3 devices (laptop, tablet, phone), with long-lasting battery.', 119900, 95, 'Accessories', 'https://images.unsplash.com/photo-1587829741301-dc798b83add3?w=600&auto=format&fit=crop&q=80')
ON CONFLICT (id) DO UPDATE SET
    name = EXCLUDED.name,
    description = EXCLUDED.description,
    price_cents = EXCLUDED.price_cents,
    stock_qty = EXCLUDED.stock_qty,
    category = EXCLUDED.category,
    image_url = EXCLUDED.image_url;

-- Reset sequence to highest ID
SELECT setval('products_id_seq', (SELECT MAX(id) FROM products));
