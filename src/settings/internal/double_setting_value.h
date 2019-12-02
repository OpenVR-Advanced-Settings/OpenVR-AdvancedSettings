#pragma once
#include <QSettings>
#include "settings.h"
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
        const auto v = getQtSetting( SettingValue::category(),
                                     SettingValue::qtInfo().settingName );
        if ( isValidQVariant( v, QMetaType::Double ) )
        {
            m_value = v.toDouble();
        }
    }

    void setValue( const double value ) noexcept
    {
        m_value = value;
    }

    double value() const noexcept
    {
        return m_value;
    }

    DoubleSetting setting() const noexcept
    {
        return m_setting;
    }

    SettingCategory category() const noexcept
    {
        return SettingValue::category();
    }

    QtInfo qtInfo() const noexcept
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
