// Dhivagar Mart - API Client & Standalone Demo Store
// 2-Day Standalone Demo Mode: Independent of PostgreSQL/Render database

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

// 6 Canonical Demo Grocery Products
const DEMO_PRODUCTS = [
  {
    id: 1,
    name: 'Apples',
    price_cents: 12000,
    category: 'Fruits',
    stock_qty: 45,
    unit: '1 kg',
    image_url: 'https://images.unsplash.com/photo-1560806887-1e4cd0b6cbd6?w=600&auto=format&fit=crop&q=80',
    description: 'Fresh, crisp, and naturally sweet organic red apples harvested directly from orchards.'
  },
  {
    id: 2,
    name: 'Milk',
    price_cents: 6000,
    category: 'Dairy',
    stock_qty: 50,
    unit: '1 Litre',
    image_url: 'https://images.unsplash.com/photo-1550583724-b2692b85b150?w=600&auto=format&fit=crop&q=80',
    description: 'Pure, pasteurized farm-fresh whole milk rich in natural calcium and essential nutrients.'
  },
  {
    id: 3,
    name: 'Rice',
    price_cents: 7000,
    category: 'Grains',
    stock_qty: 60,
    unit: '1 kg',
    image_url: 'https://images.unsplash.com/photo-1586201375761-83865001e31c?w=600&auto=format&fit=crop&q=80',
    description: 'Premium aromatic aged Basmati rice with long, slender grains for fluffy and delicious cooking.'
  },
  {
    id: 4,
    name: 'Vegetables',
    price_cents: 8000,
    category: 'Vegetables',
    stock_qty: 40,
    unit: '1 kg Assorted',
    image_url: 'https://images.unsplash.com/photo-1592924357228-91a4daadcfea?w=600&auto=format&fit=crop&q=80',
    description: 'Farm-fresh assorted seasonal vegetables directly harvested from verified organic farmers.'
  },
  {
    id: 5,
    name: 'Bananas',
    price_cents: 5000,
    category: 'Fruits',
    stock_qty: 70,
    unit: '1 Dozen',
    image_url: 'https://images.unsplash.com/photo-1571771894821-ce9b6c11b08e?w=600&auto=format&fit=crop&q=80',
    description: 'Naturally ripened sweet Cavendish bananas rich in potassium and instant energy.'
  },
  {
    id: 6,
    name: 'Eggs',
    price_cents: 9000,
    category: 'Dairy & Eggs',
    stock_qty: 55,
    unit: 'Pack of 6',
    image_url: 'https://images.unsplash.com/photo-1582722872445-44dc5f7e3c8f?w=600&auto=format&fit=crop&q=80',
    description: 'Nutritious farm-fresh brown eggs from healthy free-range hens, rich in high-quality protein.'
  }
];

