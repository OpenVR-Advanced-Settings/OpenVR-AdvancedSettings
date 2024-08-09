#pragma once
#include <QSettings>
#include "../settings.h"
#include "settings_internal.h"
#include "setting_value.h"

namespace settings
{
class DoubleSettingValue : public SettingValue
{
public:
    DoubleSettingValue( const DoubleSetting setting,
                        const SettingCategory category,
                        const QtInfo qtInfo,
                        const double defaultValue )
        : SettingValue( category, qtInfo ), m_setting( setting ),
          m_value( defaultValue )
    {
        const auto val = getQtSetting( SettingValue::category(),
                                       SettingValue::qtInfo().settingName );
        if ( isValidQVariant<double>( val ) )
        {
            m_value = val.toDouble();
        }
    }

    void setValue( const double value ) noexcept
    {
        m_value = value;
    }

    [[nodiscard]] double value() const noexcept
    {
        return m_value;
    }

    [[nodiscard]] DoubleSetting setting() const noexcept
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
    const DoubleSetting m_setting;
    double m_value;
};
} // namespace settings
