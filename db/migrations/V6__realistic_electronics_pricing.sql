-- db/migrations/V6__realistic_electronics_pricing.sql
-- Transition product catalog to realistic Indian high-value electronics retail pricing.
-- Prices are stored in integer minor units (paise: ₹1 = 100 paise).

-- 1. Mobiles
UPDATE products SET 
    name = 'Samsung Galaxy S24 Ultra 5G (512GB, Titanium Gray)',
    description = 'Flagship smartphone featuring Galaxy AI, 200MP quad camera, Snapdragon 8 Gen 3, and integrated S-Pen.',
    price_cents = 12999900, -- ₹1,29,999
    stock_qty = 45,
    category = 'Mobiles',
    image_url = 'https://images.unsplash.com/photo-1610945415295-d9bbf067e59c?w=600&auto=format&fit=crop&q=80'
WHERE id = 1;

UPDATE products SET 
    name = 'Apple iPhone 15 Pro Max (256GB, Natural Titanium)',
    description = 'Aerospace-grade titanium design, A17 Pro chip, 48MP main camera with 5x optical telephoto, and Action button.',
    price_cents = 13490000, -- ₹1,34,900
    stock_qty = 30,
    category = 'Mobiles',
    image_url = 'https://images.unsplash.com/photo-1695048133142-1a20484d2569?w=600&auto=format&fit=crop&q=80'
WHERE id = 2;

UPDATE products SET 
    name = 'OnePlus 12 5G (16GB RAM, 512GB Storage, Silky Black)',
    description = '4th Gen Hasselblad Camera System, 5400mAh battery with 100W SUPERVOOC charging, and 2K 120Hz ProXDR display.',
    price_cents = 6499900, -- ₹64,999
    stock_qty = 50,
    category = 'Mobiles',
    image_url = 'https://images.unsplash.com/photo-1598327105666-5b89351aff97?w=600&auto=format&fit=crop&q=80'
WHERE id = 3;

UPDATE products SET 
    name = 'Redmi Note 13 Pro+ 5G (Fusion Purple, 256GB)',
    description = '200MP ultra-clear camera with OIS, 120Hz 3D curved AMOLED display, and 120W HyperCharge.',
    price_cents = 2999900, -- ₹29,999
    stock_qty = 75,
    category = 'Mobiles',
    image_url = 'https://images.unsplash.com/photo-1511707171634-5f897ff02aa9?w=600&auto=format&fit=crop&q=80'
WHERE id = 4;

UPDATE products SET 
    name = 'Realme 12 Pro+ 5G (Submarine Blue, 256GB)',
    description = 'Periscope portrait camera, luxury watch design by Ollivier Saveo, and 120Hz curved vision display.',
    price_cents = 2499900, -- ₹24,999
    stock_qty = 60,
    category = 'Mobiles',
    image_url = 'https://images.unsplash.com/photo-1565849904461-04a58ad377e0?w=600&auto=format&fit=crop&q=80'
WHERE id = 5;

-- 2. Laptops
UPDATE products SET 
    name = 'Dell Inspiron 15 Laptop (Intel Core i7 13th Gen, 16GB, 512GB SSD)',
    description = 'High-productivity everyday laptop with 15.6-inch FHD 120Hz display, backlit keyboard, and Windows 11.',
    price_cents = 6899000, -- ₹68,990
    stock_qty = 35,
    category = 'Laptops',
    image_url = 'https://images.unsplash.com/photo-1588872657578-7efd1f1555ed?w=600&auto=format&fit=crop&q=80'
WHERE id = 6;

UPDATE products SET 
    name = 'HP Pavilion 15 Laptop (AMD Ryzen 7, 16GB RAM, 1TB SSD)',
    description = 'Sleek aluminum body laptop featuring Audio by B&O, micro-edge IPS anti-glare display, and fast charging.',
    price_cents = 5499900, -- ₹54,999
    stock_qty = 40,
    category = 'Laptops',
    image_url = 'https://images.unsplash.com/photo-1544244015-0df4b3ffc6b0?w=600&auto=format&fit=crop&q=80'
WHERE id = 7;

UPDATE products SET 
    name = 'Lenovo IdeaPad Slim 3 (Intel Core i5 12th Gen, 16GB, FHD IPS)',
    description = 'Thin and light laptop with military-grade durability, privacy shutter webcam, and rapid charge boost.',
    price_cents = 4999900, -- ₹49,999
    stock_qty = 50,
    category = 'Laptops',
    image_url = 'https://images.unsplash.com/photo-1603302576837-37561b2e2302?w=600&auto=format&fit=crop&q=80'
