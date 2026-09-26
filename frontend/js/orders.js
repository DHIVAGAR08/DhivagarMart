// Dhivagar Mart - Buyer Orders & Fulfillment Tracking
let cachedOrders = [];

function renderTimeline(status) {
  if (status === 'CANCELLED') {
    return `
      <div style="background:#fef2f2; border:1px solid #fecaca; color:#b91c1c; padding:0.6rem 1rem; border-radius:var(--radius-md); font-weight:600; font-size:0.88rem; display:flex; align-items:center; gap:0.5rem; margin:1rem 0;">
        <span>🚫 Order Cancelled</span>
        <span style="font-size:0.8rem; font-weight:normal; color:#dc2626;">— Reserved inventory has been restored to seller stock.</span>
      </div>
    `;
  }

  const stages = [
    { label: 'Placed', key: 'PENDING' },
    { label: 'Confirmed', key: 'CONFIRMED' },
    { label: 'Processing', key: 'PROCESSING' },
    { label: 'Shipped', key: 'SHIPPED' },
    { label: 'Delivered', key: 'DELIVERED' }
  ];

  const statusOrder = {
    'PENDING': 0,
    'CONFIRMED': 1,
    'PROCESSING': 2,
    'SHIPPED': 3,
    'DELIVERED': 4
  };

  const currentIndex = statusOrder[status] !== undefined ? statusOrder[status] : 0;

  return `
    <div style="margin: 1.25rem 0; padding: 0.75rem 0;">
      <div style="display:flex; justify-content:space-between; position:relative;">
        <div style="position:absolute; top:50%; left:0; right:0; height:3px; background:#e2e8f0; z-index:1; transform:translateY(-50%);"></div>
        <div style="position:absolute; top:50%; left:0; width:${(currentIndex / (stages.length - 1)) * 100}%; height:3px; background:var(--primary); z-index:1; transform:translateY(-50%); transition:width 0.3s ease;"></div>
        
        ${stages.map((stage, idx) => {
          const isPassed = idx <= currentIndex;
          const isCurrent = idx === currentIndex;
          const circleBg = isPassed ? 'var(--primary)' : '#ffffff';
          const circleBorder = isPassed ? 'var(--primary)' : '#cbd5e1';
          const textColor = isPassed ? 'var(--secondary)' : '#94a3b8';

          return `
            <div style="position:relative; z-index:2; text-align:center;">
              <div style="width:24px; height:24px; border-radius:50%; background:${circleBg}; border:3px solid ${circleBorder}; margin:0 auto 0.4rem; display:flex; align-items:center; justify-content:center; color:white; font-size:0.7rem; font-weight:bold;">
                ${isPassed ? '✓' : ''}
              </div>
              <span style="font-size:0.75rem; font-weight:${isCurrent ? '700' : '500'}; color:${textColor};">
                ${stage.label}
              </span>
            </div>
          `;
        }).join('')}
      </div>
    </div>
  `;
}

