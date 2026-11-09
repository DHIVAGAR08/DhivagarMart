// Dhivagar Mart - Electronics Catalog & Product Grid Controller
let currentCategory = 'All';
let currentSort = 'featured';
let filterInStockOnly = false;
let rawProducts = [];

async function loadProducts(category = currentCategory, search = '') {
  const grid = document.getElementById('products-grid');
  if (!grid) return;

  grid.innerHTML = '<div style="grid-column: 1/-1; text-align: center; padding: 3rem; color: var(--text-muted);"><span style="font-size:2rem;">⚡</span><br>Loading electronics catalog...</div>';

  try {
    rawProducts = await api.getProducts(category, search);
    renderProductGrid();
  } catch (err) {
    grid.innerHTML = `<div style="grid-column: 1/-1; text-align: center; color: red; padding: 2rem;">Failed to load catalog: ${err.message}</div>`;
  }
}

function handleSortChange(sortMode) {
  currentSort = sortMode;
  renderProductGrid();
}

function handleInStockFilter(onlyInStock) {
  filterInStockOnly = onlyInStock;
  renderProductGrid();
}

function renderProductGrid() {
  const grid = document.getElementById('products-grid');
  if (!grid) return;

  let list = [...rawProducts];

  // In-stock filtering
  if (filterInStockOnly) {
    list = list.filter(p => p.stock_qty > 0);
  }

  // Sorting
  if (currentSort === 'price-asc') {
    list.sort((a, b) => a.price_cents - b.price_cents);
  } else if (currentSort === 'price-desc') {
    list.sort((a, b) => b.price_cents - a.price_cents);
  } else if (currentSort === 'rating') {
    list.sort((a, b) => (b.rating || 0) - (a.rating || 0));
  } else if (currentSort === 'newest') {
    list.sort((a, b) => b.id - a.id);
  }

  if (list.length === 0) {
    grid.innerHTML = `
      <div style="grid-column: 1/-1; text-align: center; padding: 4rem 1rem; color: var(--text-muted); background:#ffffff; border-radius:var(--radius-md); border:1px solid var(--border-color);">
        <div style="font-size:3rem; margin-bottom:1rem;">🔍</div>
        <h3 style="font-size:1.25rem; font-weight:700; color:var(--secondary); margin-bottom:0.5rem;">No matching electronics found.</h3>
        <p>Try searching with another keyword or browse all electronics categories.</p>
        <button onclick="filterCategory('All', null)" class="btn btn-primary btn-sm" style="margin-top:1rem;">
          View All Electronics
        </button>
      </div>
    `;
    return;
  }

  grid.innerHTML = list.map(p => {
    const inStock = p.stock_qty > 0;
    const stockClass = inStock ? (p.stock_qty < 10 ? 'stock-low' : 'stock-in') : 'stock-out';
    const stockText = inStock ? (p.stock_qty < 10 ? `Only ${p.stock_qty} left` : 'In Stock') : 'Out of Stock';

    const origPrice = p.original_price_cents || Math.round(p.price_cents * 1.15);
    const discountPct = Math.round(((origPrice - p.price_cents) / origPrice) * 100);
    const formattedPrice = formatINR(p.price_cents);
    const formattedOriginalPrice = formatINR(origPrice);
    const ratingStars = p.rating ? `★ ${p.rating.toFixed(1)}` : '★ 4.8';
    const reviewsFormatted = p.review_count ? `(${p.review_count.toLocaleString()})` : '(450)';
    const badgeLabel = p.badge || (p.id % 2 === 0 ? "Amazon's Choice" : "Bestseller");

    return `
      <div class="product-card">
        <button onclick="handleQuickWishlist(event, ${p.id})" title="Save to Wishlist" 
                style="position:absolute; top:12px; right:12px; z-index:3; background:rgba(255,255,255,0.95); border:1px solid #e2e8f0; border-radius:50%; width:36px; height:36px; display:flex; align-items:center; justify-content:center; cursor:pointer; font-size:1.1rem; box-shadow:var(--shadow-sm); transition:transform 0.15s ease;">
          ❤️
        </button>
        <div class="product-image-container" onclick="window.location.href='product-details.html?id=${p.id}'" style="cursor:pointer;">
          <img class="product-img" 
               src="${p.image_url || 'https://images.unsplash.com/photo-1511707171634-5f897ff02aa9?w=600'}" 
               alt="${p.name}" 
               loading="lazy">
          <span class="product-badge-choice">${badgeLabel}</span>
        </div>
        <div class="product-info">
          <div class="product-category-row">
            <span class="product-category">${p.category}</span>
            <span class="product-rating">${ratingStars} <span style="font-weight:normal; color:#64748b; font-size:0.75rem;">${reviewsFormatted}</span></span>
          </div>
          <a href="product-details.html?id=${p.id}" class="product-title" title="${p.name}">
            ${p.name}
          </a>
          <div class="product-desc">${p.description}</div>
          
          <div class="product-price-row">
            <span class="product-price">${formattedPrice}</span>
            <span class="product-original-price">${formattedOriginalPrice}</span>
            <span class="discount-badge">${discountPct}% OFF</span>
          </div>

          <div class="prime-delivery-tag">
            <span>✓ FREE Delivery by Tomorrow</span>
          </div>

          <div class="product-footer-actions">
            <button onclick="handleAddToCart(${p.id})" class="btn btn-primary btn-sm" ${!inStock ? 'disabled' : ''} style="font-weight:700;">
              Add to Cart
            </button>
            <button onclick="handleBuyNow(${p.id})" class="btn btn-buy-now btn-sm" ${!inStock ? 'disabled' : ''} style="font-weight:700;">
              Buy Now
            </button>
          </div>
        </div>
      </div>
    `;
  }).join('');
}

