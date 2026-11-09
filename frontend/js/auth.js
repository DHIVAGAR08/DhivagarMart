// Dhivagar Mart - Auth State & Navbar Management
let currentUser = null;

async function checkAuthState() {
  try {
    currentUser = await api.getCurrentUser();
    updateNavbar(currentUser);
    updateCartCount();
    updateWishlistCount();
    return currentUser;
  } catch (err) {
    currentUser = null;
    updateNavbar(null);
    return null;
  }
}

/**
 * Enforces authentication or demo guest access on pages.
 */
async function requireAuth(allowedRoles = []) {
  try {
    const user = await api.getCurrentUser();
    if (!user) {
      window.location.replace('login.html');
      return null;
    }
    if (allowedRoles.length > 0 && !allowedRoles.includes(user.role)) {
      alert(`Access Restricted: This page requires role: ${allowedRoles.join(', ')}`);
      window.location.replace('index.html');
      return null;
    }
    currentUser = user;
    updateNavbar(user);
    updateCartCount();
    updateWishlistCount();
    return user;
  } catch (err) {
    window.location.replace('login.html');
    return null;
  }
}

/**
 * Used on login.html to redirect if already logged in as non-guest
 */
async function redirectIfAuthenticated() {
  try {
    const user = await api.getCurrentUser();
    if (user && !user.is_demo) {
      if (user.role === 'SELLER') {
        window.location.replace('seller.html');
      } else if (user.role === 'ADMIN') {
        window.location.replace('admin.html');
      } else {
        window.location.replace('index.html');
      }
    }
  } catch {
    // Unauthenticated, stay on login page
  }
}

function updateNavbar(user) {
  const authNav = document.getElementById('nav-auth-container');
  if (!authNav) return;

  if (user) {
    let roleBadge = user.is_demo 
      ? '<span class="badge" style="background:var(--primary); font-size:0.75rem;">DEMO</span>'
      : (user.role === 'SELLER' 
          ? '<span class="badge" style="background:#0284c7">SELLER</span>' 
          : (user.role === 'ADMIN' ? '<span class="badge" style="background:#dc2626">ADMIN</span>' : ''));

    let roleLinks = '<li><a href="orders.html">📦 Orders</a></li>';
    if (user.role === 'SELLER') roleLinks = '<li><a href="seller.html">🏪 Seller</a></li>';
    if (user.role === 'ADMIN') roleLinks = '<li><a href="admin.html">🛡️ Admin</a></li>';

    authNav.innerHTML = `
      ${roleLinks}
      <li style="display:flex; align-items:center; gap:0.4rem; font-weight:600; font-size:0.9rem;">
        <span>👤 ${user.name}</span>
        ${roleBadge}
      </li>
      <li><button onclick="handleLogout()" class="btn btn-secondary btn-sm">Reset</button></li>
    `;
  } else {
    authNav.innerHTML = `
      <li><a href="login.html" class="btn btn-secondary btn-sm">Login</a></li>
      <li><a href="register.html" class="btn btn-primary btn-sm">Register</a></li>
    `;
  }
}

async function updateCartCount() {
  const badge = document.getElementById('cart-badge');
  if (!badge) return;
  try {
    const cart = await api.getCart();
    badge.textContent = cart.total_items || 0;
  } catch {
    badge.textContent = 0;
  }
}

async function updateWishlistCount() {
  const badge = document.getElementById('wishlist-badge');
  if (!badge) return;
  try {
    const list = await api.getWishlist();
    badge.textContent = list.length || 0;
  } catch {
    badge.textContent = 0;
  }
}

async function handleLogout() {
  try {
    await api.logout();
    window.location.href = 'index.html';
  } catch (err) {
    alert('Reset failed: ' + err.message);
  }
}

document.addEventListener('DOMContentLoaded', () => {
  if (!window.skipAutoAuthCheck) {
    checkAuthState();
  }
});
