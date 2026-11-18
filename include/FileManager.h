#ifndef UWMS_FILEMANAGER_H
#define UWMS_FILEMANAGER_H
#include <string>
#include "Customer.h"
class FileManager {
public:
    static bool saveCustomer(const Customer& customer, const std::string& filename);
    static bool loadCustomer(Customer& customer, const std::string& filename);
    static int generateTransactionID();
    static void reserveTransactionID(int usedID);
};
#endif