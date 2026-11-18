#include "../include/FileManager.h"
#include "../include/BankWallet.h"
#include "../include/CryptoWallet.h"
#include "../include/CBDCWallet.h"
#include <fstream>
#include <iomanip>
#include <cmath>
#include <set>
#include <limits>
#include <cstdio>
#include <iostream>
#include <windows.h>

namespace {
void deleteWallets(std::vector<Wallet*>& wallets) {
    for (Wallet* wallet : wallets) delete wallet;
    wallets.clear();
}
bool validTransactionType(const std::string& t) {
    return t == "DEPOSIT" || t == "WITHDRAWAL" || t == "TRANSFER";
}
bool validStatus(const std::string& s) {
    return s == "SUCCESS" || s == "FAILED";
}
}

bool FileManager::saveCustomer(const Customer& customer, const std::string& filename) {
    if (filename.empty() || customer.userID <= 0) return false;
    const std::string tempName = filename + ".tmp";
    std::ofstream file(tempName, std::ios::trunc);
    if (!file) return false;

    file << "UWMS1\n";
    file << "USER " << customer.userID << ' ' << std::quoted(customer.name) << ' '
         << std::quoted(customer.email) << "\n";
    file << "WALLETS " << customer.wallets.size() << "\n";

    
std::set<int> walletIDs;

for (const Wallet* wallet : customer.wallets)
{
    if (!wallet ||
        wallet->userID != customer.userID ||
        wallet->walletID <= 0 ||
        !std::isfinite(wallet->balance) ||
        wallet->balance < 0 ||
        wallet->currency.empty() ||
        !walletIDs.insert(wallet->walletID).second)
    {
        file.close();
        std::remove(tempName.c_str());
        return false;
    }

    // Validate wallet type and currency
    std::string walletType = wallet->getWalletType();

    bool validWallet = false;

    if (walletType == "BANK" && wallet->currency == "INR")
        validWallet = true;

    else if (walletType == "CRYPTO" && wallet->currency == "BTC")
        validWallet = true;

    else if (walletType == "CBDC" && wallet->currency == "INR")
        validWallet = true;

    if (!validWallet)
    {
        file.close();
        std::remove(tempName.c_str());
        return false;
    }

    // Save wallet
    file << "W " << std::quoted(walletType) << ' '
         << wallet->walletID << ' '
         << wallet->userID << ' '
         << std::setprecision(17) << wallet->balance << ' '
         << std::quoted(wallet->currency) << "\n";
}


    file << "TRANSACTIONS " << customer.transactions.size() << "\n";
    std::set<int> transactionIDs;
    for (const Transaction& txn : customer.transactions) {
        bool validReferences = false;

    if (txn.type == "DEPOSIT")
    {
        validReferences =
            txn.senderWalletID == -1 &&
            walletIDs.count(txn.receiverWalletID);
    }
    else if (txn.type == "WITHDRAWAL")
    {
        validReferences =
            walletIDs.count(txn.senderWalletID) &&
            txn.receiverWalletID == -1;
    }
    else if (txn.type == "TRANSFER")
    {
        validReferences =
            walletIDs.count(txn.senderWalletID) &&
            walletIDs.count(txn.receiverWalletID) &&
            txn.senderWalletID != txn.receiverWalletID;
    }

    if (txn.transactionID <= 0 ||
        !transactionIDs.insert(txn.transactionID).second ||
        !validTransactionType(txn.type) ||
        !validStatus(txn.status) ||
        !std::isfinite(txn.amount) ||
        txn.amount <= 0 ||
        txn.timestamp.empty() ||
        !validReferences)
        {
            file.close();
            std::remove(tempName.c_str());
            return false;
        }
        file << "T " << txn.transactionID << ' ' << std::quoted(txn.type) << ' '
             << txn.senderWalletID << ' ' << txn.receiverWalletID << ' '
             << std::setprecision(17) << txn.amount << ' ' << std::quoted(txn.status) << ' '
             << std::quoted(txn.timestamp) << "\n";
        }
    file.flush();
    if (!file) { file.close(); std::remove(tempName.c_str()); return false; }
    file.close();
    if (!MoveFileExA(
        tempName.c_str(),
        filename.c_str(),
        MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH))
{
    DWORD errorCode = GetLastError();

    std::cerr << "Error: Unable to replace customer data file.\n";
    std::cerr << "Windows error code: " << errorCode << "\n";
    std::cerr << "Original file has been preserved.\n";
    std::cerr << "Temporary file: " << tempName << "\n";

    return false;
}
    return true;
}

