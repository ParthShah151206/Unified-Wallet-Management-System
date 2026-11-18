#ifndef UWMS_USER_H
#define UWMS_USER_H
#include <string>

class User {
protected:
    int userID;
    std::string name, email, password;
public:
    User(int id, const std::string& n, const std::string& e, const std::string& p);
    virtual ~User() = default;
    virtual void displayDashboard() = 0;
    void displayProfile() const;
    int getUserID() const;
    std::string getName() const;
    std::string getEmail() const;
};
#endif
