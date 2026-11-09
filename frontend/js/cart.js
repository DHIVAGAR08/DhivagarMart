// Dhivagar Mart - Electronics Cart Page Controller
async function loadCartPage() {
  const container = document.getElementById('cart-container');
  if (!container) return;

  try {
    const cart = await api.getCart();

    if (!cart.items || cart.items.length === 0) {
      container.innerHTML = `
        <div style="text-align: center; padding: 4rem 1rem; background:white; border-radius:var(--radius-md); border:1px solid var(--border-color);">
          <div style="font-size: 3.5rem; margin-bottom: 1rem;">🛒</div>
          <h2 style="color:var(--secondary); font-size:1.6rem; font-weight:800; margin-bottom:0.5rem;">Your Shopping Cart is empty</h2>
          <p style="color: var(--text-muted); margin: 0.5rem 0 2rem;">Explore the latest flagship smartphones, smartwatches, and audio hardware!</p>
          <a href="products.html" class="btn btn-primary btn-lg">Explore Electronics Catalog</a>
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
                <th>Action</th>
              </tr>
            </thead>
            <tbody>
              ${cart.items.map(item => `
                <tr>
                  <td>
                    <div style="display: flex; align-items: center; gap: 1rem;">
                      <img src="${item.product_image_url || 'https://images.unsplash.com/photo-1511707171634-5f897ff02aa9?w=100'}" 
                           style="width: 60px; height: 60px; object-fit: contain; border-radius: 6px; background:#f8fafc; padding:4px; border:1px solid #e2e8f0;">
                      <div>
                        <a href="product-details.html?id=${item.product_id}" style="font-weight: 700; color:var(--secondary); text-decoration:none;">
                          ${item.product_name}
                        </a>
                        <div style="font-size: 0.78rem; color: #2563eb; font-weight:600; margin-top:2px;">${item.product_category}</div>
                        <div style="font-size: 0.75rem; color: #059669; font-weight:600;">✓ In Stock</div>
                      </div>
                    </div>
                  </td>
                  <td style="font-weight:600;">${formatINR(item.unit_price_cents)}</td>
                  <td>
                    <div style="display: flex; align-items: center; gap: 0.4rem;">
                      <button onclick="changeQuantity(${item.product_id}, ${item.quantity - 1})" class="btn btn-secondary btn-sm" style="font-weight:800; width:30px; height:30px; padding:0;">−</button>
                      <span style="font-weight: 700; min-width: 24px; text-align: center;">${item.quantity}</span>
                      <button onclick="changeQuantity(${item.product_id}, ${item.quantity + 1})" class="btn btn-secondary btn-sm" style="font-weight:800; width:30px; height:30px; padding:0;" ${item.quantity >= item.available_stock ? 'disabled' : ''}>+</button>
                    </div>
                  </td>
                  <td style="font-weight: 800; color: var(--secondary);">${formatINR(item.subtotal_cents)}</td>
                  <td>
                    <button onclick="removeItem(${item.product_id})" class="btn btn-secondary btn-sm" title="Remove from cart" style="color:#ef4444; border-color:#fecaca;">
                      ✕ Remove
                    </button>
                  </td>
                </tr>
              `).join('')}
            </tbody>
          </table>
        </div>

        <div>
          <div class="form-card" style="margin: 0; max-width: none; position:sticky; top:80px;">
            <h3 style="margin-bottom: 1.2rem; color:var(--secondary); font-size:1.2rem; font-weight:800; border-bottom:1px solid #e2e8f0; padding-bottom:0.8rem;">
              Order Summary
            </h3>
            <div style="display: flex; justify-content: space-between; margin-bottom: 0.8rem; font-size:0.95rem;">
              <span>Subtotal (${cart.total_items} items):</span>
              <strong>${formatINR(cart.total_cents)}</strong>
            </div>
            <div style="display: flex; justify-content: space-between; margin-bottom: 0.8rem; font-size:0.95rem; color: #059669;">
              <span>Delivery Charges:</span>
              <strong style="text-transform:uppercase;">FREE Express</strong>
            </div>
            <div style="display: flex; justify-content: space-between; margin-bottom: 1.2rem; font-size:0.85rem; color: #64748b;">
              <span>Taxes & GST:</span>
              <span>Included in price</span>
            </div>
            <hr style="border: 0; border-top: 1px solid var(--border-color); margin-bottom: 1.2rem;">
            <div style="display: flex; justify-content: space-between; margin-bottom: 1.5rem; font-size: 1.25rem;">
              <span style="font-weight:800;">Order Total:</span>
              <strong style="color: var(--secondary); font-size:1.45rem;">${formatINR(cart.total_cents)}</strong>
            </div>
            <a href="checkout.html" class="btn btn-buy-now btn-lg" style="width: 100%; text-align:center; display:block; margin-bottom:0.75rem; font-weight:800; font-size:1.05rem;">
              Proceed to Checkout →
            </a>
            <a href="products.html" class="btn btn-secondary" style="width: 100%; text-align:center; display:block; font-weight:600;">
              ← Continue Shopping
            </a>
          </div>
        </div>
      </div>
    `;
  } catch (err) {
    container.innerHTML = `<div style="color: red; text-align: center;">Failed to load cart: ${err.message}</div>`;
  }
}

async function changeQuantity(productId, newQty) {
  try {
    await api.updateCartQty(productId, newQty);
    loadCartPage();
    if (typeof updateCartCount === 'function') updateCartCount();
  } catch (err) {
    alert('Error: ' + err.message);
  }
}

async function removeItem(productId) {
  if (!confirm('Remove this product from your shopping cart?')) return;
  try {
    await api.removeFromCart(productId);
    loadCartPage();
    if (typeof updateCartCount === 'function') updateCartCount();
  } catch (err) {
    alert('Error: ' + err.message);
  }
}