WHERE id = 8;

UPDATE products SET 
    name = 'ASUS Vivobook 16X (Intel Core i5, RTX 3050 4GB, 16GB RAM)',
    description = 'Creator and gaming laptop with 16-inch 120Hz display, ErgoSense keyboard, and IceCool thermal technology.',
    price_cents = 6499900, -- ₹64,999
    stock_qty = 30,
    category = 'Laptops',
    image_url = 'https://images.unsplash.com/photo-1496181133206-80ce9b88a853?w=600&auto=format&fit=crop&q=80'
WHERE id = 9;

UPDATE products SET 
    name = 'Acer Aspire 5 Laptop (13th Gen Intel Core i5, 16GB RAM, 512GB SSD)',
    description = 'Versatile multi-tasking laptop with elevated hinge design for cooling, Wi-Fi 6E, and dual stereo speakers.',
    price_cents = 5199000, -- ₹51,990
    stock_qty = 45,
    category = 'Laptops',
    image_url = 'https://images.unsplash.com/photo-1517336714731-489689fd1ca8?w=600&auto=format&fit=crop&q=80'
WHERE id = 10;

-- 3. Electrical / Home Appliances
UPDATE products SET 
    name = '55-inch 4K Ultra HD Smart LED TV (Dolby Vision & Atmos)',
    description = 'Bezel-less cinema display with Google TV OS, MEMC motion smoothing, and 30W high-fidelity speakers.',
    price_cents = 4299900, -- ₹42,999
    stock_qty = 25,
    category = 'Electrical',
    image_url = 'https://images.unsplash.com/photo-1593359677879-a4bb92f829d1?w=600&auto=format&fit=crop&q=80'
WHERE id = 11;

UPDATE products SET 
    name = 'Stainless Steel 1.8L Fast-Boil Electric Kettle',
    description = '1500W rapid boiling element with auto shut-off, boil-dry protection, and 360-degree swivel base.',
    price_cents = 149900, -- ₹1,499
    stock_qty = 120,
    category = 'Electrical',
    image_url = 'https://images.unsplash.com/photo-1570222094114-d054a817e56b?w=600&auto=format&fit=crop&q=80'
WHERE id = 12;

UPDATE products SET 
    name = 'Heavy-Duty 750W 4-Jar Kitchen Mixer Grinder',
    description = '100% copper motor with overload protection, stainless steel blades, and dedicated juicer extractor jar.',
    price_cents = 349900, -- ₹3,499
    stock_qty = 80,
    category = 'Electrical',
    image_url = 'https://images.unsplash.com/photo-1584269600464-37b1b58a9fe7?w=600&auto=format&fit=crop&q=80'
WHERE id = 13;

UPDATE products SET 
    name = 'High-Speed Silent Aerodynamic 1200mm Ceiling Fan',
    description = 'Energy-efficient BLDC motor with RF remote control, sleep timer, and anti-dust coating.',
    price_cents = 249900, -- ₹2,499
    stock_qty = 90,
    category = 'Electrical',
    image_url = 'https://images.unsplash.com/photo-1585338107529-13afc5f02586?w=600&auto=format&fit=crop&q=80'
WHERE id = 14;

UPDATE products SET 
    name = '1.5 Ton 5-Star Inverter Split Air Conditioner with Wi-Fi',
    description = 'Dual inverter compressor, 4-in-1 convertible cooling, PM 2.5 air purification filter, and voice control.',
    price_cents = 3899000, -- ₹38,990
    stock_qty = 20,
    category = 'Electrical',
    image_url = 'https://images.unsplash.com/photo-1621905251189-08b45d6a269e?w=600&auto=format&fit=crop&q=80'
WHERE id = 15;

-- 4. Audio
UPDATE products SET 
    name = 'Active Noise Cancelling True Wireless Earbuds (ANC 45dB)',
    description = 'High-res spatial audio, dual transparency mode, 40 hours total battery life with wireless charging case.',
    price_cents = 449900, -- ₹4,499
    stock_qty = 110,
    category = 'Audio',
    image_url = 'https://images.unsplash.com/photo-1590658268037-6bf12165a8df?w=600&auto=format&fit=crop&q=80'
WHERE id = 16;

