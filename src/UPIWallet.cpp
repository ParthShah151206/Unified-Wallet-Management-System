#include "../include/UPIWallet.h"

#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>

using namespace std;

UPIWallet::UPIWallet(
    int walletID,
    int userID,
    double balance,
    string currency
)
    : Wallet(walletID, userID, balance, currency),
      upiID(
          "user" + to_string(userID) +
          "_" + to_string(walletID) + "@uwms"
      ),
      transactionLimit(100000.0)
{
}

bool UPIWallet::deposit(double amount)
{
    if (!isfinite(amount) || amount <= 0) {
        cout << "Invalid deposit amount!\n";
        return false;
    }

    balance += amount;

    cout << "UPI deposit successful!\n";
    cout << "Updated balance: "
         << fixed << setprecision(2)
         << balance << " " << currency << "\n";

    return true;
}

bool UPIWallet::withdraw(double amount)
{
    if (!isfinite(amount) || amount <= 0) {
        cout << "Invalid withdrawal amount!\n";
        return false;
    }

    if (amount > transactionLimit) {
        cout << "UPI transaction limit exceeded!\n";
        cout << "Maximum limit: "
             << transactionLimit << " "
             << currency << "\n";
        return false;
    }

    if (amount > balance) {
        cout << "Insufficient UPI wallet balance!\n";
        return false;
    }

    balance -= amount;

    cout << "UPI payment successful!\n";
    cout << "Updated balance: "
         << fixed << setprecision(2)
         << balance << " " << currency << "\n";

    return true;
}

void UPIWallet::displayWallet() const
{
    cout << "\n========== UPI WALLET ==========\n";

    cout << "Wallet ID        : " << walletID << "\n";
    cout << "User ID          : " << userID << "\n";
    cout << "UPI ID           : " << upiID << "\n";

    cout << "Balance          : "
         << fixed << setprecision(2)
         << balance << " " << currency << "\n";

    cout << "Transaction Limit: "
         << transactionLimit << " " << currency << "\n";

    cout << "================================\n";
}

string UPIWallet::getWalletType() const
{
    return "UPI";
}

string UPIWallet::getUPIID() const
{
    return upiID;
}

double UPIWallet::getTransactionLimit() const
{
    return transactionLimit;
}
