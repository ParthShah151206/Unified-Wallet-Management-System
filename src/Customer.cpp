
#include "../include/Customer.h"
#include "../include/BankWallet.h"
#include "../include/CryptoWallet.h"
#include "../include/CBDCWallet.h"
#include "../include/FileManager.h"
#include "../include/Input.h"
#include "../include/AssetTypeManager.h"

#include <iostream>
#include <cmath>
#include <limits>
#include <string>
#include <iomanip>

using namespace std;

// Constructor
Customer::Customer(
    int id,
    const string& n,
    const string& e,
    const string& p
) : User(id, n, e, p) {}

// Destructor
Customer::~Customer() {
    for (Wallet* wallet : wallets) {
        delete wallet;
    }
}

// Find wallet using wallet ID
Wallet* Customer::findWallet(int walletID) const {
    for (Wallet* wallet : wallets) {
        if (wallet && wallet->getWalletID() == walletID) {
            return wallet;
        }
    }

    return nullptr;
}

// Create wallet
void Customer::createWallet(int choice, int walletID, double balance) {

    if (walletID <= 0) {
        cout << "Wallet ID must be positive!\n";
        return;
    }

    if (findWallet(walletID) != nullptr) {
        cout << "Wallet ID already exists!\n";
        return;
    }

    if (!isfinite(balance) || balance < 0) {
        cout << "Invalid initial balance!\n";
        return;
    }

    string assetCode;

    switch (choice) {
        case 1:
            assetCode = "BANK";
            break;

        case 2:
            assetCode = "CRYPTO";
            break;

        case 3:
            assetCode = "CBDC";
            break;

        default:
            cout << "Invalid wallet type!\n";
            return;
    }

    // Check whether Admin has enabled this wallet type
    if (!AssetTypeManager::isEnabled(assetCode)) {
        cout << "\nSorry! This wallet type is currently disabled by Admin.\n";
        cout << "Please select another wallet type.\n";
        return;
    }

    Wallet* newWallet = nullptr;

    switch (choice) {
        case 1:
            newWallet = new BankWallet(walletID, userID, balance, "INR");
            break;

        case 2:
            newWallet = new CryptoWallet(walletID, userID, balance, "BTC");
            break;

        case 3:
            newWallet = new CBDCWallet(walletID, userID, balance, "INR");
            break;
    }

    if (newWallet == nullptr) {
        cout << "Wallet creation failed!\n";
        return;
    }

    wallets.push_back(newWallet);

    cout << "\nWallet created successfully!\n";
}

// View wallets
void Customer::viewWallets() const {

    if (wallets.empty()) {
        cout << "No wallets available.\n";
        return;
    }

    for (const Wallet* wallet : wallets) {
        if (wallet != nullptr) {
            wallet->displayWallet();
        }
    }
}

// Deposit money
void Customer::depositMoney(int walletID, double amount) {

    Wallet* wallet = findWallet(walletID);

    if (walletID <= 0 || wallet == nullptr ||
        !isfinite(amount) || amount <= 0) {

        cout << "Invalid wallet or amount.\n";
        return;
    }

    // Generate transaction ID before changing balance
    const int id = FileManager::generateTransactionID();

    if (id <= 0) {
        cout << "Could not generate transaction ID. Deposit cancelled.\n";
        return;
    }

    if (wallet->deposit(amount)) {

        transactions.emplace_back(
            id, "DEPOSIT", -1, walletID, amount, "SUCCESS"
        );

        cout << "\nDeposit successful!\n";
    }
    else {

        transactions.emplace_back(
            id, "DEPOSIT", -1, walletID, amount, "FAILED"
        );

        cout << "\nDeposit failed!\n";
    }
}

// Withdraw money
void Customer::withdrawMoney(int walletID, double amount) {

    Wallet* wallet = findWallet(walletID);

    if (walletID <= 0 || wallet == nullptr ||
        !isfinite(amount) || amount <= 0) {

        cout << "Invalid wallet or amount.\n";
        return;
    }

    const int id = FileManager::generateTransactionID();

    if (id <= 0) {
        cout << "Could not generate transaction ID. Withdrawal cancelled.\n";
        return;
    }

    if (wallet->withdraw(amount)) {

        transactions.emplace_back(
            id, "WITHDRAWAL", walletID, -1, amount, "SUCCESS"
        );

        cout << "\nWithdrawal successful!\n";
    }
    else {

        transactions.emplace_back(
            id, "WITHDRAWAL", walletID, -1, amount, "FAILED"
        );

        cout << "\nWithdrawal failed! Insufficient balance or invalid operation.\n";
    }
}

// Transfer money between customer's wallets
void Customer::transferMoney(
    int senderID,
    int receiverID,
    double amount
) {

    Wallet* sender = findWallet(senderID);
    Wallet* receiver = findWallet(receiverID);

    if (senderID <= 0 || receiverID <= 0 ||
        sender == nullptr || receiver == nullptr ||
        senderID == receiverID ||
        !isfinite(amount) || amount <= 0) {

        cout << "Invalid transfer details. Both wallets must belong to this customer.\n";
        return;
    }

    if (sender->getCurrency() != receiver->getCurrency()) {
        cout << "Cannot transfer between different currencies.\n";
        return;
    }

    const int id = FileManager::generateTransactionID();

    if (id <= 0) {
        cout << "Could not generate transaction ID. Transfer cancelled.\n";
        return;
    }

    // Withdraw from sender
    if (!sender->withdraw(amount)) {

        transactions.emplace_back(
            id, "TRANSFER", senderID, receiverID, amount, "FAILED"
        );

        cout << "\nTransfer failed! Insufficient balance.\n";
        return;
    }

    // Deposit into receiver
    if (!receiver->deposit(amount)) {

        // Restore sender's balance
        if (!sender->deposit(amount)) {
            cerr << "CRITICAL: Transfer rollback failed. Reconciliation required.\n";
        }

        transactions.emplace_back(
            id, "TRANSFER", senderID, receiverID, amount, "FAILED"
        );

        cout << "\nTransfer failed! Receiver could not accept the amount.\n";
        return;
    }

    transactions.emplace_back(
        id, "TRANSFER", senderID, receiverID, amount, "SUCCESS"
    );

    cout << "\nTransfer successful!\n";
}

