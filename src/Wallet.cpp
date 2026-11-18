#include "../include/Wallet.h"
#include <cmath>
#include <stdexcept>
Wallet::Wallet(int wID, int uID, double bal, const std::string& curr)
    : walletID(wID), userID(uID), balance(bal), currency(curr) {
    if (wID <= 0 || uID <= 0 || !std::isfinite(bal) || bal < 0 || curr.empty())
        throw std::invalid_argument("Invalid wallet details");
}
int Wallet::getWalletID() const { return walletID; }
int Wallet::getUserID() const { return userID; }
double Wallet::getBalance() const { return balance; }
std::string Wallet::getCurrency() const { return currency; }
