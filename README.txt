UNIFIED WALLET MANAGEMENT SYSTEM - CORRECTED SOURCE

Build from the project root with MinGW / g++:
  g++ -std=c++17 -Wall -Wextra -Wpedantic -Iinclude main.cpp src/*.cpp -o UWMS.exe

Run:
  .\UWMS.exe

IMPORTANT SETUP:
1. Keep the data folder beside main.cpp and run the program from this project root.
2. The data folder is included. It must be writable by the program.
3. Customer records are stored in data/users.txt and data/customer_<id>.txt.
4. Transaction IDs are allocated using data/transaction_counter.txt.
5. Existing customer data files using the UWMS1 format can be loaded, but make a backup before replacing your original project files.

ADMIN PROTOTYPE LOGIN:
  Email: admin@uwms.com
  Password: admin123

SECURITY LIMITATIONS:
- This is an educational console prototype. The FNV-style hash used here is deterministic and is NOT a password-hashing algorithm suitable for production.
- Replace it with Argon2id, bcrypt, or scrypt plus unique salts before real use.
- Admin credentials are hardcoded for demonstration only.
- Money is represented with double for simplicity. A financial system should use integer minor units or a decimal fixed-point type.

FUNCTIONAL SCOPE / REMAINING FEATURES:
- Customer can create BANK, CRYPTO, and CBDC wallets; deposit, withdraw, transfer between their own wallets, and view history.
- The current transfer implementation is deliberately restricted to wallets owned by the same logged-in customer. Cross-customer transfers require a system-level transaction service and coordinated persistence across two customer files; they are not claimed as implemented.
- Admin dashboard menu options 1-5 are still placeholders. They are features not yet implemented, not hidden completed functionality.
- No live bank, crypto, or CBDC network is connected; balances are simulated.