bool FileManager::loadCustomer(Customer& customer, const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open())
    {
        cout << "Error: Unable to open customer data file: "
         << filename << "\n";
        return false;
    }
    std::string version, label;
    if (!std::getline(file, version) || version != "UWMS1") return false;

    int savedUserID = 0;
    std::string savedName, savedEmail;
    file >> label >> savedUserID >> std::quoted(savedName) >> std::quoted(savedEmail);
    if (!file || label != "USER" || savedUserID != customer.userID ||
        savedName != customer.name || savedEmail != customer.email) return false;

    std::size_t walletCount = 0;
    file >> label >> walletCount;
    if (!file || label != "WALLETS" || walletCount > 10000) return false;

    std::vector<Wallet*> loadedWallets;
    std::set<int> walletIDs;
    auto failWallets = [&]() { deleteWallets(loadedWallets); return false; };
    for (std::size_t i = 0; i < walletCount; ++i) {
        std::string type, currency;
        int walletID = 0, userID = 0;
        double balance = 0;
        file >> label >> std::quoted(type) >> walletID >> userID >> balance >> std::quoted(currency);
        if (!file || label != "W" || userID != savedUserID || walletID <= 0 ||
            !walletIDs.insert(walletID).second || !std::isfinite(balance) || balance < 0 || currency.empty())
            return failWallets();
        Wallet* wallet = nullptr;
        try {
            if (type == "BANK" && currency == "INR") wallet = new BankWallet(walletID, userID, balance, currency);
            else if (type == "CRYPTO" && currency == "BTC") wallet = new CryptoWallet(walletID, userID, balance, currency);
            else if (type == "CBDC" && currency == "INR") wallet = new CBDCWallet(walletID, userID, balance, currency);
            else return failWallets();
        } catch (...) { return failWallets(); }
        loadedWallets.push_back(wallet);
    }

    std::size_t transactionCount = 0;
    file >> label >> transactionCount;
    if (!file || label != "TRANSACTIONS" || transactionCount > 100000) return failWallets();
    std::vector<Transaction> loadedTransactions;
    std::set<int> transactionIDs;
    int maxTransactionID = 0;
    for (std::size_t i = 0; i < transactionCount; ++i) {
        int id = 0, sender = -1, receiver = -1;
        double amount = 0;
        std::string type, status, timestamp;
        file >> label >> id >> std::quoted(type) >> sender >> receiver >> amount
             >> std::quoted(status) >> std::quoted(timestamp);
        if (!file || label != "T" || id <= 0 || !transactionIDs.insert(id).second ||
            !validTransactionType(type) || !validStatus(status) || !std::isfinite(amount) ||
            amount <= 0 || timestamp.empty()) return failWallets();
        if (type == "DEPOSIT" && (sender != -1 || !walletIDs.count(receiver))) return failWallets();
        if (type == "WITHDRAWAL" && (!walletIDs.count(sender) || receiver != -1)) return failWallets();
        if (type == "TRANSFER" && (!walletIDs.count(sender) || !walletIDs.count(receiver) || sender == receiver)) return failWallets();
        loadedTransactions.emplace_back(id, type, sender, receiver, amount, status);
        loadedTransactions.back().timestamp = timestamp;
        if (id > maxTransactionID) maxTransactionID = id;
    }
    file >> std::ws;
    if (!file.eof()) return failWallets();

    deleteWallets(customer.wallets);
    customer.wallets.swap(loadedWallets);
    customer.transactions.swap(loadedTransactions);
    deleteWallets(loadedWallets);
    reserveTransactionID(maxTransactionID);
    return true;
}

int FileManager::generateTransactionID() {
    const std::string path = "data/transaction_counter.txt";
    int next = 10001;
    { std::ifstream in(path); if (in) in >> next; }
    if (next <= 0 || next == std::numeric_limits<int>::max()) return -1;
    { std::ofstream out(path, std::ios::trunc); if (!out) return -1; out << (next + 1) << '\n'; if (!out) return -1; }
    return next;
}
void FileManager::reserveTransactionID(int usedID) {
    if (usedID < 0 || usedID == std::numeric_limits<int>::max()) return;
    const std::string path = "data/transaction_counter.txt";
    int next = 10001;
    { std::ifstream in(path); if (in) in >> next; }
    if (next <= usedID) {
        std::ofstream out(path, std::ios::trunc);
        if (out) out << (usedID + 1) << '\n';
    }
}
