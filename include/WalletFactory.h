#ifndef UWMS_WALLET_FACTORY_H
#define UWMS_WALLET_FACTORY_H

#include "Wallet.h"
#include <string>

class WalletFactory {
public:

    // Create wallet using menu choice
    static Wallet* createWallet(
        int choice,
        int walletID,
        int userID,
        double balance
    );

    // Recreate wallet using its saved type
    static Wallet* createWallet(
        const std::string& type,
        int walletID,
        int userID,
        double balance,
        const std::string& currency
    );
};

#endif
