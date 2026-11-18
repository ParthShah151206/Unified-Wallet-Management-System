#include "../include/AdminService.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

void AdminService::viewAllCustomers()
{
    std::ifstream file("data/users.txt");

    if (!file.is_open())
    {
        std::cout << "\nUnable to open data/users.txt!\n";
        return;
    }

    int id;
    std::string name, email;
    std::size_t passwordHash;

    int count = 0;

    std::cout << "\n";
    std::cout << "============================================================\n";
    std::cout << "                    ALL CUSTOMERS                          \n";
    std::cout << "============================================================\n";

    std::cout << std::left
              << std::setw(12) << "Customer ID"
              << std::setw(25) << "Name"
              << std::setw(30) << "Email" << "\n";

    std::cout << "------------------------------------------------------------\n";

    while (file >> id
                >> std::quoted(name)
                >> std::quoted(email)
                >> passwordHash)
    {
        std::cout << std::left
                  << std::setw(12) << id
                  << std::setw(25) << name
                  << std::setw(30) << email
                  << "\n";

        count++;
    }

    std::cout << "------------------------------------------------------------\n";
    std::cout << "Total Registered Customers: " << count << "\n";
    std::cout << "============================================================\n";

    file.close();
}

void AdminService::viewAllWallets()
{
    std::ifstream usersFile("data/users.txt");

    if (!usersFile.is_open())
    {
        std::cout << "\nUnable to open data/users.txt!\n";
        return;
    }

    int customerID;
    std::string name, email;
    std::size_t passwordHash;

    int totalWallets = 0;

    std::cout << "\n";
    std::cout << "==========================================================================\n";
    std::cout << "                         ALL CUSTOMER WALLETS                             \n";
    std::cout << "==========================================================================\n";

    std::cout << std::left
              << std::setw(12) << "Customer ID"
              << std::setw(12) << "Wallet ID"
              << std::setw(15) << "Wallet Type"
              << std::setw(15) << "Currency"
              << std::setw(15) << "Balance"
              << "\n";

    std::cout << "--------------------------------------------------------------------------\n";

    while (usersFile >> customerID
                     >> std::quoted(name)
                     >> std::quoted(email)
                     >> passwordHash)
    {
        std::string filename =
            "data/customer_" + std::to_string(customerID) + ".txt";

        std::ifstream customerFile(filename);

        if (!customerFile.is_open())
            continue;

        std::string version;
        std::getline(customerFile, version);

        if (version != "UWMS1")
            continue;

        std::string label;
        int savedUserID;
        std::string savedName, savedEmail;

        customerFile >> label
                     >> savedUserID
                     >> std::quoted(savedName)
                     >> std::quoted(savedEmail);

        if (!customerFile || label != "USER" ||
            savedUserID != customerID)
            continue;

        std::size_t walletCount;

        customerFile >> label >> walletCount;

        if (!customerFile || label != "WALLETS" ||
            walletCount > 10000)
            continue;

        for (std::size_t i = 0; i < walletCount; i++)
        {
            std::string type, currency;
            int walletID, userID;
            double balance;

            customerFile >> label
                         >> std::quoted(type)
                         >> walletID
                         >> userID
                         >> balance
                         >> std::quoted(currency);

            if (!customerFile || label != "W")
                break;

            std::cout << std::left
                      << std::setw(12) << userID
                      << std::setw(12) << walletID
                      << std::setw(15) << type
                      << std::setw(15) << currency
                      << std::fixed << std::setprecision(2)
                      << balance << "\n";

            totalWallets++;
        }
    }

    usersFile.close();

    std::cout << "--------------------------------------------------------------------------\n";
    std::cout << "Total Wallets: " << totalWallets << "\n";
    std::cout << "==========================================================================\n";

    if (totalWallets == 0)
        std::cout << "No wallets found!\n";
}

