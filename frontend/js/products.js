// Dhivagar Mart - Products Catalog, Filters, Search & Cart Handlers
let currentCategory = 'All';

async function loadProducts(category = currentCategory, search = '') {
  const grid = document.getElementById('products-grid');
  if (!grid) return;

  grid.innerHTML = '<div style="grid-column: 1/-1; text-align: center; padding: 3rem; color: var(--text-muted);"><span style="font-size:2rem;">⏳</span><br>Loading high-performance tech hardware...</div>';

  try {
    const products = await api.getProducts(category, search);

    if (products.length === 0) {
      grid.innerHTML = `
        <div style="grid-column: 1/-1; text-align: center; padding: 4rem 1rem; color: var(--text-muted);">
          <div style="font-size:3rem; margin-bottom:1rem;">🔍</div>
          <h3 style="font-size:1.25rem; font-weight:700; color:var(--secondary); margin-bottom:0.5rem;">No tech products found</h3>
          <p>Try searching with different keywords such as "5G", "16GB", "Wireless", or browse all categories.</p>
          <button onclick="filterCategory('All', null)" class="btn btn-secondary btn-sm" style="margin-top:1rem;">
            Show All Products
          </button>
        </div>
      `;
      return;
    }

    grid.innerHTML = products.map(p => {
      const inStock = p.stock_qty > 0;
      const isLowStock = inStock && p.stock_qty < 10;
      const stockBadge = inStock 
        ? (isLowStock 
            ? `<span class="stock-tag stock-low">Low Stock (${p.stock_qty})</span>` 
            : `<span class="stock-tag stock-in">In Stock</span>`)
        : `<span class="stock-tag stock-out">Out of Stock</span>`;

      // Deterministic rating between 4.5 and 4.9 for realistic display
      const ratings = ['★ 4.8 (128)', '★ 4.9 (245)', '★ 4.7 (89)', '★ 4.6 (64)', '★ 5.0 (320)'];
      const rating = ratings[p.id % ratings.length];

      return `
        <div class="product-card">
          <div class="product-image-container" onclick="window.location.href='product-details.html?id=${p.id}'" style="cursor:pointer;">
            <img class="product-img" 
                 src="${p.image_url || 'https://images.unsplash.com/photo-1542838132-92c53300491e?w=500'}" 
                 alt="${p.name}" 
                 loading="lazy">
            <span class="product-tag">${p.category}</span>
            ${stockBadge}
          </div>
          <div class="product-info">
            <div class="product-category-row">
              <span class="product-category">${p.category}</span>
              <span class="product-rating">${rating}</span>
            </div>
            <a href="product-details.html?id=${p.id}" class="product-title" title="${p.name}">
              ${p.name}
            </a>
            <div class="product-desc">${p.description}</div>
            
            <div class="product-price-row">
              <span class="product-price">${p.price_formatted}</span>
            </div>

            <div class="product-footer-actions">
              <button onclick="handleAddToCart(${p.id})" class="btn btn-secondary btn-sm" ${!inStock ? 'disabled' : ''} style="font-weight:700;">
                🛒 Add
              </button>
              <button onclick="handleBuyNow(${p.id})" class="btn btn-primary btn-sm" ${!inStock ? 'disabled' : ''} style="font-weight:700;">
                ⚡ Buy
              </button>
            </div>
          </div>
        </div>
      `;
    }).join('');
  } catch (err) {
    grid.innerHTML = `<div style="grid-column: 1/-1; text-align: center; color: red; padding: 2rem;">Failed to load catalog: ${err.message}</div>`;
  }
}

async function handleAddToCart(productId) {
  try {
    await api.addToCart(productId, 1);
    updateCartCount();
    showNotification('✓ Added to shopping cart!');
  } catch (err) {
    if (err.status === 401) {
      alert('Please log in to add items to your shopping cart.');
      window.location.href = 'login.html';
    } else {
      alert('Error: ' + err.message);
    }
  }
}

async function handleBuyNow(productId) {
  try {
    await api.addToCart(productId, 1);
    updateCartCount();
    window.location.href = 'checkout.html';
  } catch (err) {
    if (err.status === 401) {
      alert('Please log in to checkout.');
      window.location.href = 'login.html';
    } else {
      alert('Error: ' + err.message);
    }
  }
}

function filterCategory(cat, element) {
  currentCategory = cat;
  
  // Update category chips
  document.querySelectorAll('.category-chip').forEach(el => {
    const text = el.textContent.replace(/[^a-zA-Z &]/g, '').trim().toLowerCase();
    if (text === cat.toLowerCase() || (cat === 'All' && text.includes('all'))) {
      el.classList.add('active');
    } else {
      el.classList.remove('active');
    }
  });

  // Update visual category cards if present
  document.querySelectorAll('.category-card').forEach(el => {
    const title = el.querySelector('.category-title')?.textContent.trim();
    if (title && title.toLowerCase() === cat.toLowerCase()) {
      el.classList.add('active');
    } else {
      el.classList.remove('active');
    }
  });

  const subtitle = document.getElementById('catalog-subtitle');
  if (subtitle) {
    subtitle.textContent = cat === 'All' 
      ? 'Showing latest electronics with verified warranty' 
      : `Browsing ${cat} hardware with fast express shipping`;
  }

  const searchInput = document.getElementById('nav-search-input') || document.getElementById('search-input');
  loadProducts(cat, searchInput ? searchInput.value : '');
}

function handleSearch(e) {
  if (e) e.preventDefault();
  const searchInput = document.getElementById('nav-search-input') || document.getElementById('search-input');
  const term = searchInput ? searchInput.value.trim() : '';

  // If on home page and searching, scroll down to products section
  const section = document.getElementById('products-section');
  if (section) {
    section.scrollIntoView({ behavior: 'smooth' });
  }

  loadProducts(currentCategory, term);
}

function showNotification(msg) {
  const toast = document.createElement('div');
  toast.innerHTML = `<span style="font-weight:700;">${msg}</span>`;
  toast.style.position = 'fixed';
  toast.style.bottom = '95px';
  toast.style.left = '50%';
  toast.style.transform = 'translateX(-50%)';
  toast.style.backgroundColor = '#0f172a';
  toast.style.color = '#38bdf8';
  toast.style.border = '1px solid #1e3a8a';
  toast.style.padding = '0.75rem 1.6rem';
  toast.style.borderRadius = '9999px';
  toast.style.boxShadow = '0 10px 25px rgba(0,0,0,0.3)';
  toast.style.zIndex = '9999';
  document.body.appendChild(toast);
  setTimeout(() => toast.remove(), 2500);
}