// View transaction history
void Customer::viewTransactionHistory() const {

    if (transactions.empty()) {
        cout << "\nNo transactions found!\n";
        return;
    }

    cout << "\n";
    cout << "====================================================================================================\n";
    cout << "                                  CUSTOMER TRANSACTION HISTORY\n";
    cout << "====================================================================================================\n";

    cout << left
         << setw(10) << "Txn ID"
         << setw(15) << "Type"
         << setw(12) << "Sender"
         << setw(12) << "Receiver"
         << setw(15) << "Amount"
         << setw(15) << "Status"
         << "Timestamp\n";

    cout << "----------------------------------------------------------------------------------------------------\n";

    for (const Transaction& transaction : transactions) {

        cout << left
             << setw(10) << transaction.getTransactionID()
             << setw(15) << transaction.getType()
             << setw(12) << transaction.getSenderWalletID();

        if (transaction.getReceiverWalletID() == -1) {
            cout << setw(12) << "N/A";
        }
        else {
            cout << setw(12) << transaction.getReceiverWalletID();
        }

        cout << setw(15) << fixed << setprecision(2)
             << transaction.getAmount()
             << setw(15) << transaction.getStatus()
             << transaction.getTimestamp() << "\n";
    }

    cout << "----------------------------------------------------------------------------------------------------\n";
    cout << "Total Transactions: " << transactions.size() << "\n";
    cout << "====================================================================================================\n";
}

// Customer dashboard
void Customer::displayDashboard() {

    int choice;

    while (true) {

        cout << "\n";
        cout << "============================================\n";
        cout << "             CUSTOMER DASHBOARD             \n";
        cout << "============================================\n";

        displayProfile();

        cout << "\n--------------- MENU OPTIONS ---------------\n";
        cout << "1. Create Wallet\n";
        cout << "2. View Wallets\n";
        cout << "3. Deposit Money\n";
        cout << "4. Withdraw Money\n";
        cout << "5. Transfer Between My Wallets\n";
        cout << "6. Transaction History\n";
        cout << "7. Logout\n";
        cout << "--------------------------------------------\n";

        cout << "Enter your choice (1-7): ";

        if (!(cin >> choice)) {

            cout << "\nInvalid input! Please enter a number.\n";

            cin.clear();

            cin.ignore(
                numeric_limits<streamsize>::max(), '\n'
            );

            continue;
        }

        cout << "\n";

        switch (choice) {

            case 1: {
                int type, id;
                double balance;

                cout << "========== CREATE WALLET ==========\n";
                cout << "1. Bank Wallet\n";
                cout << "2. Crypto Wallet\n";
                cout << "3. CBDC Wallet\n";

                if (!readInt("Select wallet type: ", type))
                    break;

                if (!readInt("Enter Wallet ID: ", id))
                    break;

                if (!readPositiveAmount("Enter Initial Balance: ", balance, true))
                    break;

                createWallet(type, id, balance);
                break;
            }

            case 2: {
                cout << "========== YOUR WALLETS ==========\n";
                viewWallets();
                break;
            }

            case 3: {
                int id;
                double amount;

                cout << "========== DEPOSIT MONEY ==========\n";

                if (!readInt("Enter Wallet ID: ", id))
                    break;

                if (!readPositiveAmount("Enter Amount: ", amount))
                    break;

                depositMoney(id, amount);
                break;
            }

            case 4: {
                int id;
                double amount;

                cout << "========== WITHDRAW MONEY ==========\n";

                if (!readInt("Enter Wallet ID: ", id))
                    break;

                if (!readPositiveAmount("Enter Amount: ", amount))
                    break;

                withdrawMoney(id, amount);
                break;
            }

            case 5: {
                int from, to;
                double amount;

                cout << "========== TRANSFER MONEY ==========\n";

                if (!readInt("Enter Sender Wallet ID: ", from))
                    break;

                if (!readInt("Enter Receiver Wallet ID: ", to))
                    break;

                if (!readPositiveAmount("Enter Amount: ", amount))
                    break;

                transferMoney(from, to, amount);
                break;
            }

            case 6: {
                cout << "========== TRANSACTION HISTORY ==========\n";
                viewTransactionHistory();
                break;
            }

            case 7: {
                cout << "\nLogging out...\n";
                return;
            }

            default: {
                cout << "Invalid choice! Please select between 1 and 7.\n";
                break;
            }
        }

        // Save customer data after every operation
        const string path =
            "data/customer_" + to_string(userID) + ".txt";

        if (!FileManager::saveCustomer(*this, path)) {
            cerr << "\nWARNING: Unable to save customer data!\n";
        }

        cout << "\nPress Enter to return to the dashboard...";

        cin.ignore(
            numeric_limits<streamsize>::max(), '\n'
        );

        cin.get();
    }
}