void AdminService::viewAllTransactions()
{
    std::ifstream usersFile("data/users.txt");

    if (!usersFile.is_open())
    {
        std::cout << "\nUnable to open users.txt!\n";
        return;
    }

    int customerID;
    std::string name, email;
    std::size_t passwordHash;

    int totalTransactions = 0;

    std::cout << "\n";
    std::cout << "====================================================================================================\n";
    std::cout << "                                  ALL CUSTOMER TRANSACTIONS\n";
    std::cout << "====================================================================================================\n";

    std::cout << std::left
              << std::setw(10) << "Cust ID"
              << std::setw(10) << "Txn ID"
              << std::setw(15) << "Type"
              << std::setw(12) << "Sender"
              << std::setw(12) << "Receiver"
              << std::setw(15) << "Amount"
              << std::setw(15) << "Status"
              << "Timestamp\n";

    std::cout << "----------------------------------------------------------------------------------------------------\n";

    while (usersFile >> customerID
                     >> std::quoted(name)
                     >> std::quoted(email)
                     >> passwordHash)
    {
        std::string filename =
            "data/customer_" + std::to_string(customerID) + ".txt";

        std::ifstream customerFile(filename);

        if (!customerFile.is_open())
            continue;

        std::string version;
        std::getline(customerFile, version);

        if (version != "UWMS1")
            continue;

        std::string label;
        int savedUserID;
        std::string savedName, savedEmail;

        customerFile >> label
                     >> savedUserID
                     >> std::quoted(savedName)
                     >> std::quoted(savedEmail);

        if (!customerFile || label != "USER" ||
            savedUserID != customerID)
            continue;

        std::size_t walletCount;

        customerFile >> label >> walletCount;

        if (!customerFile || label != "WALLETS" ||
            walletCount > 10000)
            continue;

        // Skip wallet records.
        bool validFile = true;

        for (std::size_t i = 0; i < walletCount; i++)
        {
            std::string type, currency;
            int walletID, userID;
            double balance;

            customerFile >> label
                         >> std::quoted(type)
                         >> walletID
                         >> userID
                         >> balance
                         >> std::quoted(currency);

            if (!customerFile || label != "W")
            {
                validFile = false;
                break;
            }
        }

        if (!validFile)
            continue;

        std::size_t transactionCount;

        customerFile >> label >> transactionCount;

        if (!customerFile || label != "TRANSACTIONS" ||
            transactionCount > 100000)
            continue;

        for (std::size_t i = 0; i < transactionCount; i++)
        {
            int txnID, senderID, receiverID;
            std::string type, status, timestamp;
            double amount;

            customerFile >> label
                         >> txnID
                         >> std::quoted(type)
                         >> senderID
                         >> receiverID
                         >> amount
                         >> std::quoted(status)
                         >> std::quoted(timestamp);

            if (!customerFile || label != "T")
                break;

            std::cout << std::left
                      << std::setw(10) << customerID
                      << std::setw(10) << txnID
                      << std::setw(15) << type
                      << std::setw(12) << senderID
                      << std::setw(12) << receiverID
                      << std::setw(15) << std::fixed
                      << std::setprecision(2) << amount
                      << std::setw(15) << status
                      << timestamp << "\n";

            totalTransactions++;
        }
    }

    usersFile.close();

    std::cout << "----------------------------------------------------------------------------------------------------\n";
    std::cout << "Total Transactions: " << totalTransactions << "\n";
    std::cout << "====================================================================================================\n";

    if (totalTransactions == 0)
        std::cout << "No transactions found!\n";
}
void AdminService::displaySystemReports()
{
    std::ifstream usersFile("data/users.txt");

    if (!usersFile.is_open())
    {
        std::cout << "\nUnable to open users.txt!\n";
        return;
    }

    int totalCustomers = 0;
    int totalWallets = 0;
    int bankWallets = 0;
    int cryptoWallets = 0;
    int cbdcWallets = 0;

    int totalTransactions = 0;
    int successfulTransactions = 0;
    int failedTransactions = 0;

    double totalBalance = 0.0;

    int customerID;
    std::string name, email;
    std::size_t passwordHash;

    while (usersFile >> customerID
                    >> std::quoted(name)
                    >> std::quoted(email)
                    >> passwordHash)
    {
        totalCustomers++;

        std::string filename =
            "data/customer_" + std::to_string(customerID) + ".txt";

        std::ifstream customerFile(filename);

        if (!customerFile.is_open())
            continue;

        std::string version;
        std::getline(customerFile, version);

        if (version != "UWMS1")
            continue;

        std::string label;
        int savedUserID;
        std::string savedName, savedEmail;

        customerFile >> label
                     >> savedUserID
                     >> std::quoted(savedName)
                     >> std::quoted(savedEmail);

        if (!customerFile || label != "USER" ||
            savedUserID != customerID)
            continue;

        std::size_t walletCount;

        customerFile >> label >> walletCount;

        if (!customerFile || label != "WALLETS" ||
            walletCount > 10000)
            continue;

        bool validFile = true;

        for (std::size_t i = 0; i < walletCount; i++)
        {
            std::string type, currency;
            int walletID, userID;
            double balance;

            customerFile >> label
                         >> std::quoted(type)
                         >> walletID
                         >> userID
                         >> balance
                         >> std::quoted(currency);

            if (!customerFile || label != "W")
            {
                validFile = false;
                break;
            }

            totalWallets++;
            totalBalance += balance;

            if (type == "Bank")
                bankWallets++;
            else if (type == "Crypto")
                cryptoWallets++;
            else if (type == "CBDC")
                cbdcWallets++;
        }

        if (!validFile)
            continue;

        std::size_t transactionCount;

        customerFile >> label >> transactionCount;

        if (!customerFile || label != "TRANSACTIONS" ||
            transactionCount > 100000)
            continue;

        for (std::size_t i = 0; i < transactionCount; i++)
        {
            int txnID, senderID, receiverID;
            std::string type, status, timestamp;
            double amount;

            customerFile >> label
                         >> txnID
                         >> std::quoted(type)
                         >> senderID
                         >> receiverID
                         >> amount
                         >> std::quoted(status)
                         >> std::quoted(timestamp);

            if (!customerFile || label != "T")
                break;

            totalTransactions++;

            if (status == "SUCCESS")
                successfulTransactions++;
            else if (status == "FAILED")
                failedTransactions++;
        }
    }

    usersFile.close();

    std::cout << "\n";
    std::cout << "====================================================\n";
    std::cout << "               UWMS SYSTEM REPORT                  \n";
    std::cout << "====================================================\n";

    std::cout << "\nCUSTOMER STATISTICS\n";
    std::cout << "Total Customers          : " << totalCustomers << "\n";

    std::cout << "\nWALLET STATISTICS\n";
    std::cout << "Total Wallets            : " << totalWallets << "\n";
    std::cout << "Bank Wallets             : " << bankWallets << "\n";
    std::cout << "Crypto Wallets           : " << cryptoWallets << "\n";
    std::cout << "CBDC Wallets             : " << cbdcWallets << "\n";

    std::cout << "\nTRANSACTION STATISTICS\n";
    std::cout << "Total Transactions       : " << totalTransactions << "\n";
    std::cout << "Successful Transactions  : " << successfulTransactions << "\n";
    std::cout << "Failed Transactions      : " << failedTransactions << "\n";

    std::cout << "\nFINANCIAL STATISTICS\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Total Wallet Balance     : " << totalBalance << "\n";

    std::cout << "====================================================\n";
}