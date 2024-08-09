#pragma once
#include <QSettings>
#include "../settings.h"
#include "settings_internal.h"
#include "setting_value.h"

namespace settings
{
class BoolSettingValue : public SettingValue
{
public:
    BoolSettingValue( const BoolSetting setting,
                      const SettingCategory category,
                      const QtInfo qtInfo,
                      const bool defaultValue )
        : SettingValue( category, qtInfo ), m_setting( setting ),
          m_value( defaultValue )
    {
        const auto var = getQtSetting( SettingValue::category(),
                                       SettingValue::qtInfo().settingName );
        if ( isValidQVariant<bool>( var ) )
        {
            m_value = var.toBool();
        }
    }

    void setValue( const bool value ) noexcept
    {
        m_value = value;
    }

    [[nodiscard]] bool value() const noexcept
    {
        return m_value;
    }

    [[nodiscard]] BoolSetting setting() const noexcept
    {
        return m_setting;
    }

    [[nodiscard]] SettingCategory category() const noexcept
    {
        return SettingValue::category();
    }

    [[nodiscard]] QtInfo qtInfo() const noexcept
    {
        return SettingValue::qtInfo();
    }

    void saveValue() override
    {
        saveQtSetting( SettingValue::category(),
                       SettingValue::qtInfo().settingName,
                       m_value );
    }

private:
    const BoolSetting m_setting;
    bool m_value;
};
} // namespace settings
