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
 * Enforces authentication on user pages.
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
 * Used on login.html to redirect if already logged in
 */
async function redirectIfAuthenticated() {
  try {
    const user = await api.getCurrentUser();
    if (user && user.email !== 'customer@dhivagarmart.com') {
      window.location.replace('index.html');
    }
  } catch {
    // Stay on login page
  }
}

function updateNavbar(user) {
  const authNav = document.getElementById('nav-auth-container');
  if (!authNav) return;

  if (user && user.name) {
    authNav.innerHTML = `
      <li><a href="orders.html">📦 Orders</a></li>
      <li style="display:flex; align-items:center; gap:0.4rem; font-weight:600; font-size:0.9rem;">
        <span style="color:#ffffff;">👤 ${user.name}</span>
      </li>
      <li><button onclick="handleLogout()" class="btn btn-secondary btn-sm" style="background:rgba(255,255,255,0.1); color:#ffffff; border-color:rgba(255,255,255,0.25);">Sign Out</button></li>
    `;
  } else {
    authNav.innerHTML = `
      <li><a href="login.html" class="btn btn-secondary btn-sm" style="background:rgba(255,255,255,0.1); color:#ffffff; border-color:rgba(255,255,255,0.25);">Sign In</a></li>
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
    alert('Sign out: ' + err.message);
  }
}

document.addEventListener('DOMContentLoaded', () => {
  if (!window.skipAutoAuthCheck) {
    checkAuthState();
  }
});
