// Dhivagar Mart - Cart Page Logic
async function loadCartPage() {
  const container = document.getElementById('cart-container');
  if (!container) return;

  try {
    const cart = await api.getCart();

    if (!cart.items || cart.items.length === 0) {
      container.innerHTML = `
        <div style="text-align: center; padding: 4rem 1rem;">
          <div style="font-size: 3rem; margin-bottom: 1rem;">🛒</div>
          <h2>Your cart is empty.</h2>
          <p style="color: var(--text-muted); margin: 1rem 0 2rem;">Explore our high-performance electronics, gadgets, and accessories!</p>
          <a href="products.html" class="btn btn-primary">Start Shopping</a>
        </div>
      `;
      return;
    }

    container.innerHTML = `
      <div style="display: grid; grid-template-columns: 2fr 1fr; gap: 2rem;">
        <div>
          <table>
            <thead>
              <tr>
                <th>Product</th>
                <th>Price</th>
                <th>Quantity</th>
                <th>Subtotal</th>
                <th></th>
              </tr>
            </thead>
            <tbody>
              ${cart.items.map(item => `
                <tr>
                  <td>
                    <div style="display: flex; align-items: center; gap: 0.8rem;">
                      <img src="${item.product_image_url || 'https://images.unsplash.com/photo-1542838132-92c53300491e?w=100'}" 
                           style="width: 50px; height: 50px; object-fit: cover; border-radius: 6px;">
                      <div>
                        <strong>${item.product_name}</strong>
                        <div style="font-size: 0.8rem; color: #6b7280;">${item.product_category}</div>
                      </div>
                    </div>
                  </td>
                  <td>${formatINR(item.unit_price_cents)}</td>
                  <td>
                    <div style="display: flex; align-items: center; gap: 0.4rem;">
                      <button onclick="changeQuantity(${item.product_id}, ${item.quantity - 1})" class="btn btn-secondary btn-sm">-</button>
                      <span style="font-weight: 600; min-width: 24px; text-align: center;">${item.quantity}</span>
                      <button onclick="changeQuantity(${item.product_id}, ${item.quantity + 1})" class="btn btn-secondary btn-sm" ${item.quantity >= item.available_stock ? 'disabled' : ''}>+</button>
                    </div>
                  </td>
                  <td style="font-weight: 700; color: var(--primary);">${formatINR(item.subtotal_cents)}</td>
                  <td>
                    <button onclick="removeItem(${item.product_id})" class="btn btn-danger btn-sm">✕</button>
                  </td>
                </tr>
              `).join('')}
            </tbody>
          </table>
        </div>

        <div>
          <div class="form-card" style="margin: 0; max-width: none;">
            <h3 style="margin-bottom: 1.2rem; color:var(--secondary);">Order Summary</h3>
            <div style="display: flex; justify-content: space-between; margin-bottom: 0.8rem;">
              <span>Total Items:</span>
              <strong>${cart.total_items}</strong>
            </div>
            <div style="display: flex; justify-content: space-between; margin-bottom: 0.8rem;">
              <span>Subtotal:</span>
              <strong>${formatINR(cart.total_cents)}</strong>
            </div>
            <div style="display: flex; justify-content: space-between; margin-bottom: 0.8rem; color: var(--success);">
              <span>Delivery:</span>
              <strong>FREE</strong>
            </div>
            <div style="display: flex; justify-content: space-between; margin-bottom: 1.2rem; color: var(--success);">
              <span>Discount:</span>
              <strong>FREE Shipping</strong>
            </div>
            <hr style="border: 0; border-top: 1px solid var(--border-color); margin-bottom: 1.2rem;">
            <div style="display: flex; justify-content: space-between; margin-bottom: 1.5rem; font-size: 1.25rem;">
              <span style="font-weight:700;">Grand Total:</span>
              <strong style="color: var(--primary); font-size:1.4rem;">${formatINR(cart.total_cents)}</strong>
            </div>
            <a href="checkout.html" class="btn btn-primary" style="width: 100%; text-align:center; display:block; margin-bottom:0.75rem; font-weight:700;">Proceed to Checkout →</a>
            <a href="products.html" class="btn btn-secondary" style="width: 100%; text-align:center; display:block; font-weight:600;">← Continue Shopping</a>
          </div>
        </div>
      </div>
    `;
  } catch (err) {
    if (err.status === 401) {
      container.innerHTML = `
        <div style="text-align: center; padding: 4rem;">
          <h2>Please log in to view your shopping cart</h2>
          <a href="login.html" class="btn btn-primary" style="margin-top: 1rem;">Login Now</a>
        </div>
      `;
    } else {
      container.innerHTML = `<div style="color: red; text-align: center;">Failed to load cart: ${err.message}</div>`;
    }
  }
}

async function changeQuantity(productId, newQty) {
  try {
    await api.updateCartQty(productId, newQty);
    loadCartPage();
    updateCartCount();
  } catch (err) {
    alert('Error: ' + err.message);
  }
}

async function removeItem(productId) {
  if (!confirm('Remove this item from your cart?')) return;
  try {
    await api.removeFromCart(productId);
    loadCartPage();
    updateCartCount();
  } catch (err) {
    alert('Error: ' + err.message);
  }
}
