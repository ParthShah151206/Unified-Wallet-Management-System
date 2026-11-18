#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <iostream>
#include <string>

using namespace std;

class Transaction
{
private:
    int transactionID;
    string type;
    int senderWalletID;
    int receiverWalletID;
    double amount;
    string status;
    string timestamp;

public:
    Transaction(
        int id,
        const string& t,
        int sender,
        int receiver,
        double amt,
        const string& s
    );

    void displayTransaction() const;

    int getTransactionID() const;
    string getType() const;
    int getSenderWalletID() const;
    int getReceiverWalletID() const;
    double getAmount() const;
    string getStatus() const;
    string getTimestamp() const;

    friend class FileManager;
};

#endif