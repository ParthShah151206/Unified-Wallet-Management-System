#include "../include/CryptoWallet.h"
#include <iostream>
#include <cmath>
#include <iomanip>
#include <limits>

CryptoWallet::CryptoWallet(int wID, int uID, double bal, const std::string& curr)
    : Wallet(wID, uID, bal, curr) {}

bool CryptoWallet::deposit(double amount) {
    if (!std::isfinite(amount) || amount <= 0.0 || amount > std::numeric_limits<double>::max() - balance) {
        std::cout << "Invalid deposit amount.\n";
        return false;
    }
    balance += amount;
    std::cout << "Crypto wallet deposit successful!\n";
    return true;
}

bool CryptoWallet::withdraw(double amount) {
    if (!std::isfinite(amount) || amount <= 0.0) {
        std::cout << "Invalid withdrawal amount.\n";
        return false;
    }
    if (amount > balance) {
        std::cout << "Insufficient balance!\n";
        return false;
    }
    balance -= amount;
    std::cout << "Crypto wallet withdrawal successful!\n";
    return true;
}

std::string CryptoWallet::getWalletType() const { return "CRYPTO"; }
void CryptoWallet::displayWallet() const {
    std::cout << "\n========== CRYPTO WALLET ==========\n"
              << "Wallet ID: " << walletID << "\nUser ID: " << userID
              << "\nBalance: " << std::fixed << std::setprecision(8) << balance
              << " " << currency << "\n";
}
