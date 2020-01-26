#pragma once
#include <list>
#include <easylogging++.h>

namespace settings
{
class SettingsObjectData
{
public:
    template <typename Value> void addValue( const Value value )
    {
        auto& list = getListObject<Value>();

        list.push_back( value );
    }

    template <typename Value>
    auto getNextValueOrDefault( const Value defaultValue )
    {
        auto& list = getListObject<Value>();

        if ( hasValuesOfType<Value>() )
        {
            auto returnedValue = list.front();
            list.pop_front();
            return returnedValue;
        }
        if constexpr ( std::is_same<const char*, Value>::value )
        {
            return std::string( defaultValue );
        }
        else
        {
            return defaultValue;
        }
    }

    template <typename Value> bool hasValuesOfType()
    {
        auto& list = getListObject<Value>();

        return !list.empty();
    }

    template <typename Value> void consumeDeprecatedValue()
    {
        getNextValueOrDefault( Value{} );
    }

    template <typename Value> void addDeprecatedValue( const Value value )
    {
        addValue( value );
    }

private:
    template <typename Value> auto& getListObject()
    {
        using std::is_same;
        const auto isBool = is_same<bool, Value>::value;
        const auto isInt = is_same<int, Value>::value;
        const auto isDouble = is_same<double, Value>::value;
        const auto isFloat = is_same<float, Value>::value;
        const auto isString = is_same<std::string, Value>::value
                              || is_same<const char*, Value>::value;

        static_assert( !isFloat,
                       "'float' is not supported. Use 'double' instead." );

        static_assert( isBool || isInt || isDouble || isString,
                       "Type is not supported for the settings object." );

        if constexpr ( isBool )
        {
            return m_boolValues;
        }
        else if constexpr ( isInt )
        {
            return m_intValues;
        }
        else if constexpr ( isDouble )
        {
            return m_doubleValues;
        }
        else if constexpr ( isString )
        {
            return m_stringValues;
        }
    }

    std::list<bool> m_boolValues;
    std::list<int> m_intValues;
    std::list<double> m_doubleValues;
    std::list<std::string> m_stringValues;
};
} // namespace settings
