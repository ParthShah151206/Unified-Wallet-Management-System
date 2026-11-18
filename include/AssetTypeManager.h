
#ifndef UWMS_ASSETTYPEMANAGER_H
#define UWMS_ASSETTYPEMANAGER_H

#include <string>
#include <vector>

class AssetTypeManager {
private:
    struct AssetType {
        std::string code;
        std::string name;
        bool enabled;
    };

    static std::vector<AssetType> assetTypes;

    static void loadAssetTypes();
    static bool saveAssetTypes();

public:
    static void manageAssetTypes();
    static bool isEnabled(const std::string& code);
};

#endif
