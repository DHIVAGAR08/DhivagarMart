// Dhivagar Mart - Checkout Logic
async function initCheckout() {
  const summaryBox = document.getElementById('checkout-summary');
  if (!summaryBox) return;

  try {
    const cart = await api.getCart();
    if (!cart.items || cart.items.length === 0) {
      alert('Your cart is empty. Please add items before checking out.');
      window.location.href = 'products.html';
      return;
    }

    summaryBox.innerHTML = `
      <h3 style="margin-bottom: 1rem;">Order Items (${cart.total_items})</h3>
      <div style="max-height: 240px; overflow-y: auto; margin-bottom: 1rem;">
        ${cart.items.map(i => `
          <div style="display: flex; justify-content: space-between; margin-bottom: 0.6rem; font-size: 0.9rem;">
            <span>${i.product_name} x ${i.quantity}</span>
            <strong>${i.subtotal_formatted}</strong>
          </div>
        `).join('')}
      </div>
      <hr style="border: 0; border-top: 1px solid var(--border-color); margin-bottom: 1rem;">
      <div style="display: flex; justify-content: space-between; font-size: 1.2rem; font-weight: 800;">
        <span>Grand Total:</span>
        <span style="color: var(--primary);">${cart.total_formatted}</span>
      </div>
    `;
  } catch (err) {
    if (err.status === 401) {
      window.location.href = 'login.html';
    } else {
      summaryBox.innerHTML = `<div style="color: red;">Error: ${err.message}</div>`;
    }
  }
}

async function handlePlaceOrder(e) {
  e.preventDefault();
  const addressInput = document.getElementById('shipping-address');
  const address = addressInput ? addressInput.value.trim() : '';

  if (!address) {
    alert('Please enter your complete delivery address.');
    return;
  }

  const submitBtn = document.getElementById('place-order-btn');
  if (submitBtn) {
    submitBtn.disabled = true;
    submitBtn.textContent = 'Processing Mock Payment...';
  }

  try {
    const result = await api.checkout({
      shipping_address: address,
      payment_method: 'MOCK_PAYMENT'
    });

    updateCartCount();
    alert(`Order #${result.id} placed successfully! Thank you for shopping with Dhivagar Mart.`);
    window.location.href = 'orders.html';
  } catch (err) {
    alert('Order checkout failed: ' + err.message);
    if (submitBtn) {
      submitBtn.disabled = false;
      submitBtn.textContent = 'Confirm Order & Pay (Mock)';
    }
  }
}
