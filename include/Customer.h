
#ifndef UWMS_CUSTOMER_H
#define UWMS_CUSTOMER_H

#include "User.h"
#include "Wallet.h"
#include "Transaction.h"

#include <vector>
#include <string>

class FileManager;

class Customer : public User {
private:
    std::vector<Wallet*> wallets;
    std::vector<Transaction> transactions;

    Wallet* findWallet(int walletID) const;

public:
    Customer(
        int id,
        const std::string& n,
        const std::string& e,
        const std::string& p
    );

    ~Customer() override;

    // Prevent copying and moving because Customer owns raw Wallet pointers.
    Customer(const Customer&) = delete;
    Customer& operator=(const Customer&) = delete;

    Customer(Customer&&) = delete;
    Customer& operator=(Customer&&) = delete;

    // Wallet operations
    void createWallet(int choice, int walletID, double balance);
    void viewWallets() const;

    // Transaction operations
    void depositMoney(int walletID, double amount);
    void withdrawMoney(int walletID, double amount);
    void transferMoney(int senderID, int receiverID, double amount);
    void viewTransactionHistory() const;

    // Dashboard
    void displayDashboard() override;

    friend class FileManager;
};

#endif
