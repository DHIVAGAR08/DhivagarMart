// Dhivagar Mart - AI Shopping Assistant Widget
function toggleChat() {
  const panel = document.getElementById('chat-panel');
  if (!panel) return;
  panel.classList.toggle('open');
  if (panel.classList.contains('open')) {
    const input = document.getElementById('chat-input');
    if (input) input.focus();
  }
}

async function sendChatMessage(presetText = '') {
  const input = document.getElementById('chat-input');
  const message = presetText || (input ? input.value.trim() : '');
  if (!message) return;

  if (input && !presetText) input.value = '';

  appendChatBubble('user', message);

  // Typing indicator
  const typingId = appendChatBubble('bot', '<em>Searching store information...</em>');

  try {
    const data = await api.sendChatMessage(message);
    const typingBubble = document.getElementById(typingId);
    if (typingBubble) {
      typingBubble.innerHTML = data.reply + (data.cached ? ' <span style="font-size:0.7rem; color:#9ca3af;">(cached)</span>' : '');
    }
  } catch (err) {
    const typingBubble = document.getElementById(typingId);
    if (typingBubble) {
      typingBubble.textContent = err.message || 'I am currently unable to answer. Please try again.';
    }
  }

  const container = document.getElementById('chat-messages');
  if (container) container.scrollTop = container.scrollHeight;
}

function appendChatBubble(sender, text) {
  const container = document.getElementById('chat-messages');
  if (!container) return;

  const bubble = document.createElement('div');
  const id = 'msg-' + Date.now() + '-' + Math.random().toString(36).substr(2, 5);
  bubble.id = id;
  bubble.className = `chat-bubble ${sender}`;
  bubble.innerHTML = text;
  container.appendChild(bubble);
  container.scrollTop = container.scrollHeight;
  return id;
}

function handleChatKeyPress(e) {
  if (e.key === 'Enter') {
    sendChatMessage();
  }
}
