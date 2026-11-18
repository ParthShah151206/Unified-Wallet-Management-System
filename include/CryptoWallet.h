#ifndef UWMS_CRYPTOWALLET_H
#define UWMS_CRYPTOWALLET_H
#include "Wallet.h"
class CryptoWallet : public Wallet {
public:
    CryptoWallet(int wID, int uID, double bal, const std::string& curr);
    bool deposit(double amount) override;
    bool withdraw(double amount) override;
    std::string getWalletType() const override;
    void displayWallet() const override;
};
#endif
