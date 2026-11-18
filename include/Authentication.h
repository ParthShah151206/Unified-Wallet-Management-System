#ifndef UWMS_AUTHENTICATION_H
#define UWMS_AUTHENTICATION_H
#include <string>
#include "Customer.h"
#include "Admin.h"
class Authentication {
public:
    static bool registerCustomer(const std::string& name, const std::string& email, const std::string& password);
    static Customer* loginCustomer(const std::string& email, const std::string& password);
    static Admin* loginAdmin(const std::string& email, const std::string& password);
private:
    static std::size_t hashPassword(const std::string& password);
    static bool emailExists(const std::string& email);
    static int generateUserID();
};
#endif