UPDATE products SET 
    name = 'Waterproof 20W Portable Bluetooth Speaker with Bass Boost',
    description = 'IPX7 waterproof rating, 360-degree punchy stereo sound, RGB beat lighting, and 16-hour playtime.',
    price_cents = 299900, -- ₹2,999
    stock_qty = 95,
    category = 'Audio',
    image_url = 'https://images.unsplash.com/photo-1608043152269-423dbba4e7e1?w=600&auto=format&fit=crop&q=80'
WHERE id = 17;

UPDATE products SET 
    name = 'Sony WH-1000XM5 Wireless Noise Cancelling Headphones',
    description = 'Industry leading noise cancellation, Auto NC Optimizer, 30-hour battery life, and crystal clear hands-free calling.',
    price_cents = 2499900, -- ₹24,999
    stock_qty = 70,
    category = 'Audio',
    image_url = 'https://images.unsplash.com/photo-1505740420928-5e560c06d30e?w=600&auto=format&fit=crop&q=80'
WHERE id = 18;

UPDATE products SET 
    name = '2.1 Channel 160W Soundbar with Wireless Subwoofer',
    description = 'Dolby Audio processing, HDMI ARC & optical connectivity, dedicated equalizer presets for movies and music.',
    price_cents = 899900, -- ₹8,999
    stock_qty = 40,
    category = 'Audio',
    image_url = 'https://images.unsplash.com/photo-1545454675-3531b543be5d?w=600&auto=format&fit=crop&q=80'
WHERE id = 19;

-- 5. Smart Watches
UPDATE products SET 
    name = 'Waterproof GPS Fitness Smart Watch with Heart Rate Monitor',
    description = 'Precision dual-band GPS, 120+ sport modes, all-day stress and sleep tracking, with 10-day battery life.',
    price_cents = 499900, -- ₹4,999
    stock_qty = 85,
    category = 'Smart Watches',
    image_url = 'https://images.unsplash.com/photo-1579586337278-3befd40fd17a?w=600&auto=format&fit=crop&q=80'
WHERE id = 20;

UPDATE products SET 
    name = 'Bluetooth Calling Smart Watch with AI Voice Assistant',
    description = 'Built-in mic and speaker for HD calls, 1.85-inch vibrant HD display, weather updates, and music control.',
    price_cents = 299900, -- ₹2,999
    stock_qty = 90,
    category = 'Smart Watches',
    image_url = 'https://images.unsplash.com/photo-1523275335684-37898b6baf30?w=600&auto=format&fit=crop&q=80'
WHERE id = 21;

UPDATE products SET 
    name = 'AMOLED Display Luxury Smart Watch with Stainless Steel Strap',
    description = '1.43-inch Always-On AMOLED screen with 1000 nits brightness, sapphire glass protection, and premium rotating crown.',
    price_cents = 799900, -- ₹7,999
    stock_qty = 60,
    category = 'Smart Watches',
    image_url = 'https://images.unsplash.com/photo-1508685096489-7aacd43bd3b1?w=600&auto=format&fit=crop&q=80'
WHERE id = 22;

-- 6. Gaming
UPDATE products SET 
    name = 'Precision 16000 DPI RGB Optical Gaming Mouse',
    description = 'Ultra-lightweight ergonomic honeycomb shell, 6 programmable macro buttons, and PTFE smooth glide skates.',
    price_cents = 189900, -- ₹1,899
    stock_qty = 115,
    category = 'Gaming',
    image_url = 'https://images.unsplash.com/photo-1615663245857-ac93bb7c39e7?w=600&auto=format&fit=crop&q=80'
WHERE id = 23;

UPDATE products SET 
    name = 'Tenkeyless RGB Mechanical Gaming Keyboard (Tactile Switches)',
    description = 'Aircraft-grade aluminum top plate, per-key RGB backlighting, full N-key rollover, and detachable USB-C braided cable.',
    price_cents = 349900, -- ₹3,499
    stock_qty = 75,
    category = 'Gaming',
    image_url = 'https://images.unsplash.com/photo-1587829741301-dc798b83add3?w=600&auto=format&fit=crop&q=80'
WHERE id = 24;

UPDATE products SET 
    name = 'Wireless Vibration Feedback Game Controller for PC & Console',
    description = 'Hall-effect anti-drift analog triggers, dual rumble haptic motors, textured grip, and rechargeable battery.',
    price_cents = 279900, -- ₹2,799
    stock_qty = 80,
    category = 'Gaming',
    image_url = 'https://images.unsplash.com/photo-1600080972464-8e5f35f63d08?w=600&auto=format&fit=crop&q=80'
