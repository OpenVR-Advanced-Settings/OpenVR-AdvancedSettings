#pragma once
#include <type_traits>
#include "setting_value.h"
#include "../settings.h"

namespace settings
{
template <typename Value, typename Setting>
class SpecificSettingValue : public SettingValue
{
public:
    SpecificSettingValue( const Setting setting,
                          const SettingCategory category,
                          const QtInfo qtInfo,
                          const Value defaultValue )
        : SettingValue( category, qtInfo ), m_setting( setting ),
          m_value( defaultValue )
    {
        // Only allow explicitly defined types to curtail unintended behavior
        constexpr auto isBool = std::is_same<Value, bool>::value;
        constexpr auto isDouble = std::is_same<Value, double>::value;
        constexpr auto isString = std::is_same<Value, std::string>::value;
        constexpr auto isInt = std::is_same<Value, int>::value;
        static_assert( isBool || isDouble || isString || isInt );

        const auto v = getQtSetting( SettingValue::category(),
                                     SettingValue::qtInfo().settingName );

        if ( isValidQVariant<Value>( v ) )
        {
            if constexpr ( std::is_same<Value, bool>::value )
            {
                m_value = v.toBool();
            }

            else if constexpr ( std::is_same<Value, double>::value )
            {
                m_value = v.toDouble();
            }

            else if constexpr ( std::is_same<Value, std::string>::value )
            {
                m_value = v.toString().toStdString();
            }

            else if constexpr ( std::is_same<Value, int>::value )
            {
                m_value = v.toInt();
            }
        }

        else
        {
            // If setting doesn't exist, create it automatically
            saveValue();
        }
    }

    void setValue( const Value value ) noexcept
    {
        m_value = value;
    }

    [[nodiscard]] Value value() const noexcept
    {
        return m_value;
    }

    [[nodiscard]] Setting setting() const noexcept
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
        if constexpr ( !std::is_same<Value, std::string>::value )
        {
            saveQtSetting( SettingValue::category(),
                           SettingValue::qtInfo().settingName,
                           m_value );
        }
        else
        {
            // Special case for std::string because it can't be auto
            // converted to QVariant
            saveQtSetting( SettingValue::category(),
                           SettingValue::qtInfo().settingName,
                           m_value.c_str() );
        }
    }

private:
    const Setting m_setting;
    Value m_value;
};

using BoolSettingValue = SpecificSettingValue<bool, BoolSetting>;
using DoubleSettingValue = SpecificSettingValue<double, DoubleSetting>;
using IntSettingValue = SpecificSettingValue<int, IntSetting>;
using StringSettingValue = SpecificSettingValue<std::string, StringSetting>;

} // namespace settings
