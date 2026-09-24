// Dhivagar Mart - Products Catalog & Search
let currentCategory = 'All';

async function loadProducts(category = currentCategory, search = '') {
  const grid = document.getElementById('products-grid');
  if (!grid) return;

  grid.innerHTML = '<div style="grid-column: 1/-1; text-align: center; padding: 2rem;">Loading fresh groceries...</div>';

  try {
    const products = await api.getProducts(category, search);

    if (products.length === 0) {
      grid.innerHTML = '<div style="grid-column: 1/-1; text-align: center; padding: 3rem; color: #6b7280;">No products found matching your search.</div>';
      return;
    }

    grid.innerHTML = products.map(p => `
      <div class="product-card">
        <img class="product-img" src="${p.image_url || 'https://images.unsplash.com/photo-1542838132-92c53300491e?w=500'}" alt="${p.name}">
        <div class="product-info">
          <div class="product-category">${p.category}</div>
          <a href="product-details.html?id=${p.id}" class="product-title">${p.name}</a>
          <div class="product-desc">${p.description}</div>
          <div class="product-footer">
            <span class="product-price">${p.price_formatted}</span>
            <button onclick="handleAddToCart(${p.id})" class="btn btn-primary btn-sm" ${p.stock_qty <= 0 ? 'disabled' : ''}>
              ${p.stock_qty > 0 ? '+ Add' : 'Out of Stock'}
            </button>
          </div>
        </div>
      </div>
    `).join('');
  } catch (err) {
    grid.innerHTML = `<div style="grid-column: 1/-1; text-align: center; color: red;">Failed to load products: ${err.message}</div>`;
  }
}

async function handleAddToCart(productId) {
  try {
    await api.addToCart(productId, 1);
    updateCartCount();
    showNotification('Item added to cart!');
  } catch (err) {
    if (err.status === 401) {
      alert('Please log in to add items to your shopping cart.');
      window.location.href = 'login.html';
    } else {
      alert('Error: ' + err.message);
    }
  }
}

function filterCategory(cat, element) {
  currentCategory = cat;
  document.querySelectorAll('.category-chip').forEach(el => el.classList.remove('active'));
  if (element) element.classList.add('active');
  const searchInput = document.getElementById('search-input');
  loadProducts(cat, searchInput ? searchInput.value : '');
}

function handleSearch(e) {
  if (e) e.preventDefault();
  const searchInput = document.getElementById('search-input');
  loadProducts(currentCategory, searchInput ? searchInput.value : '');
}

function showNotification(msg) {
  const toast = document.createElement('div');
  toast.textContent = msg;
  toast.style.position = 'fixed';
  toast.style.bottom = '90px';
  toast.style.left = '50%';
  toast.style.transform = 'translateX(-50%)';
  toast.style.backgroundColor = '#10b981';
  toast.style.color = '#fff';
  toast.style.padding = '0.6rem 1.4rem';
  toast.style.borderRadius = '9999px';
  toast.style.boxShadow = '0 4px 12px rgba(0,0,0,0.15)';
  toast.style.zIndex = '9999';
  document.body.appendChild(toast);
  setTimeout(() => toast.remove(), 2500);
}
