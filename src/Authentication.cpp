#include "../include/Authentication.h"
#include "../include/FileManager.h"
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <functional>
#include <cstdio>
#include <cctype>
#include <limits>
#include <iostream>

std::size_t Authentication::hashPassword(const std::string& password) {
    // Preserves compatibility with the existing users.txt format from the original project.
    // std::hash is NOT a secure or portable password-storage mechanism for production.
    return std::hash<std::string>{}(password);
}
bool Authentication::emailExists(const std::string& email) {
    std::ifstream file("data/users.txt");
    if (!file) return false;
    int id; std::string name, savedEmail; std::size_t passwordHash;
    while (file >> id >> std::quoted(name) >> std::quoted(savedEmail) >> passwordHash)
        if (savedEmail == email) return true;
    return false;
}
int Authentication::generateUserID() {
    std::ifstream file("data/users.txt");
    int id = 100, maxID = 100; std::string name, email; std::size_t hash;
    while (file >> id >> std::quoted(name) >> std::quoted(email) >> hash) maxID = std::max(maxID, id);
    if (maxID == std::numeric_limits<int>::max()) return -1;
    return maxID + 1;
}
bool Authentication::registerCustomer(const std::string& name, const std::string& email, const std::string& password) {
    if (name.empty() || email.empty() || password.empty()) { std::cout << "All fields are required.\n"; return false; }
    if (name.find_first_not_of(" \t\r\n") == std::string::npos || email.find('@') == std::string::npos ||
        email.find(' ') != std::string::npos) { std::cout << "Enter a valid name and email address.\n"; return false; }
    if (password.size() < 6) { std::cout << "Password must contain at least 6 characters.\n"; return false; }
    if (emailExists(email)) { std::cout << "Email is already registered.\n"; return false; }
    const int userID = generateUserID();
    if (userID < 0) { std::cout << "Could not generate a customer ID.\n"; return false; }
    Customer customer(userID, name, email, password);
    const std::string filename = "data/customer_" + std::to_string(userID) + ".txt";
    if (!FileManager::saveCustomer(customer, filename)) { std::cout << "Could not create customer data file. Ensure data/ exists.\n"; return false; }
    std::ofstream file("data/users.txt", std::ios::app);
    if (!file) { std::remove(filename.c_str()); std::cout << "Unable to save login credentials.\n"; return false; }
    file << userID << ' ' << std::quoted(name) << ' ' << std::quoted(email) << ' ' << hashPassword(password) << '\n';
    file.flush();
    if (!file) { file.close(); std::remove(filename.c_str()); std::cout << "Unable to write login credentials.\n"; return false; }
    std::cout << "Registration successful. Your Customer ID is: " << userID << '\n';
    return true;
}
Customer* Authentication::loginCustomer(const std::string& email, const std::string& password) {
    std::ifstream file("data/users.txt");
    if (!file) { std::cout << "Unable to open data/users.txt. Register first or check the data folder.\n"; return nullptr; }
    int id; std::string name, savedEmail; std::size_t savedHash;
    while (file >> id >> std::quoted(name) >> std::quoted(savedEmail) >> savedHash) {
        if (savedEmail == email && savedHash == hashPassword(password)) {
            file.close();
            Customer* customer = new Customer(id, name, savedEmail, password);
            const std::string path = "data/customer_" + std::to_string(id) + ".txt";
            if (!FileManager::loadCustomer(*customer, path)) { delete customer; std::cout << "Customer data file is invalid or missing.\n"; return nullptr; }
            std::cout << "Login successful.\n";
            return customer;
        }
    }
    std::cout << "Invalid email or password.\n";
    return nullptr;
}
Admin* Authentication::loginAdmin(const std::string& email, const std::string& password) {
    // Prototype credentials. Replace with a persisted, securely hashed admin account before deployment.
    if (email == "admin@uwms.com" && password == "admin123") {
        std::cout << "Admin login successful.\n";
        return new Admin(1, "System Administrator", email, password);
    }
    std::cout << "Invalid admin email or password.\n";
    return nullptr;
}