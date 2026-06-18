# Bank Management System - Extension 2 (C++)

An advanced C++ Command-Line Interface (CLI) Bank Management System that incorporates secure user authentication, fine-grained bitwise runtime permissions, transactional logging, and structured client data storage using local text databases.

## 🚀 Features

### 🔐 User & Security Management
* **Secure Authentication System:** Multi-attempt login screen verifying credentials against a stored database.
* **Bitwise Permissions System:** Uses binary flags (integers acting as bitmasks) to dynamically restrict or grant access to specific system menus:
  * Show Client List (`pListClients = 1`)
  * Add New Client (`pAddNewClient = 2`)
  * Delete Client (`pDeleteClient = 4`)
  * Update Client Info (`pUpdateClients = 8`)
  * Find Client (`pFindClient = 16`)
  * Transactions Menu (`pTranactions = 32`)
  * Manage Users Menu (`pManageUsers = 64`)
  * Super Admin Access (`eAll = -1`)

### 👥 Client Operations
* **Structured Records:** Client fields include Account Number, Pin Code, Full Name, Phone Number, and Account Balance.
* **Robust Redundancy Checks:** Real-time data verification to prevent duplicate account creation.
* **Safe Deletion & Editing:** Implements a logical "Mark-for-Delete" flag before updating data to prevent accidental loss and corruption during file stream processes.

### 💰 Transaction System
* **Core Functions:** Supports deposit (`+`) and withdrawal (`-`) actions.
* **Overdraft Prevention:** Checks available balances at runtime to block transactions that exceed a client's current funds.
* **Global Asset Tracking:** Displays total balances across the entire bank registry.

---

## 🛠️ Tech Stack & Concepts Used

* **Language:** C++11 or higher.
* **Object Mapping:** Custom `struct` data shapes mapped dynamically to record lines.
* **Data Persistence:** Flat-file text structures (`Clients.txt` and `Users.txt`) utilizing a custom delimiter (`#//#`).
* **Memory Containers:** `std::vector` combined with pass-by-reference memory optimizations to manipulate runtime dynamic data safely.
* **Input Parsing:** Cleans standard stream vulnerabilities via explicit `cin >> ws` and `getline` usage.

---

## 📂 File Architecture

* `Bank_Extension.cpp` - Core programmatic logic containing structural definitions and runtime execution loops.
* `Clients.txt` - Flat file data repository for bank clients.
* `Users.txt` - Registry for user credentials and mathematical bitwise permission levels.

---

## ⚙️ How It Works (Data Formatting)

Records are stored seamlessly within flat-file databases using the `#//#` separator token format:
```text
Admin#//#1234#//#-1
User01#//#9988#//#3
