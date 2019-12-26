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
        std::list<Value>& list = getListObject<Value>();

        list.push_back( value );
    }

    template <typename Value>
    Value getNextValueOrDefault( const Value defaultValue )
    {
        Value returnedValue = defaultValue;

        std::list<Value>& list = getListObject<Value>();

        if ( hasValuesOfType<Value>() )
        {
            returnedValue = list.front();
            list.pop_front();
        }

        return returnedValue;
    }

    template <typename Value> bool hasValuesOfType()
    {
        std::list<Value>& list = getListObject<Value>();

        return !list.empty();
    }

private:
    template <typename Value> std::list<Value>& getListObject()
    {
        using std::is_same;
        const auto isBool = is_same<bool, Value>::value;
        const auto isInt = is_same<int, Value>::value;
        const auto isDouble = is_same<double, Value>::value;
        const auto isString = is_same<std::string, Value>::value;

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
