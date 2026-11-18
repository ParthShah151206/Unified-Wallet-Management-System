#include "include/Authentication.h"
#include "include/FileManager.h"
#include "include/Input.h"
#include <iostream>
#include <string>

int main() {
    while (true) {
        std::cout << "\n====================================\n"
                  << "   UNIFIED WALLET MANAGEMENT SYSTEM\n"
                  << "====================================\n"
                  << "1. Customer Registration\n2. Customer Login\n3. Admin Login\n4. Exit\n";
        int choice;
        if (!readInt("Enter your choice: ", choice)) break;
        discardLine();
        if (choice == 1) {
            std::string name, email, password;
            std::cout << "\n========== CUSTOMER REGISTRATION ==========\nName: ";
            std::getline(std::cin, name);
            std::cout << "Email: "; std::getline(std::cin, email);
            std::cout << "Password: "; std::getline(std::cin, password);
            Authentication::registerCustomer(name, email, password);
        } else if (choice == 2) {
            std::string email, password;
            std::cout << "\n========== CUSTOMER LOGIN ==========\nEmail: "; std::getline(std::cin, email);
            std::cout << "Password: "; std::getline(std::cin, password);
            Customer* customer = Authentication::loginCustomer(email, password);
            if (customer) {
                std::cout << "Welcome, " << customer->getName() << "!\n";
                customer->displayDashboard();
                const std::string path = "data/customer_" + std::to_string(customer->getUserID()) + ".txt";
                if (FileManager::saveCustomer(*customer, path)) std::cout << "Customer data saved.\n";
                else std::cerr << "ERROR: Customer data could not be saved.\n";
                delete customer;
            }
        } else if (choice == 3) {
            std::string email, password;
            std::cout << "\n========== ADMIN LOGIN ==========\nEmail: "; std::getline(std::cin, email);
            std::cout << "Password: "; std::getline(std::cin, password);
            Admin* admin = Authentication::loginAdmin(email, password);
            if (admin) { admin->displayDashboard(); delete admin; }
        } else if (choice == 4) {
            std::cout << "Thank you for using UWMS!\n"; break;
        } else std::cout << "Invalid choice. Please try again.\n";
    }
    return 0;
}
