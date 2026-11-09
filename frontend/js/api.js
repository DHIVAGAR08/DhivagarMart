// Dhivagar Mart - Premium Electronics Marketplace API Client & Store
const API_BASE = '/api/v1';

// Universal Indian Rupee Currency Formatter
const inrFormatter = new Intl.NumberFormat("en-IN", {
  style: "currency",
  currency: "INR",
  maximumFractionDigits: 0
});

function formatINR(cents) {
  if (cents === null || cents === undefined || isNaN(cents)) return '₹0';
  const rupees = cents / 100;
  if (cents % 100 === 0) {
    return inrFormatter.format(rupees);
  }
  return new Intl.NumberFormat("en-IN", {
    style: "currency",
    currency: "INR",
    maximumFractionDigits: 2
  }).format(rupees);
}

// Canonical Premium Electronics Catalog
const ELECTRONICS_PRODUCTS = [
  {
    id: 1,
    name: 'Apple iPhone 17 Pro',
    price_cents: 13490000,
    original_price_cents: 14490000,
    category: 'Apple iPhones',
    sub_category: 'Smartphones',
    badge: 'Amazon\'s Choice',
    stock_qty: 24,
    rating: 4.9,
    review_count: 1420,
    image_url: 'https://images.unsplash.com/photo-1695048133142-1a20484d2569?w=700&auto=format&fit=crop&q=80',
    description: 'Flagship Apple iPhone with next-generation A19 Pro 3nm silicon, 6.3-inch Super Retina XDR ProMotion 120Hz display, 48MP Pro triple-camera system, Grade 5 Titanium body, and USB-C 10Gbps transfer speed.',
    specs: {
      'Brand': 'Apple',
      'Processor': 'A19 Pro Chip with 6-core GPU',
      'Display': '6.3-inch Super Retina XDR OLED (120Hz ProMotion)',
      'Storage': '256GB NVMe',
      'Camera': '48MP Main + 48MP Ultra Wide + 12MP 5x Telephoto',
      'Build': 'Grade 5 Titanium & Ceramic Shield Front',
      'Battery': 'Up to 29 hours video playback, MagSafe wireless',
      'Warranty': '1 Year Apple Official Limited Warranty'
    }
  },
  {
    id: 2,
    name: 'Apple iPhone 17',
    price_cents: 7990000,
    original_price_cents: 8990000,
    category: 'Apple iPhones',
    sub_category: 'Smartphones',
    badge: 'Bestseller',
    stock_qty: 35,
    rating: 4.8,
    review_count: 2150,
    image_url: 'https://images.unsplash.com/photo-1510557880182-3d4d3cba35a5?w=700&auto=format&fit=crop&q=80',
    description: 'High-performance Apple iPhone powered by A18 Bionic chip, 6.1-inch Dynamic Island OLED display, dual 48MP Fusion camera system with 2x Telephoto, Aerospace-grade aluminum finish, and all-day battery life.',
    specs: {
      'Brand': 'Apple',
      'Processor': 'A18 Bionic Chip',
      'Display': '6.1-inch Super Retina XDR OLED with Dynamic Island',
      'Storage': '128GB NVMe',
      'Camera': '48MP Main Fusion Camera with 2x Optical Telephoto',
      'Build': 'Color-infused Glass & Aerospace Aluminum',
      'Battery': 'Up to 22 hours video playback, Fast Charging',
      'Warranty': '1 Year Apple Official Limited Warranty'
    }
  },
  {
    id: 3,
    name: 'Samsung Galaxy S25 Ultra',
    price_cents: 12999900,
    original_price_cents: 13999900,
    category: 'Smartphones',
    sub_category: 'Smartphones',
    badge: 'Top Rated',
    stock_qty: 18,
    rating: 4.9,
    review_count: 980,
    image_url: 'https://images.unsplash.com/photo-1610945415295-d9bbf067e59c?w=700&auto=format&fit=crop&q=80',
    description: 'Ultra-premium Android smartphone featuring Qualcomm Snapdragon 8 Elite, 6.8-inch Dynamic AMOLED 2X 120Hz flat display, 200MP Quad Camera with 100x Space Zoom, integrated S-Pen, and 5000mAh battery.',
    specs: {
      'Brand': 'Samsung',
      'Processor': 'Snapdragon 8 Elite Mobile Platform for Galaxy',
      'Display': '6.8-inch QHD+ Dynamic AMOLED 2X (1-120Hz)',
      'Storage': '256GB UFS 4.0 / 12GB RAM',
      'Camera': '200MP Main + 50MP 5x Telephoto + 50MP Ultra Wide',
      'Stylus': 'Embedded Low-latency S-Pen with Air Actions',
      'Battery': '5000mAh with 45W Super Fast Charging 2.0',
      'Warranty': '1 Year Samsung India Brand Warranty'
    }
  },
  {
    id: 4,
    name: 'OnePlus 13',
    price_cents: 6499900,
    original_price_cents: 6999900,
    category: 'Smartphones',
    sub_category: 'Smartphones',
    badge: 'Trending Flagship',
    stock_qty: 40,
    rating: 4.8,
    review_count: 1120,
    image_url: 'https://images.unsplash.com/photo-1598327105666-5b89351aff97?w=700&auto=format&fit=crop&q=80',
    description: 'Next-gen flagship smartphone engineered with Snapdragon 8 Elite, 6.82-inch 2K 120Hz Oriental display, Hasselblad Camera for Mobile with 50MP Sony sensors, 6000mAh battery and 100W SUPERVOOC charging.',
    specs: {
      'Brand': 'OnePlus',
      'Processor': 'Snapdragon 8 Elite (3nm)',
      'Display': '6.82-inch 2K DisplayMate A++ ProXDR OLED (120Hz)',
      'Storage': '256GB UFS 4.0 / 16GB LPDDR5X RAM',
      'Camera': '50MP Sony LYT-808 + 50MP 3x Periscope + 50MP UW',
      'Battery': '6000mAh with 100W SUPERVOOC Wired & 50W Wireless',
      'OS': 'OxygenOS 15 based on Android 15',
      'Warranty': '1 Year OnePlus India Comprehensive Warranty'
    }
  },
  {
    id: 5,
    name: 'Apple Watch Series 11',
    price_cents: 4490000,
    original_price_cents: 4990000,
    category: 'Apple Watches',
    sub_category: 'Smart Watches',
    badge: 'New Launch',
    stock_qty: 28,
    rating: 4.9,
    review_count: 830,
    image_url: 'https://images.unsplash.com/photo-1546868871-7041f2a55e12?w=700&auto=format&fit=crop&q=80',
    description: 'Thinner, lighter smartwatch with expanded Always-On Retina OLED display, S10 SiP for on-device Siri, advanced health sensors with sleep apnea detection, water depth gauge, and fast charging up to 80% in 30 minutes.',
    specs: {
      'Brand': 'Apple',
      'Case Size': '45mm Midnight Aluminum',
      'Processor': 'Apple S10 SiP with 4-core Neural Engine',
      'Display': 'Wide-angle Always-On OLED with up to 2000 nits',
      'Sensors': 'Electrical Heart (ECG), Blood Oxygen, Sleep Apnea, Depth Gauge',
      'Water Resistance': '50m Swimproof & High-speed Water Sports',
      'Battery': 'All-day 18 hours (36h in Low Power Mode)',
      'Warranty': '1 Year Apple Official Warranty'
    }
  },
  {
    id: 6,
    name: 'Apple Watch Ultra 3',
    price_cents: 8990000,
    original_price_cents: 9490000,
    category: 'Apple Watches',
    sub_category: 'Smart Watches',
    badge: 'Rugged Flagship',
    stock_qty: 15,
    rating: 5.0,
    review_count: 520,
    image_url: 'https://images.unsplash.com/photo-1523275335684-37898b6baf30?w=700&auto=format&fit=crop&q=80',
    description: 'The ultimate sports and adventure watch crafted from aerospace Grade 5 titanium with 3000 nits sapphire crystal screen, dual-frequency precision GPS, siren, dive computer capability, and up to 72 hours of battery life.',
    specs: {
      'Brand': 'Apple',
      'Case Size': '49mm Grade 5 Titanium',
      'Display': 'Flat Sapphire Crystal with 3000 nits Peak Brightness',
      'Sensors': 'Dual-Frequency L1/L5 GPS, Depth Gauge, Water Temperature, ECG',
      'Durability': 'MIL-STD 810H Certified, 100m Water Resistant, 40m Dive Rated',
      'Action Button': 'Customizable international orange tactical button',
      'Battery': 'Up to 36 hours regular use / 72 hours Low Power Mode',
      'Warranty': '1 Year Apple Official Warranty'
    }
  },
  {
    id: 7,
    name: 'Samsung Galaxy Watch',
    price_cents: 2799900,
    original_price_cents: 3299900,
    category: 'Android Watches',
    sub_category: 'Smart Watches',
    badge: 'Popular Wearable',
    stock_qty: 30,
    rating: 4.7,
    review_count: 760,
    image_url: 'https://images.unsplash.com/photo-1579586337278-3befd40fd17a?w=700&auto=format&fit=crop&q=80',
    description: 'Advanced Android smartwatch featuring BioActive Sensor, AI-powered Energy Score, personalized heart-rate zones, Sapphire Crystal glass, and Wear OS Powered by Samsung for seamless Google and Galaxy app integration.',
    specs: {
      'Brand': 'Samsung',
      'Case Size': '44mm Armor Aluminum with Sapphire Crystal',
      'OS': 'Wear OS Powered by Samsung (One UI 6 Watch)',
      'Sensors': 'BioActive (Optical Heart + Electrical Heart + BIA Sensor), Skin Temp',
      'Connectivity': 'Bluetooth 5.3, Dual-band Wi-Fi, NFC, GPS',
      'Water Resistance': '5ATM + IP68 Water & Dust Resistance',
      'Battery': 'Up to 40 hours with WPC-based Wireless Fast Charging',
      'Warranty': '1 Year Samsung India Brand Warranty'
    }
  },
  {
    id: 8,
    name: 'boAt Airdopes',
    price_cents: 149900,
    original_price_cents: 399000,
    category: 'Wireless Earbuds',
    sub_category: 'Headphones',
    badge: 'Mega Deal',
    stock_qty: 85,
    rating: 4.6,
    review_count: 5400,
    image_url: 'https://images.unsplash.com/photo-1590658268037-6bf12165a8df?w=700&auto=format&fit=crop&q=80',
    description: 'True wireless stereo earbuds with 42 hours massive playback, ENx Environmental Noise Cancellation with Quad Microphones, BEAST Mode low latency for gaming, 13mm dynamic drivers, and ASAP Fast Charge.',
    specs: {
      'Brand': 'boAt',
      'Driver Size': '13mm Signature Audio Drivers',
      'Battery Life': '42 Hours Total (up to 8.5 hours per bud)',
      'Fast Charge': 'ASAP Charge (10 mins charge = 100 mins playtime)',
      'Microphone': 'Quad Mics with ENx Environmental Noise Cancellation',
      'Latency': 'BEAST Mode with 50ms Ultra-low Latency',
      'Water Resistance': 'IPX5 Sweat and Water Resistance',
      'Warranty': '1 Year boAt India Replacement Warranty'
    }
  },
  {
    id: 9,
    name: 'AirPods Pro',
    price_cents: 2490000,
    original_price_cents: 2690000,
    category: 'Wireless Earbuds',
    sub_category: 'Headphones',
    badge: 'Editor\'s Choice',
    stock_qty: 50,
    rating: 4.9,
    review_count: 3890,
    image_url: 'https://images.unsplash.com/photo-1600294037681-c80b4cb5b434?w=700&auto=format&fit=crop&q=80',
    description: 'Flagship Apple noise-cancelling wireless earbuds featuring custom H2 chip, up to 2x more Active Noise Cancellation, Adaptive Audio, Personalized Spatial Audio with dynamic head tracking, and USB-C MagSafe charging case.',
    specs: {
      'Brand': 'Apple',
      'Processor': 'Apple H2 Headphone Chip, Apple U1 in MagSafe Case',
      'Noise Control': 'Active Noise Cancellation, Transparency Mode, Adaptive Audio',
      'Audio Tech': 'Personalized Spatial Audio with dynamic head tracking',
      'Battery Life': 'Up to 6 hours listening (up to 30 hours with USB-C case)',
      'Water Resistance': 'IP54 dust, sweat, and water resistant for buds and case',
      'Charging': 'USB-C, MagSafe, Apple Watch charger, or Qi-certified',
      'Warranty': '1 Year Apple Official Limited Warranty'
    }
  },
  {
    id: 10,
    name: 'Sony WH-1000XM5',
    price_cents: 2899000,
    original_price_cents: 3499000,
    category: 'Headphones',
    sub_category: 'Headphones',
    badge: 'Audiophile Choice',
    stock_qty: 22,
    rating: 4.9,
    review_count: 1820,
    image_url: 'https://images.unsplash.com/photo-1505740420928-5e560c06d30e?w=700&auto=format&fit=crop&q=80',
    description: 'Industry-leading over-ear noise-cancelling headphones featuring two processors and eight microphones for unprecedented ANC, precision voice pickup technology, LDAC High-Resolution Audio, and 30-hour battery life.',
    specs: {
      'Brand': 'Sony',
      'Processor': 'Integrated Processor V1 + HD Noise Cancelling Processor QN1',
      'Microphones': '8 Microphones with Auto NC Optimizer',
      'Battery Life': '30 Hours with ANC On (3 min quick charge = 3 hours playback)',
      'Bluetooth': 'Version 5.2 with Multipoint Connection (pair 2 devices)',
      'Codec Support': 'LDAC, AAC, SBC (Hi-Res Audio Wireless Certified)',
      'Comfort': 'Soft fit leather with lightweight stepless slider',
      'Warranty': '1 Year Sony India Brand Warranty'
    }
  },
  {
    id: 11,
    name: 'Anker 65W GaN Fast Charger',
    price_cents: 299900,
    original_price_cents: 449900,
    category: 'Accessories',
    sub_category: 'Accessories',
    badge: 'Essential Gear',
    stock_qty: 65,
    rating: 4.8,
    review_count: 1400,
    image_url: 'https://images.unsplash.com/photo-1583863788434-e58a36330cf0?w=700&auto=format&fit=crop&q=80',
    description: 'Compact 3-port GaNPrime wall charger delivering up to 65W ultra-fast charging. Power your MacBook, iPhone, Galaxy, and AirPods simultaneously with PowerIQ 4.0 dynamic power distribution and ActiveShield 2.0 temperature monitoring.',
    specs: {
      'Brand': 'Anker',
      'Total Output': '65W Max GaNPrime Technology',
      'Ports': '2x USB-C Power Delivery 3.0, 1x USB-A PowerIQ',
      'Compatibility': 'MacBook Pro/Air, iPhone 17/16/15, Samsung Galaxy, iPad, Laptops',
      'Safety': 'ActiveShield 2.0 Real-time temperature monitoring',
      'Dimensions': '53% smaller than standard 67W laptop chargers',
      'Warranty': '18 Months Anker Official Warranty'
    }
  },
  {
    id: 12,
    name: 'MagSafe Leather Wallet & Stand',
    price_cents: 189900,
    original_price_cents: 299900,
    category: 'Accessories',
    sub_category: 'Accessories',
    badge: 'Popular Accessory',
    stock_qty: 70,
    rating: 4.7,
    review_count: 940,
    image_url: 'https://images.unsplash.com/photo-1622434641406-a158123450f9?w=700&auto=format&fit=crop&q=80',
    description: 'Premium vegan leather magnetic wallet designed for iPhone with ultra-strong built-in N52 neodymium magnets, RFID blocking card protection, 3-card capacity, and multi-angle folding kickstand for video calls and streaming.',
    specs: {
      'Material': 'Premium Eco-Friendly Vegan Leather',
      'Magnets': 'Ultra-strong N52 Neodymium Magnets (1,500g holding force)',
      'Capacity': 'Safely holds up to 3 credit/ID cards with easy thumb notch access',
      'Kickstand': 'Adjustable portrait and landscape viewing angles (15° to 160°)',
      'Security': 'Built-in RFID/NFC shielding against unauthorized wireless scanning',
      'Compatibility': 'iPhone 12 through iPhone 17 series & MagSafe cases',
      'Warranty': '1 Year Replacement Warranty'
    }
  }
];

