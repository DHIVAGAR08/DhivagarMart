// Dhivagar Mart - Admin Control Center
async function initAdminDashboard() {
  await loadAdminStats();
  await loadAdminUsers();
  await loadAdminProducts();
  await loadAdminOrders();
}

async function loadAdminStats() {
  try {
    const stats = await api.getAdminStats();
    document.getElementById('stat-users').textContent = stats.total_users;
    document.getElementById('stat-products').textContent = stats.total_products;
    document.getElementById('stat-orders').textContent = stats.total_orders;
    document.getElementById('stat-revenue').textContent = stats.total_revenue_formatted;
  } catch (err) {
    console.error('Failed to load stats', err);
  }
}

async function loadAdminUsers() {
  const container = document.getElementById('admin-users-table');
  if (!container) return;

  try {
    const users = await api.getAdminUsers();
    container.innerHTML = users.map(u => `
      <tr>
        <td><strong>#${u.id}</strong></td>
        <td>${u.name}</td>
        <td>${u.email}</td>
        <td>
          <span class="badge" style="background: ${u.role === 'ADMIN' ? '#dc2626' : u.role === 'SELLER' ? '#0284c7' : '#10b981'};">
            ${u.role}
          </span>
        </td>
        <td><span style="font-size:0.85rem; color:#6b7280;">${u.created_at}</span></td>
      </tr>
    `).join('');
  } catch (err) {
    if (err.status === 401 || err.status === 403) {
      alert('Access restricted to System Administrators.');
      window.location.href = 'login.html';
    } else {
      container.innerHTML = `<tr><td colspan="5" style="color:red;">Error: ${err.message}</td></tr>`;
    }
  }
}

async function loadAdminProducts() {
  const container = document.getElementById('admin-products-table');
  if (!container) return;

  try {
    const products = await api.getAdminProducts();
    container.innerHTML = products.map(p => `
      <tr>
        <td><strong>#${p.id}</strong></td>
        <td>Seller #${p.seller_id}</td>
        <td>${p.name}</td>
        <td><span class="badge" style="background:#e5e7eb; color:#374151;">${p.category}</span></td>
        <td><strong>${p.price_formatted}</strong></td>
        <td>${p.stock_qty}</td>
        <td>
          <button onclick="handleAdminDeleteProduct(${p.id})" class="btn btn-danger btn-sm">Remove Listing</button>
        </td>
      </tr>
    `).join('');
  } catch (err) {
    container.innerHTML = `<tr><td colspan="7" style="color:red;">Error: ${err.message}</td></tr>`;
  }
}

async function handleAdminDeleteProduct(id) {
  if (!confirm(`Are you sure you want to moderate and remove product listing #${id}?`)) return;
  try {
    await api.deleteAdminProduct(id);
    loadAdminProducts();
    loadAdminStats();
  } catch (err) {
    alert('Failed to remove product: ' + err.message);
  }
}

async function loadAdminOrders() {
  const container = document.getElementById('admin-orders-table');
  if (!container) return;

  try {
    const orders = await api.getAdminOrders();
    container.innerHTML = orders.map(ord => `
      <tr>
        <td><strong>#${ord.id}</strong></td>
        <td>${ord.buyer_name} (${ord.buyer_email})</td>
        <td><strong>${ord.total_amount_formatted}</strong></td>
        <td>
          <select onchange="handleAdminUpdateStatus(${ord.id}, this.value)" class="form-control" style="width: auto; padding: 0.3rem 0.6rem;">
            <option value="PENDING" ${ord.status === 'PENDING' ? 'selected' : ''}>PENDING</option>
            <option value="CONFIRMED" ${ord.status === 'CONFIRMED' ? 'selected' : ''}>CONFIRMED</option>
            <option value="SHIPPED" ${ord.status === 'SHIPPED' ? 'selected' : ''}>SHIPPED</option>
            <option value="DELIVERED" ${ord.status === 'DELIVERED' ? 'selected' : ''}>DELIVERED</option>
            <option value="CANCELLED" ${ord.status === 'CANCELLED' ? 'selected' : ''}>CANCELLED</option>
          </select>
        </td>
        <td><span style="font-size:0.85rem; color:#6b7280;">${ord.created_at}</span></td>
      </tr>
    `).join('');
  } catch (err) {
    container.innerHTML = `<tr><td colspan="5" style="color:red;">Error: ${err.message}</td></tr>`;
  }
}

async function handleAdminUpdateStatus(orderId, status) {
  try {
    await api.updateAdminOrderStatus(orderId, status);
    alert(`Order #${orderId} status updated to ${status}`);
    loadAdminOrders();
    loadAdminStats();
  } catch (err) {
    alert('Status update failed: ' + err.message);
  }
}
