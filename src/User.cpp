#include "../include/User.h"
#include <iostream>
User::User(int id, const std::string& n, const std::string& e, const std::string& p)
    : userID(id), name(n), email(e), password(p) {}
void User::displayProfile() const {
    std::cout << "\nUser ID: " << userID << "\nName: " << name << "\nEmail: " << email << '\n';
}
int User::getUserID() const { return userID; }
std::string User::getName() const { return name; }
std::string User::getEmail() const { return email; }