async function handleQuickWishlist(e, productId) {
  if (e) e.stopPropagation();
  try {
    await api.addToWishlist(productId);
    if (typeof updateWishlistCount === 'function') updateWishlistCount();
    showNotification('❤️ Added to your Wishlist!');
  } catch (err) {
    showNotification('ℹ️ ' + err.message);
  }
}

async function handleAddToCart(productId) {
  try {
    await api.addToCart(productId, 1);
    if (typeof updateCartCount === 'function') updateCartCount();
    showNotification('✓ Item added to Cart!');
  } catch (err) {
    alert('Error: ' + err.message);
  }
}

async function handleBuyNow(productId) {
  try {
    await api.addToCart(productId, 1);
    if (typeof updateCartCount === 'function') updateCartCount();
    window.location.href = 'checkout.html';
  } catch (err) {
    alert('Error: ' + err.message);
  }
}

function filterCategory(cat, element) {
  currentCategory = cat;
  
  // Update category chips
  document.querySelectorAll('.category-chip').forEach(el => {
    const text = el.textContent.replace(/[^a-zA-Z &]/g, '').trim().toLowerCase();
    if (text === cat.toLowerCase() || (cat === 'All' && (text.includes('all') || text === 'all electronics'))) {
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
    subtitle.textContent = (cat === 'All' || cat === 'All Electronics')
      ? 'Showing flagship smartphones, smartwatches, audio & accessories with brand warranty' 
      : `Browsing ${cat} with fast express delivery & warranty`;
  }

  const searchInput = document.getElementById('nav-search-input') || document.getElementById('search-input');
  loadProducts(cat, searchInput ? searchInput.value : '');
}

function handleSearch(e) {
  if (e) e.preventDefault();
  const searchInput = document.getElementById('nav-search-input') || document.getElementById('search-input');
  const catSelect = document.getElementById('search-category-select');
  const term = searchInput ? searchInput.value.trim() : '';

  if (catSelect && catSelect.value && catSelect.value !== 'All') {
    currentCategory = catSelect.value;
  }

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
  toast.style.backgroundColor = '#131921';
  toast.style.color = '#febd69';
  toast.style.border = '1px solid #374151';
  toast.style.padding = '0.75rem 1.6rem';
  toast.style.borderRadius = '9999px';
  toast.style.boxShadow = '0 10px 25px rgba(0,0,0,0.3)';
  toast.style.zIndex = '9999';
  document.body.appendChild(toast);
  setTimeout(() => toast.remove(), 2500);
}
