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

async function apiRequest(endpoint, method = 'GET', body = null) {
  const options = {
    method,
    headers: {
      'Accept': 'application/json'
    },
    credentials: 'include' // Include Drogon session cookies
  };

  if (body) {
    options.headers['Content-Type'] = 'application/json';
    options.body = JSON.stringify(body);
  }

  const response = await fetch(`${API_BASE}${endpoint}`, options);
  const data = await response.json();

  if (!response.ok || (data && data.success === false)) {
    const errorMsg = data?.error?.message || `HTTP ${response.status}: Request failed`;
    const err = new Error(errorMsg);
    err.code = data?.error?.code || 'UNKNOWN_ERROR';
    err.status = response.status;
    throw err;
  }

  return data.data;
}

const api = {
  // Auth
  register: (payload) => apiRequest('/auth/register', 'POST', payload),
  login: (payload) => apiRequest('/auth/login', 'POST', payload),
  logout: () => apiRequest('/auth/logout', 'POST'),
  getCurrentUser: () => apiRequest('/auth/me', 'GET'),

  // Products
  getProducts: (category, search) => {
    const params = new URLSearchParams();
    if (category && category !== 'All') params.append('category', category);
    if (search) params.append('search', search);
    const qs = params.toString() ? `?${params.toString()}` : '';
    return apiRequest(`/products${qs}`, 'GET');
  },
  getProductById: (id) => apiRequest(`/products/${id}`, 'GET'),

  // Cart
  getCart: () => apiRequest('/cart', 'GET'),
  addToCart: (productId, quantity = 1) => apiRequest('/cart', 'POST', { product_id: productId, quantity }),
  updateCartQty: (productId, quantity) => apiRequest(`/cart/${productId}`, 'PUT', { quantity }),
  removeFromCart: (productId) => apiRequest(`/cart/${productId}`, 'DELETE'),

  // Orders
  checkout: (payload) => apiRequest('/orders', 'POST', payload),
  getOrders: () => apiRequest('/orders', 'GET'),
  getOrderById: (id) => apiRequest(`/orders/${id}`, 'GET'),
  cancelOrder: (id) => apiRequest(`/orders/${id}/cancel`, 'PUT'),

  // Wishlist
  getWishlist: () => apiRequest('/wishlist', 'GET'),
  addToWishlist: (productId) => apiRequest('/wishlist', 'POST', { product_id: productId }),
  removeFromWishlist: (productId) => apiRequest(`/wishlist/${productId}`, 'DELETE'),
  moveWishlistToCart: (productId) => apiRequest(`/wishlist/${productId}/move-to-cart`, 'POST'),

  // Reviews
  getProductReviews: (productId) => apiRequest(`/products/${productId}/reviews`, 'GET'),
  addReview: (productId, rating, comment) => apiRequest(`/products/${productId}/reviews`, 'POST', { rating, comment }),

  // Seller
  getSellerProducts: () => apiRequest('/seller/products', 'GET'),
  createSellerProduct: (payload) => apiRequest('/seller/products', 'POST', payload),
  updateSellerProduct: (id, payload) => apiRequest(`/seller/products/${id}`, 'PUT', payload),
  deleteSellerProduct: (id) => apiRequest(`/seller/products/${id}`, 'DELETE'),
  getSellerOrders: () => apiRequest('/seller/orders', 'GET'),
  updateSellerOrderStatus: (id, status) => apiRequest(`/seller/orders/${id}/status`, 'PUT', { status }),
  getSellerStats: () => apiRequest('/seller/stats', 'GET'),

  // Admin
  getAdminUsers: () => apiRequest('/admin/users', 'GET'),
  getAdminProducts: () => apiRequest('/admin/products', 'GET'),
  deleteAdminProduct: (id) => apiRequest(`/admin/products/${id}`, 'DELETE'),
  getAdminOrders: () => apiRequest('/admin/orders', 'GET'),
  updateAdminOrderStatus: (id, status) => apiRequest(`/admin/orders/${id}/status`, 'PUT', { status }),
  getAdminStats: () => apiRequest('/admin/stats', 'GET'),

  // AI Chat
  sendChatMessage: (message) => apiRequest('/ai/chat', 'POST', { message })
};
