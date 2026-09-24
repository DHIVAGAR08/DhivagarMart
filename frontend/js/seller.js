// Dhivagar Mart - Seller Portal
async function initSellerDashboard() {
  await loadSellerProducts();
  await loadSellerOrders();
}

async function loadSellerProducts() {
  const container = document.getElementById('seller-products-table');
  if (!container) return;

  try {
    const products = await api.getSellerProducts();
    if (products.length === 0) {
      container.innerHTML = '<tr><td colspan="6" style="text-align:center;">No products listed yet. Click "+ Add Product" to create your first listing.</td></tr>';
      return;
    }

    container.innerHTML = products.map(p => `
      <tr>
        <td><strong>#${p.id}</strong></td>
        <td>
          <div style="display:flex; align-items:center; gap:0.6rem;">
            <img src="${p.image_url || 'https://images.unsplash.com/photo-1542838132-92c53300491e?w=80'}" style="width:40px; height:40px; object-fit:cover; border-radius:4px;">
            <span>${p.name}</span>
          </div>
        </td>
        <td><span class="badge" style="background:#e5e7eb; color:#374151;">${p.category}</span></td>
        <td><strong>${p.price_formatted}</strong></td>
        <td>
          <span style="font-weight:600; color: ${p.stock_qty < 10 ? 'var(--danger)' : 'var(--success)'};">
            ${p.stock_qty} in stock
          </span>
        </td>
        <td>
          <div style="display:flex; gap:0.4rem;">
            <button onclick='openEditModal(${JSON.stringify(p)})' class="btn btn-secondary btn-sm">Edit</button>
            <button onclick="handleDeleteProduct(${p.id})" class="btn btn-danger btn-sm">Delete</button>
          </div>
        </td>
      </tr>
    `).join('');
  } catch (err) {
    if (err.status === 401 || err.status === 403) {
      alert('Access restricted to Seller accounts.');
      window.location.href = 'login.html';
    } else {
      container.innerHTML = `<tr><td colspan="6" style="color:red;">Error: ${err.message}</td></tr>`;
    }
  }
}

async function loadSellerOrders() {
  const container = document.getElementById('seller-orders-table');
  if (!container) return;

  try {
    const orders = await api.getSellerOrders();
    if (orders.length === 0) {
      container.innerHTML = '<tr><td colspan="5" style="text-align:center;">No customer orders received yet.</td></tr>';
      return;
    }

    container.innerHTML = orders.map(ord => `
      <tr>
        <td><strong>#${ord.id}</strong></td>
        <td>${ord.buyer_name} (${ord.buyer_email})</td>
        <td><strong>${ord.total_amount_formatted}</strong></td>
        <td>
          <select onchange="handleUpdateStatus(${ord.id}, this.value)" class="form-control" style="width: auto; padding: 0.3rem 0.6rem;">
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

async function handleSaveProduct(e) {
  e.preventDefault();
  const id = document.getElementById('product-modal-id').value;
  const name = document.getElementById('product-name').value;
  const category = document.getElementById('product-category').value;
  const priceDollars = parseFloat(document.getElementById('product-price').value);
  const stock = parseInt(document.getElementById('product-stock').value, 10);
  const desc = document.getElementById('product-desc').value;
  const imgUrl = document.getElementById('product-image').value;

  const payload = {
    name,
    category,
    price_cents: Math.round(priceDollars * 100),
    stock_qty: stock,
    description: desc,
    image_url: imgUrl
  };

  try {
    if (id) {
      await api.updateSellerProduct(id, payload);
    } else {
      await api.createSellerProduct(payload);
    }
    closeProductModal();
    loadSellerProducts();
  } catch (err) {
    alert('Failed to save product: ' + err.message);
  }
}

async function handleDeleteProduct(id) {
  if (!confirm(`Are you sure you want to delete product #${id}?`)) return;
  try {
    await api.deleteSellerProduct(id);
    loadSellerProducts();
  } catch (err) {
    alert('Delete failed: ' + err.message);
  }
}

async function handleUpdateStatus(orderId, status) {
  try {
    await api.updateSellerOrderStatus(orderId, status);
    alert(`Order #${orderId} status updated to ${status}`);
    loadSellerOrders();
  } catch (err) {
    alert('Status update failed: ' + err.message);
  }
}

function openAddModal() {
  document.getElementById('product-modal-id').value = '';
  document.getElementById('product-modal-title').textContent = 'Add New Grocery Product';
  document.getElementById('product-form').reset();
  document.getElementById('product-modal').style.display = 'flex';
}

function openEditModal(prod) {
  document.getElementById('product-modal-id').value = prod.id;
  document.getElementById('product-modal-title').textContent = `Edit Product #${prod.id}`;
  document.getElementById('product-name').value = prod.name;
  document.getElementById('product-category').value = prod.category;
  document.getElementById('product-price').value = (prod.price_cents / 100).toFixed(2);
  document.getElementById('product-stock').value = prod.stock_qty;
  document.getElementById('product-desc').value = prod.description;
  document.getElementById('product-image').value = prod.image_url;
  document.getElementById('product-modal').style.display = 'flex';
}

function closeProductModal() {
  document.getElementById('product-modal').style.display = 'none';
}
