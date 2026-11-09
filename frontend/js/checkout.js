// Dhivagar Mart - Checkout Logic
let currentCart = null;

function togglePaymentInputs(mode) {
  const upiDiv = document.getElementById('payment-input-upi');
  const cardDiv = document.getElementById('payment-input-card');
  if (upiDiv) upiDiv.style.display = (mode === 'UPI') ? 'block' : 'none';
  if (cardDiv) cardDiv.style.display = (mode === 'CARD') ? 'block' : 'none';
}

async function initCheckout() {
  const summaryBox = document.getElementById('checkout-summary');
  if (!summaryBox) return;

  try {
    currentCart = await api.getCart();
    if (!currentCart.items || currentCart.items.length === 0) {
      alert('Your cart is empty. Please add some fresh groceries first!');
      window.location.href = 'products.html';
      return;
    }

    summaryBox.innerHTML = `
      <h3 style="margin-bottom: 1rem; color: var(--secondary);">Order Breakdown (${currentCart.total_items} items)</h3>
      <div style="max-height: 260px; overflow-y: auto; margin-bottom: 1rem;">
        ${currentCart.items.map(i => `
          <div style="display: flex; justify-content: space-between; margin-bottom: 0.75rem; font-size: 0.9rem; border-bottom: 1px dashed var(--border-color); padding-bottom: 0.5rem;">
            <div>
              <span style="font-weight: 600;">${i.product_name}</span>
              <div style="font-size: 0.8rem; color: var(--text-muted);">${i.quantity} × ${formatINR(i.unit_price_cents)}</div>
            </div>
            <strong>${formatINR(i.subtotal_cents)}</strong>
          </div>
        `).join('')}
      </div>
      <div style="display: flex; justify-content: space-between; margin-bottom: 0.5rem; font-size: 0.9rem; color: var(--text-muted);">
        <span>Delivery:</span>
        <span style="color: var(--success); font-weight: 700;">FREE Express</span>
      </div>
      <div style="display: flex; justify-content: space-between; margin-bottom: 0.5rem; font-size: 0.9rem; color: var(--text-muted);">
        <span>Fresh Quality Guarantee:</span>
        <span style="color: var(--success); font-weight: 700;">Included</span>
      </div>
      <hr style="border: 0; border-top: 1px solid var(--border-color); margin-bottom: 1rem;">
      <div style="display: flex; justify-content: space-between; font-size: 1.25rem; font-weight: 800;">
        <span>Grand Total:</span>
        <span style="color: var(--primary);">${formatINR(currentCart.total_cents)}</span>
      </div>
    `;
  } catch (err) {
    summaryBox.innerHTML = `<div style="color: red;">Error: ${err.message}</div>`;
  }
}

async function handlePlaceOrder(e) {
  e.preventDefault();

  const fullName = document.getElementById('full-name')?.value.trim() || 'Demo Guest';
  const phone = document.getElementById('phone-number')?.value.trim() || '+91 98765 43210';
  const street = document.getElementById('shipping-address')?.value.trim() || 'Demo Street';
  const city = document.getElementById('city')?.value.trim() || 'Chennai';
  const state = document.getElementById('state')?.value.trim() || 'Tamil Nadu';
  const pincode = document.getElementById('pincode')?.value.trim() || '600001';

  const selectedPaymentRadio = document.querySelector('input[name="payment-method"]:checked');
  const paymentMethod = selectedPaymentRadio ? selectedPaymentRadio.value : 'CASH_ON_DELIVERY';

  const submitBtn = document.getElementById('place-order-btn');
  if (submitBtn) {
    submitBtn.disabled = true;
    submitBtn.textContent = 'Processing Demo Order...';
  }

  try {
    const payload = {
      full_name: fullName,
      phone: phone,
      shipping_address: street,
      city: city,
      state: state,
      pincode: pincode,
      payment_method: paymentMethod
    };

    const order = await api.checkout(payload);

    if (typeof updateCartCount === 'function') updateCartCount();

    // Show Confirmation Modal
    const modal = document.getElementById('order-confirm-modal');
    if (modal) {
      document.getElementById('confirm-order-id').textContent = `#DHV-${order.id}`;
      document.getElementById('confirm-payment-method').textContent = order.payment_method || paymentMethod;
      const statusBadge = document.getElementById('confirm-payment-status');
      if (statusBadge) {
        statusBadge.textContent = order.payment_status || 'PAID (Demo)';
        statusBadge.style.background = '#10b981';
      }
      document.getElementById('confirm-address').textContent = order.delivery_address || `${street}, ${city}`;
      document.getElementById('confirm-total').textContent = formatINR(order.total_amount_cents);
      modal.style.display = 'flex';
    } else {
      alert(`Order #DHV-${order.id} placed successfully!`);
      window.location.href = 'orders.html';
    }
  } catch (err) {
    alert('Order checkout failed: ' + err.message);
    if (submitBtn) {
      submitBtn.disabled = false;
      submitBtn.textContent = 'Place Demo Order Now';
    }
  }
}
