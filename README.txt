# 💳 Unified Wallet Management System (UWMS)


## 📑 Table of Contents

- [Overview](#-overview)
- [Key Features](#-key-features)
- [System Architecture](#-system-architecture)
- [Object-Oriented Design](#-object-oriented-programming)
- [Class Structure](#-class-structure)
- [Project Structure](#-project-structure)
- [Technology Stack](#-technology-stack)
- [Data Management](#-data-management)
- [Getting Started](#-getting-started)
- [Application Workflow](#-application-workflow)
- [Future Enhancements](#-future-enhancements)
- [Contributing](#-contributing)
- [Author](#-author)

---

## 📌 Overview

The **Unified Wallet Management System (UWMS)** is a C++ application developed to demonstrate the practical implementation of Object-Oriented Programming principles through a digital wallet management platform.

The application provides a centralized environment where customers can manage different wallet types, perform financial operations, and monitor transaction histories.

It follows a role-based architecture with two primary user categories:

- **Customer:** Manages personal wallets and transactions.
- **Admin:** Oversees customers, wallets, transactions, and system-level operations.

The system currently uses file handling for persistent storage, allowing application data to be maintained between executions without requiring an external database server.

### 🎯 Project Objectives

- Apply real-world OOP concepts in a practical application.
- Implement inheritance and runtime polymorphism.
- Manage multiple wallet types through a common interface.
- Maintain transaction records and wallet balances.
- Implement role-based access and administrative operations.
- Demonstrate file handling and persistent data storage.

---

## 🚀 Key Features

### 👤 Customer Module

| Feature | Description |
|---|---|
| Registration | Create a customer account |
| Authentication | Log in using account credentials |
| Wallet Management | Create and manage different wallet types |
| Deposit | Add funds to a wallet |
| Withdrawal | Withdraw available funds |
| Fund Transfer | Transfer funds between wallets |
| Transaction History | View recorded transactions |
| Wallet Details | View wallet balances and information |

### 🛡️ Admin Module

| Feature | Description |
|---|---|
| Customer Management | View registered customers |
| Wallet Management | View system wallets |
| Transaction Monitoring | View transaction records |
| Asset Management | Manage supported asset types |
| System Reports | Display system-level information |

### 💰 Supported Wallet Types

The system uses an inheritance-based wallet architecture.

| Wallet | Purpose |
|---|---|
| Bank Wallet | Manages conventional bank-style balances |
| Crypto Wallet | Represents cryptocurrency holdings |
| CBDC Wallet | Represents Central Bank Digital Currency balances |

---

## 🏗️ System Architecture

The application is organized into multiple layers of responsibility.

```mermaid
flowchart TD
    A["UWMS Application"] --> B["Authentication"]
    B --> C{"User Role"}
    C --> D["Customer Dashboard"]
    C --> E["Admin Dashboard"]

    D --> F["Wallet Management"]
    D --> G["Transaction Management"]

    F --> H["Bank Wallet"]
    F --> I["Crypto Wallet"]
    F --> J["CBDC Wallet"]

    E --> K["Admin Service"]
    K --> L["Customer Management"]
    K --> M["System Monitoring"]

    G --> N["File Manager"]
    F --> N
    K --> N

    N --> O["Text File Storage"]
```

### Architectural Responsibilities

| Component | Responsibility |
|---|---|
| Authentication | Handles user login and verification |
| Customer | Manages customer-specific operations |
| Admin | Provides administrative functionality |
| Wallet hierarchy | Implements wallet-specific operations |
| Transaction | Represents financial transactions |
| AdminService | Handles administrative services |
| FileManager | Handles persistent data storage |

---

## 🧠 Object-Oriented Programming

One of the primary goals of this project is to demonstrate the four fundamental pillars of OOP.

### 1. Encapsulation

Encapsulation is implemented by keeping class data members private or protected and exposing controlled public methods.

**Example:**

```cpp
class Wallet {
protected:
    int walletID;
    double balance;

public:
    bool deposit(double amount);
    bool withdraw(double amount);
};
```

The wallet balance is modified through controlled operations rather than unrestricted access.

### 2. Abstraction

Abstract classes define common interfaces while leaving implementation details to derived classes.

**Example:**

```cpp
class Wallet {
public:
    virtual bool deposit(double amount) = 0;
    virtual bool withdraw(double amount) = 0;

    virtual void displayWallet() const = 0;

    virtual ~Wallet() = default;
};
```

The base class defines what a wallet should do, while individual wallet classes define how those operations work.

### 3. Inheritance

The application uses hierarchical inheritance.

**User hierarchy:**

```mermaid
classDiagram
    User <|-- Customer
    User <|-- Admin

    class User {
        <<abstract>>
        +displayDashboard()
    }

    class Customer {
        +manageWallets()
        +viewTransactions()
    }

    class Admin {
        +manageCustomers()
        +viewReports()
    }
```

**Wallet hierarchy:**

```mermaid
classDiagram
    Wallet <|-- BankWallet
    Wallet <|-- CryptoWallet
    Wallet <|-- CBDCWallet

    class Wallet {
        <<abstract>>
        +deposit()
        +withdraw()
        +displayWallet()
    }

    class BankWallet
    class CryptoWallet
    class CBDCWallet
```

### 4. Polymorphism

Runtime polymorphism is implemented using virtual functions and base-class pointers.

**Example:**

```cpp
vector<Wallet*> wallets;

wallets.push_back(new BankWallet(...));
wallets.push_back(new CryptoWallet(...));
wallets.push_back(new CBDCWallet(...));

for (Wallet* wallet : wallets) {
    wallet->displayWallet();
}
```

Although all objects are accessed through `Wallet*`, the appropriate derived-class implementation is executed at runtime.

This makes the system extensible because new wallet types can be introduced without redesigning the entire wallet-management interface.

---

## 🧩 Class Structure

The core architecture consists of approximately **10 primary classes**, with additional supporting classes depending on the current implementation.

| Class | Responsibility |
|---|---|
| `User` | Abstract base class for users |
| `Customer` | Customer operations and wallet management |
| `Admin` | Administrative operations |
| `Wallet` | Abstract base class for wallets |
| `BankWallet` | Bank wallet implementation |
| `CryptoWallet` | Cryptocurrency wallet implementation |
| `CBDCWallet` | CBDC wallet implementation |
| `Transaction` | Stores transaction information |
| `FileManager` | Handles file operations |
| `AdminService` | Provides administrative services |

### Important Relationships

- `Customer` IS-A `User`.
- `Admin` IS-A `User`.
- `BankWallet` IS-A `Wallet`.
- `CryptoWallet` IS-A `Wallet`.
- `CBDCWallet` IS-A `Wallet`.
- `Customer` HAS-A collection of wallets.
- `Transaction` represents an operation involving wallet identifiers.

---

## 📁 Project Structure

```text
Unified-Wallet-Management-System/
│
├── include/
│   ├── User.h
│   ├── Customer.h
│   ├── Admin.h
│   ├── Wallet.h
│   ├── BankWallet.h
│   ├── CryptoWallet.h
│   ├── CBDCWallet.h
│   ├── Transaction.h
│   ├── FileManager.h
│   └── Input.h
│
├── src/
│   ├── main.cpp
│   ├── User.cpp
│   ├── Customer.cpp
│   ├── Admin.cpp
│   ├── Wallet.cpp
│   ├── BankWallet.cpp
│   ├── CryptoWallet.cpp
│   ├── CBDCWallet.cpp
│   ├── Transaction.cpp
│   └── FileManager.cpp
│
├── data/
│   └── *.txt
│
├── .gitignore
└── README.md
```

*The structure above illustrates the core organization. Refer to the actual repository for the complete list of files.*

---

## 🛠️ Technology Stack

| Technology | Usage |
|---|---|
| C++17 | Core programming language |
| OOP | Application architecture |
| STL | Vectors, strings, and data management |
| File Handling | Persistent application data |
| VS Code | Development environment |
| MinGW / G++ | Compilation |
| Git & GitHub | Version control and project hosting |

---

## 💾 Data Management

The application currently uses text-file-based storage.

The `FileManager` class is responsible for reading and writing application data.

### Persistence Responsibilities

- User information storage
- Customer wallet information
- Transaction records
- Transaction counter persistence
- Loading previously stored application data

### Why File Handling?

File handling provides a lightweight persistence mechanism without requiring a database server.

It also demonstrates the use of C++ file streams, including:

```cpp
#include <fstream>

ifstream inputFile("data/users.txt");
ofstream outputFile("data/users.txt");
```

**Note:** The current implementation is file-based. Database integration is a potential future enhancement.

---

## ⚙️ Getting Started

### Prerequisites

Make sure you have:

- Windows 10/11 or a compatible environment
- Visual Studio Code
- MinGW-w64 or another C++17-compatible compiler
- Git

### 1. Clone the Repository

```bash
git clone https://github.com/ParthShah151206/Unified-Wallet-Management-System
```

### 2. Navigate to the Project

```bash
cd Unified-Wallet-Management-System
```

### 3. Compile the Application

If your source files are organized under `src/` and headers under `include/`, a typical G++ command is:

```bash
g++ -std=c++17 -Iinclude src/*.cpp -o UWMS.exe
```

If your project has additional source files or a different entry point, adjust the command accordingly.

### 4. Run the Application

```bash
./UWMS.exe
```

On Windows PowerShell, you can also run:

```powershell
.\UWMS.exe
```

### 5. Start Using UWMS

Follow the on-screen menu to register or log in and access the available customer or administrator operations.

---

## 🔄 Application Workflow

```mermaid
flowchart TD
    A["Start Application"] --> B["Load Stored Data"]
    B --> C["Login / Registration"]
    C --> D{"Authentication"}
    D -->|Customer| E["Customer Dashboard"]
    D -->|Admin| F["Admin Dashboard"]
    D -->|Invalid| C

    E --> G["Wallet Operations"]
    E --> H["Transaction History"]

    G --> I["Deposit / Withdraw / Transfer"]
    I --> J["Update Wallet Data"]
    J --> K["Save Data"]

    F --> L["View Customers / Wallets"]
    F --> M["Reports / Asset Management"]

    K --> N["Logout"]
    H --> N
    L --> N
    M --> N
    N --> O["Exit"]
```

## 🔮 Future Enhancements

Potential improvements planned for future versions:

- [ ] MySQL database integration
- [ ] Graphical user interface
- [ ] REST API and backend integration
- [ ] Secure password hashing with a dedicated password-hashing algorithm
- [ ] Improved transaction validation and atomic transfers
- [ ] Exchange-rate integration for multiple currencies
- [ ] Exportable transaction reports
- [ ] Unit and integration testing
- [ ] Improved logging and error handling

---

## 🔐 Security Considerations

This project is an educational implementation, not a production-ready financial application.

Production deployment would require:

- Dedicated salted password hashing.
- Secure credential management.
- Strong authentication and authorization.
- Transaction integrity and concurrency control.
- Input validation and audit logging.
- Secure database storage and backups.

Do not use real banking credentials or personal financial information in the demo data.

---

## 🤝 Contributing

Contributions and suggestions are welcome.

To contribute:

1. Fork the repository.
2. Create a new feature branch.
3. Implement your changes.
4. Test the application.
5. Submit a pull request.

```bash
git checkout -b feature/your-feature
git add .
git commit -m "Add: your feature"
git push origin feature/your-feature
```

---

## 👨‍💻 Author

**Parth Shah**

C++ | Object-Oriented Programming | Data Structures & Algorithms

Developed as an academic project to explore practical applications of C++ and Object-Oriented Programming.

---

<p align="center">
  **Unified Wallet Management System**
  
  *One Platform. Multiple Wallets. Unified Management.*
</p>