async function loadOrders() {
  const container = document.getElementById('orders-container');
  if (!container) return;

  try {
    const orders = await api.getOrders();
    cachedOrders = orders || [];

    if (cachedOrders.length === 0) {
      container.innerHTML = `
        <div style="text-align: center; padding: 4rem 1rem;">
          <div style="font-size:3rem; margin-bottom:1rem;">📦</div>
          <h2 style="color:var(--secondary); margin-bottom:0.5rem;">No orders placed yet</h2>
          <p style="color: var(--text-muted); margin-bottom: 2rem;">Ready to shop high-performance electronics?</p>
          <a href="products.html" class="btn btn-primary">Browse Catalog</a>
        </div>
      `;
      return;
    }

    container.innerHTML = cachedOrders.map(ord => {
      let statusColor = '#3b82f6';
      if (ord.status === 'CONFIRMED') statusColor = '#10b981';
      if (ord.status === 'PROCESSING') statusColor = '#8b5cf6';
      if (ord.status === 'SHIPPED') statusColor = '#f59e0b';
      if (ord.status === 'DELIVERED') statusColor = '#059669';
      if (ord.status === 'CANCELLED') statusColor = '#ef4444';

      const canCancel = (ord.status === 'PENDING' || ord.status === 'CONFIRMED' || ord.status === 'PROCESSING');

      const paymentMethod = ord.payment_method || 'CASH_ON_DELIVERY';
      const paymentStatus = ord.payment_status || 'PENDING';
      const payStatusColor = (paymentStatus === 'PAID') ? '#10b981' : (paymentStatus === 'REFUNDED' ? '#64748b' : '#f59e0b');

      return `
        <div class="form-card" style="max-width: none; margin-bottom: 2rem; padding: 1.75rem; box-shadow:var(--shadow-md);">
          <!-- Header Bar -->
          <div style="display: flex; justify-content: space-between; align-items: center; flex-wrap:wrap; gap:0.8rem; border-bottom: 1px solid var(--border-color); padding-bottom: 1rem;">
            <div>
              <span style="font-weight: 800; font-size: 1.2rem; color:var(--secondary);">Order #DHV-${ord.id}</span>
              <span style="color: var(--text-muted); font-size: 0.85rem; margin-left: 0.8rem;">
                Placed on: ${ord.created_at}
              </span>
            </div>
            <div style="display:flex; align-items:center; gap:0.6rem;">
              <span class="badge" style="background:${payStatusColor}; font-size:0.78rem;">
                ${paymentMethod} • ${paymentStatus}
              </span>
              <span class="badge" style="background: ${statusColor}; font-size: 0.85rem; padding: 0.35rem 0.85rem;">
                ${ord.status}
              </span>
            </div>
          </div>

          <!-- Tracking Milestones -->
          ${renderTimeline(ord.status)}

          <!-- Order Metadata: Address & Phone -->
          ${ord.delivery_address ? `
            <div style="background:var(--bg-subtle); padding:0.8rem 1rem; border-radius:var(--radius-md); font-size:0.85rem; color:var(--secondary); margin-bottom:1rem; border:1px solid var(--border-color);">
              <strong>📍 Delivery Address:</strong> ${ord.full_name ? `${ord.full_name} • ` : ''}${ord.delivery_address}
              ${ord.phone ? `<span style="color:var(--text-muted); margin-left:0.5rem;">(Tel: ${ord.phone})</span>` : ''}
            </div>
          ` : ''}

          <!-- Items Table -->
          <div style="margin-bottom: 1.2rem; overflow-x:auto;">
            <table style="box-shadow: none; border: 1px solid var(--border-color);">
              <thead>
                <tr>
                  <th>Product Hardware</th>
                  <th>Quantity</th>
                  <th>Unit Price</th>
                  <th>Subtotal</th>
                  <th>Verified Review</th>
                </tr>
              </thead>
              <tbody>
                ${ord.items.map(item => `
                  <tr>
                    <td>
                      <a href="product-details.html?id=${item.product_id}" style="font-weight: 700; color:var(--secondary); text-decoration:none;">
                        ${item.product_name}
                      </a>
                    </td>
                    <td>${item.quantity}</td>
                    <td>${formatINR(item.unit_price_cents)}</td>
                    <td><strong style="color:var(--primary);">${formatINR(item.subtotal_cents)}</strong></td>
                    <td>
                      <a href="product-details.html?id=${item.product_id}#reviews" class="btn btn-secondary btn-sm" style="font-weight:600;">
                        ⭐ Write Review
                      </a>
                    </td>
                  </tr>
                `).join('')}
              </tbody>
            </table>
          </div>

          <!-- Footer Actions & Total -->
          <div style="display:flex; justify-content:space-between; align-items:center; flex-wrap:wrap; gap:1rem; border-top:1px solid var(--border-color); padding-top:1rem;">
            <div style="display:flex; gap:0.8rem;">
              <button onclick="openOrderModal(${ord.id})" class="btn btn-secondary btn-sm" style="font-weight:600;">
                📄 View Full Invoice
              </button>
              ${canCancel ? `
                <button onclick="handleCancelOrder(${ord.id})" class="btn btn-danger btn-sm" style="font-weight:700;">
                  ❌ Cancel Order
                </button>
              ` : ''}
            </div>
            <div>
              <span style="font-size: 1rem; color:var(--text-muted);">Total Paid / Due: </span>
              <strong style="color: var(--primary); font-size: 1.4rem;">${formatINR(ord.total_amount_cents)}</strong>
            </div>
          </div>
        </div>
      `;
    }).join('');
  } catch (err) {
    if (err.status === 401) {
      window.location.href = 'login.html';
    } else {
      container.innerHTML = `<div style="color: red; text-align: center; padding: 3rem;">Failed to load orders: ${err.message}</div>`;
    }
  }
}