// Helper: localStorage keys
const STORAGE_KEYS = {
  CART: 'dhivagarmart_cart',
  USER: 'dhivagarmart_user',
  WISHLIST: 'dhivagarmart_wishlist',
  ORDERS: 'dhivagarmart_orders',
  REVIEWS: 'dhivagarmart_reviews'
};

function getStorageJSON(key, defaultVal) {
  try {
    const raw = localStorage.getItem(key);
    return raw ? JSON.parse(raw) : defaultVal;
  } catch (e) {
    return defaultVal;
  }
}

function setStorageJSON(key, val) {
  try {
    localStorage.setItem(key, JSON.stringify(val));
  } catch (e) {}
}

// Client-Side Marketplace Store
const DemoStore = {
  getProducts(category, search) {
    let list = [...ELECTRONICS_PRODUCTS];

    if (category && category !== 'All' && category !== 'All Electronics') {
      const cat = category.toLowerCase().trim();
      list = list.filter(p => {
        const pCat = (p.category || '').toLowerCase();
        const pSubCat = (p.sub_category || '').toLowerCase();

        if (cat === 'smartphones') {
          return pCat === 'smartphones' || pCat === 'apple iphones' || pSubCat === 'smartphones';
        }
        if (cat === 'apple iphones') {
          return pCat === 'apple iphones';
        }
        if (cat === 'smart watches') {
          return pCat === 'smart watches' || pCat === 'apple watches' || pCat === 'android watches' || pSubCat === 'smart watches';
        }
        if (cat === 'apple watches') {
          return pCat === 'apple watches';
        }
        if (cat === 'android watches') {
          return pCat === 'android watches';
        }
        if (cat === 'wireless earbuds') {
          return pCat === 'wireless earbuds';
        }
        if (cat === 'headphones') {
          return pCat === 'headphones' || pCat === 'wireless earbuds' || pSubCat === 'headphones';
        }
        if (cat === 'accessories') {
          return pCat === 'accessories' || pSubCat === 'accessories';
        }

        return pCat.includes(cat) || pSubCat.includes(cat) || cat.includes(pCat);
      });
    }

    if (search && search.trim()) {
      const q = search.trim().toLowerCase();
      list = list.filter(p => 
        p.name.toLowerCase().includes(q) || 
        p.description.toLowerCase().includes(q) || 
        p.category.toLowerCase().includes(q) ||
        (p.specs && Object.values(p.specs).some(v => String(v).toLowerCase().includes(q)))
      );
    }

    return Promise.resolve(list);
  },

  getProductById(id) {
    const prodId = parseInt(id, 10);
    const item = ELECTRONICS_PRODUCTS.find(p => p.id === prodId);
    if (!item) {
      return Promise.reject(new Error(`Product #${id} not found`));
    }
    return Promise.resolve(item);
  },

  getCart() {
    const cartMap = getStorageJSON(STORAGE_KEYS.CART, {});
    const items = [];
    let totalItems = 0;
    let totalCents = 0;

    for (const [prodIdStr, qty] of Object.entries(cartMap)) {
      const prodId = parseInt(prodIdStr, 10);
      const product = ELECTRONICS_PRODUCTS.find(p => p.id === prodId);
      if (product && qty > 0) {
        const subtotal = product.price_cents * qty;
        totalItems += qty;
        totalCents += subtotal;
        items.push({
          product_id: product.id,
          product_name: product.name,
          product_category: product.category,
          product_image_url: product.image_url,
          unit_price_cents: product.price_cents,
          quantity: qty,
          subtotal_cents: subtotal,
          available_stock: product.stock_qty
        });
      }
    }

    return Promise.resolve({
      items,
      total_items: totalItems,
      total_cents: totalCents
    });
  },

  addToCart(productId, quantity = 1) {
    const prodId = parseInt(productId, 10);
    const cartMap = getStorageJSON(STORAGE_KEYS.CART, {});
    const currentQty = cartMap[prodId] || 0;
    const addQty = parseInt(quantity, 10) || 1;
    cartMap[prodId] = currentQty + addQty;
    setStorageJSON(STORAGE_KEYS.CART, cartMap);
    return this.getCart();
  },

  updateCartQty(productId, quantity) {
    const prodId = parseInt(productId, 10);
    const cartMap = getStorageJSON(STORAGE_KEYS.CART, {});
    const newQty = parseInt(quantity, 10);
    if (isNaN(newQty) || newQty <= 0) {
      delete cartMap[prodId];
    } else {
      cartMap[prodId] = newQty;
    }
    setStorageJSON(STORAGE_KEYS.CART, cartMap);
    return this.getCart();
  },

  removeFromCart(productId) {
    const prodId = parseInt(productId, 10);
    const cartMap = getStorageJSON(STORAGE_KEYS.CART, {});
    delete cartMap[prodId];
    setStorageJSON(STORAGE_KEYS.CART, cartMap);
    return this.getCart();
  },

  clearCart() {
    setStorageJSON(STORAGE_KEYS.CART, {});
    return Promise.resolve();
  },

  getCurrentUser() {
    let user = getStorageJSON(STORAGE_KEYS.USER, null);
    if (!user) {
      // Default customer profile
      user = {
        id: 101,
        name: 'Dhivagar Customer',
        email: 'customer@dhivagarmart.com',
        role: 'BUYER'
      };
      setStorageJSON(STORAGE_KEYS.USER, user);
    }
    return Promise.resolve(user);
  },

  login(payload) {
    const email = payload?.email || 'customer@dhivagarmart.com';
    const name = email.split('@')[0] || 'Customer';
    const user = {
      id: Math.floor(100 + Math.random() * 900),
      name: name.charAt(0).toUpperCase() + name.slice(1),
      email: email,
      role: 'BUYER'
    };
    setStorageJSON(STORAGE_KEYS.USER, user);
    return Promise.resolve(user);
  },

  register(payload) {
    const user = {
      id: Math.floor(100 + Math.random() * 900),
      name: payload?.name || 'Customer',
      email: payload?.email || 'customer@dhivagarmart.com',
      role: payload?.role || 'BUYER'
    };
    setStorageJSON(STORAGE_KEYS.USER, user);
    return Promise.resolve(user);
  },

  logout() {
    localStorage.removeItem(STORAGE_KEYS.USER);
    return Promise.resolve({ success: true });
  },

  getWishlist() {
    const idList = getStorageJSON(STORAGE_KEYS.WISHLIST, []);
    const items = idList.map(id => {
      const prod = ELECTRONICS_PRODUCTS.find(p => p.id === id);
      if (!prod) return null;
      return {
        product_id: prod.id,
        product_name: prod.name,
        product_category: prod.category,
        product_image_url: prod.image_url,
        product_price_cents: prod.price_cents,
        product_stock_qty: prod.stock_qty
      };
    }).filter(Boolean);
    return Promise.resolve(items);
  },

  addToWishlist(productId) {
    const prodId = parseInt(productId, 10);
    const list = getStorageJSON(STORAGE_KEYS.WISHLIST, []);
    if (!list.includes(prodId)) {
      list.push(prodId);
      setStorageJSON(STORAGE_KEYS.WISHLIST, list);
    }
    return this.getWishlist();
  },

  removeFromWishlist(productId) {
    const prodId = parseInt(productId, 10);
    let list = getStorageJSON(STORAGE_KEYS.WISHLIST, []);
    list = list.filter(id => id !== prodId);
    setStorageJSON(STORAGE_KEYS.WISHLIST, list);
    return this.getWishlist();
  },

  moveWishlistToCart(productId) {
    const prodId = parseInt(productId, 10);
    this.addToCart(prodId, 1);
    this.removeFromWishlist(prodId);
    return Promise.resolve({ success: true });
  },

  checkout(payload) {
    return this.getCart().then(cart => {
      const orderId = Math.floor(1000 + Math.random() * 9000);
      const newOrder = {
        id: orderId,
        order_number: `DHV-${orderId}`,
        total_amount_cents: cart.total_cents,
        total_items: cart.total_items,
        payment_method: payload?.payment_method || 'CASH_ON_DELIVERY',
        payment_status: 'CONFIRMED',
        status: 'CONFIRMED',
        delivery_address: `${payload?.shipping_address || 'Customer Street'}, ${payload?.city || 'Chennai'} - ${payload?.pincode || '600001'}`,
        created_at: new Date().toISOString(),
        items: [...cart.items]
      };

      const orders = getStorageJSON(STORAGE_KEYS.ORDERS, []);
      orders.unshift(newOrder);
      setStorageJSON(STORAGE_KEYS.ORDERS, orders);

      this.clearCart();
      return newOrder;
    });
  },

  getOrders() {
    const orders = getStorageJSON(STORAGE_KEYS.ORDERS, []);
    return Promise.resolve(orders);
  },

  getOrderById(id) {
    const orders = getStorageJSON(STORAGE_KEYS.ORDERS, []);
    const order = orders.find(o => o.id === parseInt(id, 10));
    return Promise.resolve(order || null);
  },

  cancelOrder(id) {
    const orders = getStorageJSON(STORAGE_KEYS.ORDERS, []);
    const order = orders.find(o => o.id === parseInt(id, 10));
    if (order) {
      order.status = 'CANCELLED';
      setStorageJSON(STORAGE_KEYS.ORDERS, orders);
    }
    return Promise.resolve(order);
  },

  getProductReviews(productId) {
    const prodId = parseInt(productId, 10);
    const allReviews = getStorageJSON(STORAGE_KEYS.REVIEWS, {
      1: [
        { user_name: 'Aditya S.', rating: 5, comment: 'Incredible titanium finish and the A19 Pro camera dynamic range is unmatched. Flawless display!' },
        { user_name: 'Meghna R.', rating: 5, comment: 'Upgraded from iPhone 13. The 120Hz ProMotion and battery life are phenomenal.' }
      ],
      2: [
        { user_name: 'Rohan V.', rating: 5, comment: 'Perfect balanced iPhone. The Dynamic Island and 48MP camera take breathtaking shots.' }
      ],
      3: [
        { user_name: 'Karthik N.', rating: 5, comment: 'Best screen on any smartphone period. S-Pen precision and 200MP zoom are insane.' }
      ],
      4: [
        { user_name: 'Siddharth M.', rating: 5, comment: '100W charging fills the massive 6000mAh battery in 30 mins! OxygenOS is lightning fast.' }
      ],
      5: [
        { user_name: 'Pooja T.', rating: 5, comment: 'Sleeker and noticeably lighter on the wrist. Sleep tracking and display brightness are top notch.' }
      ],
      6: [
        { user_name: 'Varun K.', rating: 5, comment: 'Built like a tank. Survives heavy treks and open-water swimming easily. 3000 nits display is crystal clear under bright sun.' }
      ],
      7: [
        { user_name: 'Naveen G.', rating: 5, comment: 'Great pairing with Galaxy smartphone. Body composition and sleep coaching are very helpful.' }
      ],
      8: [
        { user_name: 'Arun B.', rating: 5, comment: 'Punchy bass, crystal clear vocals and battery easily lasts 5 days of commute. Great value!' }
      ],
      9: [
        { user_name: 'Divya C.', rating: 5, comment: 'The Active Noise Cancellation is like entering a quiet room. USB-C case is very convenient.' }
      ],
      10: [
        { user_name: 'Suresh L.', rating: 5, comment: 'Pure audiophile bliss. Blocks out 100% of flight engine noise and ear cushions are extremely comfortable.' }
      ],
      11: [
        { user_name: 'Gautam P.', rating: 5, comment: 'Single compact charger powers my MacBook Air and iPhone together without overheating. GaN tech is magic.' }
      ],
      12: [
        { user_name: 'Anjali D.', rating: 5, comment: 'Strong magnets snap securely to iPhone. Doubles as a kickstand during FaceTime calls. Premium leather feel.' }
      ]
    });
    return Promise.resolve(allReviews[prodId] || []);
  },

  addReview(productId, rating, comment) {
    const prodId = parseInt(productId, 10);
    const allReviews = getStorageJSON(STORAGE_KEYS.REVIEWS, {});
    if (!allReviews[prodId]) allReviews[prodId] = [];
    allReviews[prodId].unshift({
      user_name: 'Verified Buyer',
      rating: parseInt(rating, 10) || 5,
      comment: comment
    });
    setStorageJSON(STORAGE_KEYS.REVIEWS, allReviews);
    return Promise.resolve(allReviews[prodId]);
  },

  sendChatMessage(message) {
    const msg = (message || '').toLowerCase();
    let reply = "Hello! Welcome to Dhivagar Mart Electronics. How can I help you choose the best flagship smartphone, smartwatch, or audio gear today?";

    if (msg.includes('iphone') || msg.includes('apple')) {
      reply = "We offer the latest Apple iPhone 17 Pro (₹1,34,900) and iPhone 17 (₹79,900), along with Apple Watch Series 11 (₹44,900) and AirPods Pro (₹24,900). All come with 1-Year Official Apple Warranty!";
    } else if (msg.includes('samsung') || msg.includes('s25') || msg.includes('galaxy')) {
      reply = "The Samsung Galaxy S25 Ultra is available at ₹1,29,999 with 200MP Quad Camera & S-Pen. We also feature the Samsung Galaxy Watch at ₹27,999.";
    } else if (msg.includes('oneplus')) {
      reply = "The OnePlus 13 is in stock at ₹64,999 featuring Snapdragon 8 Elite, Hasselblad cameras, and 100W SUPERVOOC charging.";
    } else if (msg.includes('watch') || msg.includes('smartwatch')) {
      reply = "Explore our top smartwatches: Apple Watch Series 11 (₹44,900), Apple Watch Ultra 3 (₹89,900), and Samsung Galaxy Watch (₹27,999).";
    } else if (msg.includes('earbuds') || msg.includes('headphone') || msg.includes('audio')) {
      reply = "Check out boAt Airdopes at ₹1,499, AirPods Pro (2nd Gen) at ₹24,900, and Sony WH-1000XM5 ANC headphones at ₹28,990.";
    } else if (msg.includes('order') || msg.includes('delivery') || msg.includes('ship')) {
      reply = "All orders are eligible for Fast Express Delivery with 100% Genuine Brand Certification. Click 'Add to Cart' to proceed!";
    }

    return Promise.resolve({ reply });
  }
};

