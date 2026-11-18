#ifndef UWMS_BANKWALLET_H
#define UWMS_BANKWALLET_H
#include "Wallet.h"
class BankWallet : public Wallet {
public:
    BankWallet(int wID, int uID, double bal, const std::string& curr);
    bool deposit(double amount) override;
    bool withdraw(double amount) override;
    std::string getWalletType() const override;
    void displayWallet() const override;
};
#endif