// Helper: localStorage demo storage keys
const STORAGE_KEYS = {
  CART: 'dhivagarmart_demo_cart',
  USER: 'dhivagarmart_demo_user',
  WISHLIST: 'dhivagarmart_demo_wishlist',
  ORDERS: 'dhivagarmart_demo_orders',
  REVIEWS: 'dhivagarmart_demo_reviews'
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

// Client-Side Demo Store
const DemoStore = {
  getProducts(category, search) {
    let list = [...DEMO_PRODUCTS];
    if (category && category !== 'All') {
      const catLower = category.toLowerCase();
      list = list.filter(p => {
        const pCat = p.category.toLowerCase();
        return pCat === catLower || pCat.includes(catLower) || catLower.includes(pCat);
      });
    }
    if (search && search.trim()) {
      const q = search.trim().toLowerCase();
      list = list.filter(p => 
        p.name.toLowerCase().includes(q) || 
        p.description.toLowerCase().includes(q) || 
        p.category.toLowerCase().includes(q)
      );
    }
    return Promise.resolve(list);
  },

  getProductById(id) {
    const prodId = parseInt(id, 10);
    const item = DEMO_PRODUCTS.find(p => p.id === prodId);
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
      const product = DEMO_PRODUCTS.find(p => p.id === prodId);
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
      // Default guest session so visitors can immediately browse, add to cart, and checkout
      user = {
        id: 101,
        name: 'Demo Guest',
        email: 'guest@dhivagarmart.com',
        role: 'BUYER',
        is_demo: true
      };
      setStorageJSON(STORAGE_KEYS.USER, user);
    }
    return Promise.resolve(user);
  },

  login(payload) {
    const email = payload?.email || 'guest@dhivagarmart.com';
    const name = email.split('@')[0] || 'Demo Guest';
    const user = {
      id: Math.floor(100 + Math.random() * 900),
      name: name.charAt(0).toUpperCase() + name.slice(1),
      email: email,
      role: 'BUYER',
      is_demo: true
    };
    setStorageJSON(STORAGE_KEYS.USER, user);
    return Promise.resolve(user);
  },

  register(payload) {
    const user = {
      id: Math.floor(100 + Math.random() * 900),
      name: payload?.name || 'Demo Buyer',
      email: payload?.email || 'user@dhivagarmart.com',
      role: payload?.role || 'BUYER',
      is_demo: true
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
      const prod = DEMO_PRODUCTS.find(p => p.id === id);
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
    return Promise.resolve(list);
  },

  removeFromWishlist(productId) {
    const prodId = parseInt(productId, 10);
    let list = getStorageJSON(STORAGE_KEYS.WISHLIST, []);
    list = list.filter(id => id !== prodId);
    setStorageJSON(STORAGE_KEYS.WISHLIST, list);
    return Promise.resolve(list);
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
        payment_status: 'PAID (Demo)',
        status: 'CONFIRMED',
        delivery_address: `${payload?.shipping_address || 'Demo Address'}, ${payload?.city || 'Chennai'}`,
        created_at: new Date().toISOString(),
        items: [...cart.items]
      };

      const orders = getStorageJSON(STORAGE_KEYS.ORDERS, []);
      orders.unshift(newOrder);
      setStorageJSON(STORAGE_KEYS.ORDERS, orders);

      // Clear cart on successful checkout
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
      1: [{ user_name: 'Priya S.', rating: 5, comment: 'Crisp, delicious, and fresh apples! Truly great quality.' }],
      2: [{ user_name: 'Karthik R.', rating: 5, comment: 'Pure fresh milk, excellent quality and fast delivery.' }],
      3: [{ user_name: 'Sundar M.', rating: 5, comment: 'Fragrant and long-grain Basmati rice. Superb!' }],
      4: [{ user_name: 'Meena K.', rating: 5, comment: 'Very fresh farm vegetables directly at reasonable price.' }],
      5: [{ user_name: 'Vijay T.', rating: 5, comment: 'Sweet, well-ripened bananas. Value for money.' }],
      6: [{ user_name: 'Anand B.', rating: 5, comment: 'Fresh country brown eggs. Excellent packaging.' }]
    });
    return Promise.resolve(allReviews[prodId] || []);
  },

  addReview(productId, rating, comment) {
    const prodId = parseInt(productId, 10);
    const allReviews = getStorageJSON(STORAGE_KEYS.REVIEWS, {});
    if (!allReviews[prodId]) allReviews[prodId] = [];
    allReviews[prodId].unshift({
      user_name: 'Demo Verified Buyer',
      rating: parseInt(rating, 10) || 5,
      comment: comment
    });
    setStorageJSON(STORAGE_KEYS.REVIEWS, allReviews);
    return Promise.resolve(allReviews[prodId]);
  },

  sendChatMessage(message) {
    const msg = (message || '').toLowerCase();
    let reply = "Hello! Welcome to Dhivagar Mart. We offer 100% fresh grocery essentials: Apples (₹120), Milk (₹60), Rice (₹70), Vegetables (₹80), Bananas (₹50), and Eggs (₹90) with instant doorstep delivery!";
    
    if (msg.includes('apple') || msg.includes('fruit')) {
      reply = "Our fresh organic Apples are just ₹120/kg and fresh sweet Bananas are ₹50/dozen. Direct from farm orchards!";
    } else if (msg.includes('milk') || msg.includes('dairy')) {
      reply = "Pure pasteurized farm-fresh whole milk is available at ₹60/Litre. Pack of 6 nutritious eggs is ₹90.";
    } else if (msg.includes('rice') || msg.includes('grain')) {
      reply = "Premium aged Basmati rice is available at ₹70/kg. Fluffy, aromatic, and excellent for all meals.";
    } else if (msg.includes('vegetable') || msg.includes('veggie')) {
      reply = "Fresh assorted seasonal vegetables are ₹80/kg. Direct harvest from local farms!";
    } else if (msg.includes('order') || msg.includes('buy') || msg.includes('cart')) {
      reply = "Simply click 'Add to Cart' on any item, view your cart, and proceed to our instant Demo Checkout!";
    }
    return Promise.resolve({ reply });
  }
};

// Main API Export: Connected to standalone demo store
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
  addReview: (productId, rating, comment) => DemoStore.addReview(productId, rating, comment),

  // Seller & Admin Demo Stubs
  getSellerProducts: () => DemoStore.getProducts('All', ''),
  createSellerProduct: (payload) => DemoStore.getProducts('All', ''),
  updateSellerProduct: (id, payload) => DemoStore.getProducts('All', ''),
  deleteSellerProduct: (id) => DemoStore.getProducts('All', ''),
  getSellerOrders: () => DemoStore.getOrders(),
  updateSellerOrderStatus: (id, status) => DemoStore.getOrders(),
  getSellerStats: () => Promise.resolve({ total_sales_cents: 350000, total_orders: 12 }),

  getAdminUsers: () => Promise.resolve([
    { id: 1, name: 'Admin', email: 'admin@dhivagarmart.com', role: 'ADMIN' },
    { id: 2, name: 'Demo Guest', email: 'guest@dhivagarmart.com', role: 'BUYER' }
  ]),
  getAdminProducts: () => DemoStore.getProducts('All', ''),
  deleteAdminProduct: (id) => DemoStore.getProducts('All', ''),
  getAdminOrders: () => DemoStore.getOrders(),
  updateAdminOrderStatus: (id, status) => DemoStore.getOrders(),
  getAdminStats: () => Promise.resolve({ total_users: 7, total_products: 6, total_orders: 43 }),

  // AI Chat
  sendChatMessage: (message) => DemoStore.sendChatMessage(message)
};