// Main API Export
const api = {
  // Auth
  register: (payload) => DemoStore.register(payload),
  login: (payload) => DemoStore.login(payload),
  logout: () => DemoStore.logout(),
  getCurrentUser: () => DemoStore.getCurrentUser(),

  // Products
  getProducts: (category, search) => DemoStore.getProducts(category, search),
  getProductById: (id) => DemoStore.getProductById(id),

  // Cart
  getCart: () => DemoStore.getCart(),
  addToCart: (productId, quantity = 1) => DemoStore.addToCart(productId, quantity),
  updateCartQty: (productId, quantity) => DemoStore.updateCartQty(productId, quantity),
  removeFromCart: (productId) => DemoStore.removeFromCart(productId),

  // Orders
  checkout: (payload) => DemoStore.checkout(payload),
  getOrders: () => DemoStore.getOrders(),
  getOrderById: (id) => DemoStore.getOrderById(id),
  cancelOrder: (id) => DemoStore.cancelOrder(id),

  // Wishlist
  getWishlist: () => DemoStore.getWishlist(),
  addToWishlist: (productId) => DemoStore.addToWishlist(productId),
  removeFromWishlist: (productId) => DemoStore.removeFromWishlist(productId),
  moveWishlistToCart: (productId) => DemoStore.moveWishlistToCart(productId),

  // Reviews
  getProductReviews: (productId) => DemoStore.getProductReviews(productId),
  addReview(productId, rating, comment) => DemoStore.addReview(productId, rating, comment),

  // AI Chat
  sendChatMessage: (message) => DemoStore.sendChatMessage(message)
};
