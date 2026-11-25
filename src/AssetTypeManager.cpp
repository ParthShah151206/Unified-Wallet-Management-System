
#include "../include/AssetTypeManager.h"
#include "../include/Input.h"

#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

vector<AssetTypeManager::AssetType> AssetTypeManager::assetTypes;

void AssetTypeManager::loadAssetTypes()
{
    assetTypes.clear();

    // Default supported wallet types
    assetTypes.push_back({"BANK", "Bank Wallet", true});
    assetTypes.push_back({"CRYPTO", "Crypto Wallet", true});
    assetTypes.push_back({"CBDC", "CBDC Wallet", true});
    assetTypes.push_back({"UPI", "UPI Wallet", true});

    ifstream file("data/asset_types.txt");

    // If the file does not exist, create it with default settings.
    if (!file.is_open()) {
        saveAssetTypes();
        return;
    }

    string code;
    int status;

    while (file >> code >> status) {

        // Update the status of an existing wallet type
        for (auto& asset : assetTypes) {

            if (asset.code == code &&
                (status == 0 || status == 1)) {

                asset.enabled = (status == 1);
                break;
            }
        }
    }

    file.close();
}

bool AssetTypeManager::saveAssetTypes()
{
    ofstream file("data/asset_types.txt");

    if (!file.is_open()) {
        cout << "\nUnable to save asset types!\n";
        return false;
    }

    for (const auto& asset : assetTypes) {

        file << asset.code << " "
             << (asset.enabled ? 1 : 0)
             << "\n";
    }

    file.close();
    return true;
}

bool AssetTypeManager::isEnabled(const string& code)
{
    loadAssetTypes();

    for (const auto& asset : assetTypes) {

        if (asset.code == code) {
            return asset.enabled;
        }
    }

    return false;
}

void AssetTypeManager::manageAssetTypes()
{
    loadAssetTypes();

    while (true) {

        cout << "\n========================================\n";
        cout << "          MANAGE ASSET TYPES             \n";
        cout << "========================================\n";

        cout << left << setw(8) << "No."
             << setw(20) << "Asset Type"
             << "Status\n";

        cout << "----------------------------------------\n";

        for (int i = 0;
             i < static_cast<int>(assetTypes.size());
             i++) {

            cout << left << setw(8) << i + 1
                 << setw(20) << assetTypes[i].name
                 << (assetTypes[i].enabled
                     ? "Enabled"
                     : "Disabled")
                 << "\n";
        }

        cout << "----------------------------------------\n";

        cout << "1. Enable an Asset Type\n";
        cout << "2. Disable an Asset Type\n";
        cout << "3. Back to Admin Dashboard\n";

        int choice;

        if (!readInt("\nEnter your choice: ", choice)) {
            return;
        }

        if (choice == 3) {
            return;
        }

        if (choice != 1 && choice != 2) {
            cout << "\nInvalid choice!\n";
            continue;
        }

        int assetChoice;

        if (!readInt("Enter asset type number: ", assetChoice)) {
            return;
        }

        if (assetChoice < 1 ||
            assetChoice > static_cast<int>(assetTypes.size())) {

            cout << "\nInvalid asset type number!\n";
            continue;
        }

        AssetType& selected = assetTypes[assetChoice - 1];

        bool newStatus = (choice == 1);

        if (selected.enabled == newStatus) {

            cout << "\n" << selected.name
                 << " is already "
                 << (newStatus ? "enabled" : "disabled")
                 << ".\n";

            continue;
        }

        selected.enabled = newStatus;

        if (saveAssetTypes()) {

            cout << "\n" << selected.name
                 << " has been "
                 << (newStatus ? "enabled" : "disabled")
                 << " successfully!\n";
        }
        else {
            // Restore the previous status if saving fails
            selected.enabled = !newStatus;
        }
    }
}
