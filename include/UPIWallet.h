#ifndef UWMS_UPI_WALLET_H
#define UWMS_UPI_WALLET_H

#include "Wallet.h"
#include <string>

class UPIWallet : public Wallet {
private:
    std::string upiID;
    double transactionLimit;

public:
    UPIWallet(
        int walletID,
        int userID,
        double balance,
        std::string currency
    );

    bool deposit(double amount) override;
    bool withdraw(double amount) override;

    void displayWallet() const override;
    std::string getWalletType() const override;

    std::string getUPIID() const;
    double getTransactionLimit() const;
};

#endif
