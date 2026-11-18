#ifndef UWMS_CBDCWALLET_H
#define UWMS_CBDCWALLET_H
#include "Wallet.h"
class CBDCWallet : public Wallet {
public:
    CBDCWallet(int wID, int uID, double bal, const std::string& curr);
    bool deposit(double amount) override;
    bool withdraw(double amount) override;
    std::string getWalletType() const override;
    void displayWallet() const override;
};
#endif
