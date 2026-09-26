// Dhivagar Mart - Wishlist Page Logic
async function loadWishlistPage() {
  const container = document.getElementById('wishlist-container');
  if (!container) return;

  try {
    const items = await api.getWishlist();

    if (!items || items.length === 0) {
      container.innerHTML = `
        <div style="text-align: center; padding: 4rem 1rem;">
          <div style="font-size: 3rem; margin-bottom: 1rem;">❤️</div>
          <h2>Your wishlist is empty</h2>
          <p style="color: var(--text-muted); margin: 1rem 0 2rem;">Save your favorite electronics, laptops, and gadgets for later!</p>
          <a href="products.html" class="btn btn-primary">Explore Products</a>
        </div>
      `;
      return;
    }

    container.innerHTML = `
      <div style="overflow-x: auto;">
        <table>
          <thead>
            <tr>
              <th>Product Hardware</th>
              <th>Category</th>
              <th>Price</th>
              <th>Stock Status</th>
              <th>Actions</th>
            </tr>
          </thead>
          <tbody>
            ${items.map(item => {
              const inStock = item.product_stock_qty > 0;
              const isLowStock = inStock && item.product_stock_qty < 10;
              const stockClass = inStock ? (isLowStock ? 'stock-low' : 'stock-in') : 'stock-out';
              const stockText = inStock ? (isLowStock ? `Low (${item.product_stock_qty} left)` : `In Stock`) : 'Out of Stock';

              return `
                <tr>
                  <td>
                    <div style="display: flex; align-items: center; gap: 0.8rem;">
                      <img src="${item.product_image_url || 'https://images.unsplash.com/photo-1542838132-92c53300491e?w=100'}" 
                           style="width: 50px; height: 50px; object-fit: cover; border-radius: 6px;">
                      <div>
                        <a href="product-details.html?id=${item.product_id}" style="font-weight: 700; color: var(--secondary); text-decoration: none;">
                          ${item.product_name}
                        </a>
                      </div>
                    </div>
                  </td>
                  <td><span class="badge" style="background:#e5e7eb; color:#374151;">${item.product_category}</span></td>
                  <td><strong style="color: var(--primary);">${formatINR(item.product_price_cents)}</strong></td>
                  <td>
                    <span class="stock-tag ${stockClass}" style="position: static;">
                      ${stockText}
                    </span>
                  </td>
                  <td>
                    <div style="display: flex; gap: 0.5rem;">
                      <button onclick="handleMoveToCart(${item.product_id})" class="btn btn-primary btn-sm" ${!inStock ? 'disabled' : ''} style="font-weight: 700;">
                        🛒 Move to Cart
                      </button>
                      <button onclick="handleRemoveWishlist(${item.product_id})" class="btn btn-secondary btn-sm" title="Remove from wishlist">
                        ✕
                      </button>
                    </div>
                  </td>
                </tr>
              `;
            }).join('')}
          </tbody>
        </table>
      </div>
    `;
  } catch (err) {
    if (err.status === 401) {
      window.location.href = 'login.html';
    } else {
      container.innerHTML = `<div style="color: red; text-align: center; padding: 2rem;">Failed to load wishlist: ${err.message}</div>`;
    }
  }
}

async function handleMoveToCart(productId) {
  try {
    await api.moveWishlistToCart(productId);
    updateCartCount();
    updateWishlistCount();
    loadWishlistPage();
    alert('Item moved to your cart!');
  } catch (err) {
    alert('Failed to move item to cart: ' + err.message);
  }
}

async function handleRemoveWishlist(productId) {
  try {
    await api.removeFromWishlist(productId);
    updateWishlistCount();
    loadWishlistPage();
  } catch (err) {
    alert('Failed to remove item: ' + err.message);
  }
}
