#include "../include/Transaction.h"

#include <ctime>
#include <iomanip>
#include <iostream>

using namespace std;

Transaction::Transaction(
    int id,
    const string& t,
    int sender,
    int receiver,
    double amt,
    const string& s
): 
      transactionID(id),
      type(t),
      senderWalletID(sender),
      receiverWalletID(receiver),
      amount(amt),
      status(s)
{
    time_t now = time(nullptr);
    tm* local = localtime(&now);

    if (local != nullptr) {
        char buffer[30];

        if (strftime(buffer, sizeof(buffer),
                     "%Y-%m-%d %H:%M:%S", local) > 0) {
            timestamp = buffer;
        } else {
            timestamp = "Unknown";
        }
    } else {
        timestamp = "Unknown";
    }
}

void Transaction::displayTransaction() const
{
    cout << "\nTransaction ID: " << transactionID;
    cout << "\nType: " << type;
    cout << "\nSender Wallet ID: " << senderWalletID;
    cout << "\nReceiver Wallet ID: " << receiverWalletID;
    cout << "\nAmount: " << fixed << setprecision(4) << amount;
    cout << "\nStatus: " << status;
    cout << "\nTimestamp: " << timestamp << "\n";
}

int Transaction::getTransactionID() const
{
    return transactionID;
}

string Transaction::getType() const
{
    return type;
}

int Transaction::getSenderWalletID() const
{
    return senderWalletID;
}

int Transaction::getReceiverWalletID() const
{
    return receiverWalletID;
}

double Transaction::getAmount() const
{
    return amount;
}

string Transaction::getStatus() const
{
    return status;
}

string Transaction::getTimestamp() const
{
    return timestamp;
}