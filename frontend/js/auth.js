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

function updateNavbar(user) {
  const authNav = document.getElementById('nav-auth-container');
  if (!authNav) return;

  if (user) {
    let roleBadge = '';
    let roleLinks = '';

    if (user.role === 'SELLER') {
      roleBadge = '<span class="badge" style="background:#0284c7">Seller</span>';
      roleLinks = '<li><a href="seller.html">Seller Portal</a></li>';
    } else if (user.role === 'ADMIN') {
      roleBadge = '<span class="badge" style="background:#dc2626">Admin</span>';
      roleLinks = '<li><a href="admin.html">Admin Center</a></li>';
    } else {
      roleLinks = '<li><a href="orders.html">My Orders</a></li>';
    }

    authNav.innerHTML = `
      ${roleLinks}
      <li style="display:flex; align-items:center; gap:0.5rem; font-weight:600;">
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
  checkAuthState();
});
