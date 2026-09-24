-- db/seed.sql
-- Seed data for Dhivagar Mart
-- Passwords below are Argon2id hashes for:
-- Admin:   Admin@123
-- Seller:  Seller@123
-- Buyer:   Buyer@123

-- Clean existing data for reproducible seed
TRUNCATE TABLE reviews, cart_items, order_items, orders, products, users RESTART IDENTITY CASCADE;

-- Insert Seed Users
-- Passwords generated via libsodium crypto_pwhash_str (Argon2id)
INSERT INTO users (id, name, email, password_hash, role, created_at) VALUES
(1, 'System Administrator', 'admin@dhivagarmart.com', '$argon2id$v=19$m=65536,t=2,p=1$/gMIaFVSkxkVrUEcRmk4Fw$dXrXmSmKg/CLNNwhD5urIu+zrSJTQQ9KXqkCwwjplgI', 'ADMIN', CURRENT_TIMESTAMP),
(2, 'Fresh Farms Seller', 'seller@dhivagarmart.com', '$argon2id$v=19$m=65536,t=2,p=1$60XS1/dAcJXeCGW9QH49HQ$Qc+9gXEL0gw5j9/pWApfCOo09ypVuWEE/VE5Ix/h9BA', 'SELLER', CURRENT_TIMESTAMP),
(3, 'John Buyer', 'buyer@dhivagarmart.com', '$argon2id$v=19$m=65536,t=2,p=1$Ch4z8sIA7Rv4pqAbb5ncnA$sVJSho07stk6d6fWPtpA9VfBM8tvj4Td41CLva3usgA', 'BUYER', CURRENT_TIMESTAMP);

-- Reset users sequence to next value
SELECT setval('users_id_seq', (SELECT MAX(id) FROM users));

-- Insert Sample Grocery Products (seller_id = 2)
-- Prices in BIGINT cents (e.g., $4.99 = 499 cents)
INSERT INTO products (id, seller_id, name, description, price_cents, stock_qty, category, image_url, created_at) VALUES
-- Fruits
(1, 2, 'Fresh Organic Gala Apples (1 kg)', 'Crisp, sweet, and locally harvested premium organic Gala apples.', 399, 100, 'Fruits', 'https://images.unsplash.com/photo-1560806887-1e4cd0b6cbd6?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
(2, 2, 'Cavendish Bananas (Bunch of 6)', 'Naturally ripened high-energy fresh bananas, rich in potassium.', 199, 150, 'Fruits', 'https://images.unsplash.com/photo-1571771894821-ce9b6c11b08e?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
(3, 2, 'Alphonso Mangoes (Box of 4)', 'Aromatic and lusciously sweet hand-picked king of mangoes.', 899, 50, 'Fruits', 'https://images.unsplash.com/photo-1553279768-865429fa0078?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),

-- Dairy
(4, 2, 'Farm Fresh Whole Milk (1 Gallon)', 'Grade A pasteurized whole vitamin D milk from grass-fed cows.', 449, 80, 'Dairy', 'https://images.unsplash.com/photo-1550583724-b2692b85b150?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
(5, 2, 'Organic Salted Creamery Butter (500g)', 'Slow-churned European-style salted butter, perfect for baking.', 529, 60, 'Dairy', 'https://images.unsplash.com/photo-1589985270826-4b7bb135bc9d?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
(6, 2, 'Authentic Greek Yogurt Plain (907g)', 'Thick, creamy, high-protein traditional strained Greek yogurt.', 599, 75, 'Dairy', 'https://images.unsplash.com/photo-1488477181946-6428a0291777?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),

-- Rice & Grains
(7, 2, 'Royal Aged Basmati Rice (5 kg)', 'Extra long grain aromatic basmati rice, aged for 2 years.', 1499, 40, 'Rice & Grains', 'https://images.unsplash.com/photo-1586201375761-83865001e31c?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
(8, 2, 'Stoneground 100% Whole Wheat Atta (5 kg)', 'Nutritious unbleached whole wheat flour for soft rotis and breads.', 949, 50, 'Rice & Grains', 'https://images.unsplash.com/photo-1574323347407-f5e1ad6d020b?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
(9, 2, 'Organic Tri-Color Quinoa (1 kg)', 'Gluten-free nutrient-dense superfood grain packed with complete protein.', 799, 65, 'Rice & Grains', 'https://images.unsplash.com/photo-1586201375761-83865001e31c?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),

-- Vegetables
(10, 2, 'Vine-Ripened Roma Tomatoes (1 kg)', 'Plump, juicy, deep-red field grown vine tomatoes.', 299, 120, 'Vegetables', 'https://images.unsplash.com/photo-1592924357228-91a4daadcfea?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
(11, 2, 'Crisp Red Onions (2 kg Bag)', 'Sharp, vibrant red onions ideal for cooking and fresh salads.', 349, 90, 'Vegetables', 'https://images.unsplash.com/photo-1618512496248-a07fe83aa8cb?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
(12, 2, 'Organic Yukon Gold Potatoes (3 kg)', 'Smooth buttery-textured golden potatoes great for roasting or mashing.', 499, 70, 'Vegetables', 'https://images.unsplash.com/photo-1518977676601-b53f82aba655?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),

-- Snacks
(13, 2, 'Crunchy Roasted Mixed Nuts (400g)', 'Wholesome blend of roasted almonds, cashews, walnuts, and pecans.', 849, 85, 'Snacks', 'https://images.unsplash.com/photo-1599599810769-bcde5a160d32?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
(14, 2, 'Artisan Sea Salt Potato Chips (180g)', 'Kettle-cooked golden potato chips tossed with pure sea salt crystals.', 329, 110, 'Snacks', 'https://images.unsplash.com/photo-1566478989037-eec170784d0b?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),

-- Beverages
(15, 2, 'Cold-Pressed Valencia Orange Juice (1 L)', '100% freshly squeezed pure orange juice with vibrant pulp.', 499, 60, 'Beverages', 'https://images.unsplash.com/photo-1621506289937-a8e4df240d0b?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP),
(16, 2, 'Sparkling Italian Mineral Water (6x750ml)', 'Naturally carbonated pristine Italian alpine spring water.', 999, 45, 'Beverages', 'https://images.unsplash.com/photo-1548839140-29a749e1bc4e?w=600&auto=format&fit=crop&q=80', CURRENT_TIMESTAMP);

-- Reset products sequence
SELECT setval('products_id_seq', (SELECT MAX(id) FROM products));

-- Record baseline migrations in schema_migrations
INSERT INTO schema_migrations (version, name, applied_at) VALUES
(1, 'V1__init_schema.sql', CURRENT_TIMESTAMP),
(2, 'V2__add_reviews_table.sql', CURRENT_TIMESTAMP),
(3, 'V3__add_indexes.sql', CURRENT_TIMESTAMP)
ON CONFLICT (version) DO NOTHING;
