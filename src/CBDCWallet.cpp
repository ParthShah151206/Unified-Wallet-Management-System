#include "../include/CBDCWallet.h"
#include <iostream>
#include <cmath>
#include <iomanip>
#include <limits>

CBDCWallet::CBDCWallet(int wID, int uID, double bal, const std::string& curr)
    : Wallet(wID, uID, bal, curr) {}

bool CBDCWallet::deposit(double amount) {
    if (!std::isfinite(amount) || amount <= 0.0 || amount > std::numeric_limits<double>::max() - balance) {
        std::cout << "Invalid deposit amount.\n";
        return false;
    }
    balance += amount;
    std::cout << "CBDC wallet deposit successful!\n";
    return true;
}

bool CBDCWallet::withdraw(double amount) {
    if (!std::isfinite(amount) || amount <= 0.0) {
        std::cout << "Invalid withdrawal amount.\n";
        return false;
    }
    if (amount > balance) {
        std::cout << "Insufficient balance!\n";
        return false;
    }
    balance -= amount;
    std::cout << "CBDC wallet withdrawal successful!\n";
    return true;
}

std::string CBDCWallet::getWalletType() const { return "CBDC"; }
void CBDCWallet::displayWallet() const {
    std::cout << "\n========== CBDC WALLET ==========\n"
              << "Wallet ID: " << walletID << "\nUser ID: " << userID
              << "\nBalance: " << std::fixed << std::setprecision(8) << balance
              << " " << currency << "\n";
}
