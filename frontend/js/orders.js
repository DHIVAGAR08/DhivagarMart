// Dhivagar Mart - Buyer Orders
async function loadOrders() {
  const container = document.getElementById('orders-container');
  if (!container) return;

  try {
    const orders = await api.getOrders();

    if (orders.length === 0) {
      container.innerHTML = `
        <div style="text-align: center; padding: 4rem;">
          <h2>No orders placed yet</h2>
          <p style="color: #6b7280; margin: 1rem 0;">Ready to buy fresh groceries?</p>
          <a href="products.html" class="btn btn-primary">Browse Catalog</a>
        </div>
      `;
      return;
    }

    container.innerHTML = orders.map(ord => {
      let statusColor = '#3b82f6';
      if (ord.status === 'CONFIRMED') statusColor = '#10b981';
      if (ord.status === 'SHIPPED') statusColor = '#f59e0b';
      if (ord.status === 'DELIVERED') statusColor = '#059669';
      if (ord.status === 'CANCELLED') statusColor = '#ef4444';

      return `
        <div class="form-card" style="max-width: none; margin-bottom: 1.5rem; padding: 1.5rem;">
          <div style="display: flex; justify-content: space-between; align-items: center; margin-bottom: 1rem; border-bottom: 1px solid var(--border-color); padding-bottom: 0.8rem;">
            <div>
              <span style="font-weight: 800; font-size: 1.1rem;">Order #${ord.id}</span>
              <span style="color: #6b7280; font-size: 0.85rem; margin-left: 0.8rem;">Placed on: ${ord.created_at}</span>
            </div>
            <div>
              <span class="badge" style="background: ${statusColor}; font-size: 0.85rem; padding: 0.3rem 0.8rem;">
                ${ord.status}
              </span>
            </div>
          </div>

          <div style="margin-bottom: 1rem;">
            <table style="box-shadow: none; border: 1px solid var(--border-color);">
              <thead>
                <tr>
                  <th>Product</th>
                  <th>Quantity</th>
                  <th>Unit Price</th>
                  <th>Subtotal</th>
                  <th>Actions</th>
                </tr>
              </thead>
              <tbody>
                ${ord.items.map(item => `
                  <tr>
                    <td><strong>${item.product_name}</strong></td>
                    <td>${item.quantity}</td>
                    <td>${item.unit_price_formatted}</td>
                    <td>${item.subtotal_formatted}</td>
                    <td>
                      <a href="product-details.html?id=${item.product_id}#reviews" class="btn btn-secondary btn-sm">
                        ⭐ Review Product
                      </a>
                    </td>
                  </tr>
                `).join('')}
              </tbody>
            </table>
          </div>

          <div style="text-align: right; font-size: 1.15rem;">
            <span>Order Total: </span>
            <strong style="color: var(--primary); font-size: 1.3rem;">${ord.total_amount_formatted}</strong>
          </div>
        </div>
      `;
    }).join('');
  } catch (err) {
    if (err.status === 401) {
      window.location.href = 'login.html';
    } else {
      container.innerHTML = `<div style="color: red; text-align: center;">Failed to load orders: ${err.message}</div>`;
    }
  }
}
