#include "../include/Admin.h"
#include "../include/AdminService.h"
#include "../include/Input.h"
#include "../include/AssetTypeManager.h"
#include <iostream>

Admin::Admin(int id, const std::string& n,
             const std::string& e, const std::string& p)
    : User(id, n, e, p)
{
}

void Admin::displayDashboard()
{
    while (true)
    {
        std::cout << "\n";
        std::cout << "============================================\n";
        std::cout << "               ADMIN DASHBOARD              \n";
        std::cout << "============================================\n";

        displayProfile();

        std::cout << "\n--------------- MENU OPTIONS ---------------\n";
        std::cout << "1. View All Customers\n";
        std::cout << "2. Manage Asset Types\n";
        std::cout << "3. View All Wallets\n";
        std::cout << "4. View All Transactions\n";
        std::cout << "5. Generate System Reports\n";
        std::cout << "6. Logout\n";
        std::cout << "--------------------------------------------\n";

        int choice;

        if (!readInt("Enter your choice: ", choice))
        {
            std::cout << "Input ended. Logging out...\n";
            return;
        }

        switch (choice)
        {
            case 1:
                AdminService::viewAllCustomers();
                break;

            case 2:
                AssetTypeManager::manageAssetTypes();
                break;

            case 3:
                AdminService::viewAllWallets();
                break;

            case 4:
                AdminService::viewAllTransactions();
                break;

            case 5:
                AdminService::displaySystemReports();
                break;

            case 6:
                std::cout << "\nLogging out from Admin Account...\n";
                return;

            default:
                std::cout << "\nInvalid choice! Select between 1 and 6.\n";
        }

        std::cout << "\nPress Enter to continue...";
        discardLine();
        std::cin.get();
    }
}