async function handleCancelOrder(orderId) {
  if (!confirm(`Are you sure you want to cancel Order #DHV-${orderId}?\n\nInventory will be automatically restored to warehouse stock.`)) {
    return;
  }

  try {
    await api.cancelOrder(orderId);
    alert(`Order #DHV-${orderId} has been cancelled. Inventory has been restored.`);
    loadOrders();
  } catch (err) {
    alert('Cancellation failed: ' + err.message);
  }
}

function openOrderModal(orderId) {
  const ord = cachedOrders.find(o => o.id === orderId);
  if (!ord) return;

  const modal = document.getElementById('order-details-modal');
  const title = document.getElementById('modal-order-title');
  const content = document.getElementById('modal-order-content');

  if (title) title.textContent = `Invoice & Receipt #DHV-${ord.id}`;
  if (content) {
    content.innerHTML = `
      <div style="display:grid; grid-template-columns:1fr 1fr; gap:1rem; margin-bottom:1.2rem; font-size:0.9rem;">
        <div>
          <div><strong>Order Status:</strong> <span class="badge" style="background:#2563eb;">${ord.status}</span></div>
          <div style="margin-top:0.4rem;"><strong>Payment Method:</strong> ${ord.payment_method || 'CASH_ON_DELIVERY'}</div>
          <div style="margin-top:0.4rem;"><strong>Payment Status:</strong> ${ord.payment_status || 'PENDING'}</div>
        </div>
        <div>
          <div><strong>Buyer Name:</strong> ${ord.full_name || ord.buyer_name}</div>
          <div style="margin-top:0.4rem;"><strong>Phone:</strong> ${ord.phone || 'N/A'}</div>
          <div style="margin-top:0.4rem;"><strong>Address:</strong> ${ord.delivery_address || 'N/A'}</div>
        </div>
      </div>

      <h4 style="margin-bottom:0.6rem; color:var(--secondary);">Purchased Hardware:</h4>
      <div style="border:1px solid var(--border-color); border-radius:var(--radius-md); padding:0.8rem; margin-bottom:1.2rem;">
        ${ord.items.map(i => `
          <div style="display:flex; justify-content:space-between; margin-bottom:0.5rem; font-size:0.88rem;">
            <span>${i.product_name} (× ${i.quantity})</span>
            <strong>${formatINR(i.subtotal_cents)}</strong>
          </div>
        `).join('')}
        <div style="border-top:1px solid var(--border-color); padding-top:0.6rem; display:flex; justify-content:space-between; font-weight:800; font-size:1.1rem;">
          <span>Grand Total:</span>
          <span style="color:var(--primary);">${formatINR(ord.total_amount_cents)}</span>
        </div>
      </div>

      <div style="text-align:right;">
        <button onclick="window.print()" class="btn btn-secondary btn-sm">🖨️ Print Invoice</button>
        <button onclick="closeOrderModal()" class="btn btn-primary btn-sm" style="margin-left:0.5rem;">Close</button>
      </div>
    `;
  }

  if (modal) modal.style.display = 'flex';
}

function closeOrderModal() {
  const modal = document.getElementById('order-details-modal');
  if (modal) modal.style.display = 'none';
}
