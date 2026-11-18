#ifndef UWMS_WALLET_H
#define UWMS_WALLET_H
#include <string>

class FileManager;
class Wallet {
protected:
    int walletID, userID;
    double balance;
    std::string currency;
public:
    Wallet(int wID, int uID, double bal, const std::string& curr);
    virtual ~Wallet() = default;
    virtual bool deposit(double amount) = 0;
    virtual bool withdraw(double amount) = 0;
    virtual void displayWallet() const = 0;
    virtual std::string getWalletType() const = 0;
    int getWalletID() const;
    int getUserID() const;
    double getBalance() const;
    std::string getCurrency() const;
    friend class FileManager;
};
#endif