WHERE id = 25;

UPDATE products SET 
    name = '7.1 Surround Sound Immersive Gaming Headset with Mic',
    description = '50mm neodymium audio drivers, memory foam breathable earcups, broadcast-quality detachable mic, and RGB lighting.',
    price_cents = 299900, -- ₹2,999
    stock_qty = 70,
    category = 'Gaming',
    image_url = 'https://images.unsplash.com/photo-1546435770-a3e426bf472b?w=600&auto=format&fit=crop&q=80'
WHERE id = 26;

-- 7. Cameras
UPDATE products SET 
    name = 'Sony Alpha ILCE-6400L 24.2MP Mirrorless Digital Camera (16-50mm Lens)',
    description = '24.2MP APS-C sensor, world-fastest 0.02s AF with Real-Time Eye AF & Tracking, 4K HDR video, and 180-degree tiltable LCD.',
    price_cents = 7999000, -- ₹79,990
    stock_qty = 25,
    category = 'Cameras',
    image_url = 'https://images.unsplash.com/photo-1516035069371-29a1b244cc32?w=600&auto=format&fit=crop&q=80'
WHERE id = 27;

UPDATE products SET 
    name = '4K 60FPS Waterproof Dual-Screen Action Camera',
    description = 'Electronic image stabilization 4.0, 10m native waterproof housing, 170-degree ultra-wide angle, and Wi-Fi transfer.',
    price_cents = 1699900, -- ₹16,999
    stock_qty = 50,
    category = 'Cameras',
    image_url = 'https://images.unsplash.com/photo-1526170375885-4d8ecf77b99f?w=600&auto=format&fit=crop&q=80'
WHERE id = 28;

UPDATE products SET 
    name = '1080P Full HD Streaming Webcam with Privacy Cover',
    description = 'Auto light correction, dual stereo noise-cancelling mics, 90-degree field of view, and tripod-ready universal clip.',
    price_cents = 249900, -- ₹2,499
    stock_qty = 90,
    category = 'Cameras',
    image_url = 'https://images.unsplash.com/photo-1587826080692-f439cd0b70da?w=600&auto=format&fit=crop&q=80'
WHERE id = 29;

-- 8. Accessories
UPDATE products SET 
    name = '100W Braided USB-C to USB-C Fast Charging Cable (2m)',
    description = 'E-Marker smart chip, 480Mbps data sync, heavy-duty nylon braided jacket with 25000+ bend lifespan.',
    price_cents = 49900, -- ₹499
    stock_qty = 200,
    category = 'Accessories',
    image_url = 'https://images.unsplash.com/photo-1610492461129-9e8a5b28d7d9?w=600&auto=format&fit=crop&q=80'
WHERE id = 30;

UPDATE products SET 
    name = '20000mAh 22.5W Fast Charging Power Bank with LED Display',
    description = 'Triple output ports (2x USB-A, 1x USB-C PD), multi-protection safety system, and accurate digital percentage read-out.',
    price_cents = 189900, -- ₹1,899
    stock_qty = 120,
    category = 'Accessories',
    image_url = 'https://images.unsplash.com/photo-1609592424364-7f15998f45f9?w=600&auto=format&fit=crop&q=80'
WHERE id = 31;

UPDATE products SET 
    name = 'Water-Resistant Padded Laptop Backpack (Fits up to 15.6 inch)',
    description = 'Shockproof velvet-lined laptop compartment, anti-theft hidden pocket, luggage strap, and USB charging port pass-through.',
    price_cents = 149900, -- ₹1,499
    stock_qty = 85,
    category = 'Accessories',
    image_url = 'https://images.unsplash.com/photo-1553062407-98eeb64c6a62?w=600&auto=format&fit=crop&q=80'
WHERE id = 32;

UPDATE products SET 
    name = 'Silent 2.4GHz Ergonomic Rechargeable Wireless Mouse',
    description = '90% reduced click noise, 3 adjustable DPI levels (800/1200/1600), USB nano receiver, and auto sleep power saving.',
    price_cents = 79900, -- ₹799
    stock_qty = 150,
    category = 'Accessories',
    image_url = 'https://images.unsplash.com/photo-1527864550417-7fd91fc51a46?w=600&auto=format&fit=crop&q=80'
WHERE id = 33;

