#pragma once
#include "internal/settings_object_data.h"

namespace settings
{
class ISettingsObject
{
public:
    virtual ~ISettingsObject() {}

    virtual SettingsObjectData saveSettings() const = 0;
    virtual void loadSettings( SettingsObjectData& obj ) = 0;

    virtual std::string settingsName() const = 0;
};

void saveObject( const ISettingsObject& obj );

void loadObject( ISettingsObject& obj );

void saveNumberedObject( const ISettingsObject& obj, const int slot );

void loadNumberedObject( ISettingsObject& obj, const int slot );

int getAmountOfSavedObjects( ISettingsObject& obj );

} // namespace settings
