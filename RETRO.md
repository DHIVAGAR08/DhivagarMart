# Sprint Retrospectives (RETRO.md)

### Sprint 1 (Jul 27 – Aug 2)
- **What worked**: Project scaffolding, CMakeLists.txt setup, vcpkg dependency management, and PostgreSQL database initialization.
- **What didn't**: Initial Windows non-admin environment required localized toolchain configuration.
- **Action for next sprint**: Standardized all paths and environment variables using `.env` configurations.

---

### Sprint 2 (Aug 3 – Aug 16)
- **What worked**: Argon2id password hashing via libsodium and core session management using Drogon HttpFilter.
- **What didn't**: Testing session invalidation across parallel requests required thread synchronization in mocks.
- **Action for next sprint**: Added isolated session fixture helpers in the test suite.

---

### Sprint 3 (Aug 17 – Aug 30)
- **What worked**: Cart and transactional checkout implementation with rollback guarantees on out-of-stock events.
- **What didn't**: PostgreSQL connection pooling needed explicit RAII wrappers to avoid connection leakage during exceptions.
- **Action for next sprint**: Centralized all connection access through `DatabasePlugin::GetConnection()` RAII handle.

---

### Sprint 4 (Aug 31 – Sep 13)
- **What worked**: Seller and Admin dashboards with verified-purchase reviews and order status transition state machines.
- **What didn't**: Frontend vanilla JavaScript fetch error handling had repetitive code.
- **Action for next sprint**: Consolidated fetch handling in a shared `frontend/js/api.js` client module.

---

### Sprint 5 (Sep 14 – Sep 24)
- **What worked**: AI Chatbot integration with Strategy pattern, Gemini API, fallback mock provider, rate-limiting, and memory caching.
- **What didn't**: Long LLM network latencies risked stalling request threads without explicit timeouts.
- **Action for next sprint**: Configured 5-second outbound HTTP socket timeout and automated degraded FAQ responses.
