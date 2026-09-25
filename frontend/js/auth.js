// Dhivagar Mart - Auth State & Navbar Management
let currentUser = null;

async function checkAuthState() {
  try {
    currentUser = await api.getCurrentUser();
    updateNavbar(currentUser);
    updateCartCount();
    return currentUser;
  } catch (err) {
    currentUser = null;
    updateNavbar(null);
    return null;
  }
}

/**
 * Enforces authentication on protected pages.
 * If user is not logged in, redirects immediately to login.html.
 * If allowedRoles is provided and user role does not match, redirects to index.html.
 */
async function requireAuth(allowedRoles = []) {
  try {
    const user = await api.getCurrentUser();
    if (!user) {
      window.location.replace('login.html');
      return null;
    }
    if (allowedRoles.length > 0 && !allowedRoles.includes(user.role)) {
      alert(`Access Restricted: This page requires one of the following roles: ${allowedRoles.join(', ')}`);
      window.location.replace('index.html');
      return null;
    }
    currentUser = user;
    updateNavbar(user);
    updateCartCount();
    return user;
  } catch (err) {
    // Session expired or unauthenticated -> redirect to login
    window.location.replace('login.html');
    return null;
  }
}

/**
 * Used on login.html to prevent already-logged-in users from seeing login again.
 */
async function redirectIfAuthenticated() {
  try {
    const user = await api.getCurrentUser();
    if (user) {
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
    let roleBadge = '';
    let roleLinks = '';

    if (user.role === 'SELLER') {
      roleBadge = '<span class="badge" style="background:#0284c7">SELLER</span>';
      roleLinks = '<li><a href="seller.html">🏪 Seller Portal</a></li>';
    } else if (user.role === 'ADMIN') {
      roleBadge = '<span class="badge" style="background:#dc2626">ADMIN</span>';
      roleLinks = '<li><a href="admin.html">🛡️ Admin Center</a></li>';
    } else {
      roleLinks = '<li><a href="orders.html">📦 My Orders</a></li>';
    }

    authNav.innerHTML = `
      ${roleLinks}
      <li style="display:flex; align-items:center; gap:0.4rem; font-weight:600; font-size:0.9rem;">
        <span>👤 ${user.name}</span>
        ${roleBadge}
      </li>
      <li><button onclick="handleLogout()" class="btn btn-secondary btn-sm">Logout</button></li>
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

async function handleLogout() {
  try {
    await api.logout();
    window.location.href = 'login.html';
  } catch (err) {
    alert('Logout failed: ' + err.message);
  }
}

document.addEventListener('DOMContentLoaded', () => {
  // If the page doesn't explicitly invoke requireAuth or redirectIfAuthenticated,
  // we still populate auth status in navbar
  if (!window.skipAutoAuthCheck) {
    checkAuthState();
  }
});