UPDATE products SET 
    name = 'Slim Multi-Device Wireless Bluetooth Keyboard',
    description = 'Scissor-switch low-profile keys, switch seamlessly across 3 devices (laptop, tablet, phone), with long-lasting battery.',
    price_cents = 129900, -- ₹1,299
    stock_qty = 95,
    category = 'Accessories',
    image_url = 'https://images.unsplash.com/photo-1587829741301-dc798b83add3?w=600&auto=format&fit=crop&q=80'
WHERE id = 34;

-- Additional Market Products for Complete Price Spectrum Coverage
INSERT INTO products (id, seller_id, name, description, price_cents, stock_qty, category, image_url, created_at) VALUES
(35, 2, 'Redmi 13C 5G (Starshine Green, 128GB)', 'MediaTek Dimensity 6100+ 5G processor, 50MP AI dual camera, and 5000mAh battery.', 1099900, 80, 'Mobiles', 'https://images.unsplash.com/photo-1511707171634-5f897ff02aa9?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
(36, 2, 'Apple iPhone 14 (128GB, Midnight)', '6.1-inch Super Retina XDR display, advanced dual-camera system, cinematic mode in 4K Dolby Vision.', 5999900, 35, 'Mobiles', 'https://images.unsplash.com/photo-1695048133142-1a20484d2569?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
(37, 2, 'ASUS ROG Strix G16 Gaming Laptop (Intel i7 13th Gen, RTX 4060, 16GB, 1TB SSD)', '16-inch FHD+ 165Hz ROG Nebula display, Tri-Fan thermal cooling, and Aura Sync per-key RGB.', 11999900, 20, 'Laptops', 'https://images.unsplash.com/photo-1603302576837-37561b2e2302?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
(38, 2, 'Apple MacBook Pro 16-inch (M3 Pro chip, 18GB Unified Memory, 512GB SSD)', 'Extreme dynamic range Liquid Retina XDR display, 22 hours battery life, ProRes hardware acceleration.', 19999900, 15, 'Laptops', 'https://images.unsplash.com/photo-1517336714731-489689fd1ca8?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
(39, 2, 'Samsung 253L 3-Star Inverter Frost Free Double Door Refrigerator', 'Digital inverter technology, all-around cooling, toughened glass shelves, and stabilizer free operation.', 2699000, 25, 'Electrical', 'https://images.unsplash.com/photo-1584269600464-37b1b58a9fe7?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
(40, 2, 'LG 7.0 Kg 5-Star Smart Inverter Fully-Automatic Front Load Washing Machine', '6 Motion Direct Drive for optimal fabric wash, Steam allergy care, and touch panel control.', 3199000, 20, 'Electrical', 'https://images.unsplash.com/photo-1621905251189-08b45d6a269e?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
(41, 2, 'boAt Airdopes 141 True Wireless Earbuds (42H Playtime, ENx Tech)', '8mm drivers, ASAP charge (5 mins = 75 mins), IPX4 water resistance, and low-latency BEAST mode.', 129900, 150, 'Audio', 'https://images.unsplash.com/photo-1590658268037-6bf12165a8df?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
(42, 2, 'Apple Watch Series 9 GPS 45mm (Midnight Aluminum Case)', 'S9 SiP with Double Tap gesture, brighter Always-On display, precision finding for iPhone, and ECG app.', 4490000, 25, 'Smart Watches', 'https://images.unsplash.com/photo-1508685096489-7aacd43bd3b1?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
(43, 2, 'LG UltraGear 27-inch QHD 165Hz IPS Gaming Monitor (1ms, HDR10)', 'Nano IPS technology, NVIDIA G-SYNC compatible, AMD FreeSync Premium, and borderless design.', 2199900, 30, 'Gaming', 'https://images.unsplash.com/photo-1546435770-a3e426bf472b?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
(44, 2, 'Canon EOS R6 Mark II Full-Frame Professional Mirrorless Camera (Body Only)', '24.2MP full-frame CMOS sensor, up to 40 fps electronic shutter, 6K oversampled 4K 60p, Dual Pixel CMOS AF II.', 21599900, 10, 'Cameras', 'https://images.unsplash.com/photo-1516035069371-29a1b244cc32?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP)
ON CONFLICT (id) DO UPDATE SET
    name = EXCLUDED.name,
    description = EXCLUDED.description,
    price_cents = EXCLUDED.price_cents,
    stock_qty = EXCLUDED.stock_qty,
    category = EXCLUDED.category,
    image_url = EXCLUDED.image_url;

-- Update products sequence
SELECT setval('products_id_seq', (SELECT MAX(id) FROM products));
