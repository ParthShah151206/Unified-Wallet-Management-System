#ifndef UWMS_ADMIN_H
#define UWMS_ADMIN_H
#include "User.h"
class Admin : public User {
public:
    Admin(int id, const std::string& n, const std::string& e, const std::string& p);
    void displayDashboard() override;
};
#endif