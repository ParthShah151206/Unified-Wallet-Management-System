#include "../include/WalletFactory.h"

#include "../include/BankWallet.h"
#include "../include/CryptoWallet.h"
#include "../include/CBDCWallet.h"
#include "../include/UPIWallet.h"

#include <iostream>

using namespace std;

Wallet* WalletFactory::createWallet(
    int choice,
    int walletID,
    int userID,
    double balance
)
{
    switch (choice)
    {
        case 1:
            return new BankWallet(
                walletID, userID, balance, "INR"
            );

        case 2:
            return new CryptoWallet(
                walletID, userID, balance, "BTC"
            );

        case 3:
            return new CBDCWallet(
                walletID, userID, balance, "INR"
            );

        case 4:
            return new UPIWallet(
                walletID, userID, balance, "INR"
            );

        default:
            cout << "Invalid wallet choice!\n";
            return nullptr;
    }
}

Wallet* WalletFactory::createWallet(
    const string& type,
    int walletID,
    int userID,
    double balance,
    const string& currency
)
{
    if (type == "BANK")
    {
        return new BankWallet(
            walletID, userID, balance, currency
        );
    }

    if (type == "CRYPTO")
    {
        return new CryptoWallet(
            walletID, userID, balance, currency
        );
    }

    if (type == "CBDC")
    {
        return new CBDCWallet(
            walletID, userID, balance, currency
        );
    }

    if (type == "UPI")
    {
        return new UPIWallet(
            walletID, userID, balance, currency
        );
    }

    return nullptr;
}
