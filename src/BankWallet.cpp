#include "../include/BankWallet.h"
#include <iostream>
#include <cmath>
#include <iomanip>
#include <limits>

BankWallet::BankWallet(int wID, int uID, double bal, const std::string& curr)
    : Wallet(wID, uID, bal, curr) {}

bool BankWallet::deposit(double amount) {
    if (!std::isfinite(amount) || amount <= 0.0 || amount > std::numeric_limits<double>::max() - balance) {
        std::cout << "Invalid deposit amount.\n";
        return false;
    }
    balance += amount;
    std::cout << "Bank wallet deposit successful!\n";
    return true;
}

bool BankWallet::withdraw(double amount) {
    if (!std::isfinite(amount) || amount <= 0.0) {
        std::cout << "Invalid withdrawal amount.\n";
        return false;
    }
    if (amount > balance) {
        std::cout << "Insufficient balance!\n";
        return false;
    }
    balance -= amount;
    std::cout << "Bank wallet withdrawal successful!\n";
    return true;
}

std::string BankWallet::getWalletType() const { return "BANK"; }
void BankWallet::displayWallet() const {
    std::cout << "\n========== BANK WALLET ==========\n"
              << "Wallet ID: " << walletID << "\nUser ID: " << userID
              << "\nBalance: " << std::fixed << std::setprecision(8) << balance
              << " " << currency << "\n";
}
