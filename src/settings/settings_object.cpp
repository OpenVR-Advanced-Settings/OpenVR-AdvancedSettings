#include <QSettings>
#include "settings_object.h"

namespace settings
{
[[nodiscard]] QSettings& getQSettings();
} // namespace settings

namespace
{
template <typename Value>
std::list<Value> createListFromObject( settings::SettingsObjectData& obj )
{
    std::list<Value> list;
    while ( obj.hasValuesOfType<Value>() )
    {
        list.push_back( obj.getNextValueOrDefault( Value{} ) );
    }
    return list;
}

template <typename Value>
void addListToObject( std::list<Value>& list,
                      settings::SettingsObjectData& obj )
{
    while ( !list.empty() )
    {
        obj.addValue( list.front() );
        list.pop_front();
    }
}

template <typename Value>
void saveListToDisk( std::list<Value> values,
                     const std::string structName,
                     const std::string typeName )
{
    auto& s = settings::getQSettings();

    s.beginGroup( structName.c_str() );
    s.beginWriteArray( typeName.c_str() );

    for ( int i = 0; !values.empty(); ++i )
    {
        s.setArrayIndex( i );
        if constexpr ( std::is_same<std::string, Value>::value )
        {
            s.setValue( typeName.c_str(), values.front().c_str() );
        }
        else
        {
            s.setValue( typeName.c_str(), values.front() );
        }
        values.pop_front();
    }

    s.endArray();
    s.endGroup();
}
template <typename Value>
std::list<Value> loadListFromDisk( const std::string structName,
                                   const std::string typeName )
{
    using std::is_same;
    const auto isBool = is_same<bool, Value>::value;
    const auto isInt = is_same<int, Value>::value;
    const auto isDouble = is_same<double, Value>::value;
    const auto isFloat = is_same<float, Value>::value;
    const auto isString = is_same<std::string, Value>::value;

    static_assert( !isFloat,
                   "'float' is not supported. Use 'double' instead." );

    static_assert( isBool || isInt || isDouble || isString,
                   "Type is not supported for the settings object." );

    auto& s = settings::getQSettings();

    s.beginGroup( structName.c_str() );
    auto size = s.beginReadArray( typeName.c_str() );

    std::list<Value> list;
    for ( int i = 0; i < size; ++i )
    {
        s.setArrayIndex( i );
        auto v = s.value( typeName.c_str() );

        if constexpr ( isBool )
        {
            list.push_back( v.toBool() );
        }
        else if constexpr ( isInt )
        {
            list.push_back( v.toInt() );
        }
        else if constexpr ( isDouble )
        {
            list.push_back( v.toDouble() );
        }
        else if constexpr ( isString )
        {
            list.push_back( v.toString().toStdString() );
        }
    }

    s.endArray();
    s.endGroup();

    return list;
}

settings::SettingsObjectData loadSettingsObject( std::string objName )
{
    settings::SettingsObjectData s;

    auto boolValues = loadListFromDisk<bool>( objName, "bools" );
    addListToObject( boolValues, s );

    auto intValues = loadListFromDisk<int>( objName, "ints" );
    addListToObject( intValues, s );

    auto doubleValues = loadListFromDisk<double>( objName, "doubles" );
    addListToObject( doubleValues, s );

    auto stringValues = loadListFromDisk<std::string>( objName, "strings" );
    addListToObject( stringValues, s );

    return s;
}

void saveSettingsObject( settings::SettingsObjectData& s, std::string objName )
{
    auto boolValues = createListFromObject<bool>( s );
    saveListToDisk( boolValues, objName, "bools" );

    auto intValues = createListFromObject<int>( s );
    saveListToDisk( intValues, objName, "ints" );

    auto doubleValues = createListFromObject<double>( s );
    saveListToDisk( doubleValues, objName, "doubles" );

    auto stringValues = createListFromObject<std::string>( s );
    saveListToDisk( stringValues, objName, "strings" );
}

std::string appendSlotNumberToSettingsName( const std::string name,
                                            const int slot )
{
    return name + "-" + std::to_string( slot );
}

} // namespace

namespace settings
{
void saveObject( const ISettingsObject& obj )
{
    auto s = obj.saveSettings();
    saveSettingsObject( s, obj.settingsName() );
}

void loadObject( ISettingsObject& obj )

{
    auto s = loadSettingsObject( obj.settingsName() );
    obj.loadSettings( s );
}

void saveNumberedObject( const ISettingsObject& obj, const int slot )
{
    auto s = obj.saveSettings();
    saveSettingsObject(
        s, appendSlotNumberToSettingsName( obj.settingsName(), slot ) );
}

void loadNumberedObject( ISettingsObject& obj, const int slot )
{
    auto s = loadSettingsObject(
        appendSlotNumberToSettingsName( obj.settingsName(), slot ) );
    obj.loadSettings( s );
}

int getAmountOfSavedObjects( ISettingsObject& obj )
{
    auto& s = getQSettings();

    auto groups = s.childGroups();

    for ( int i = 1;; ++i )
    {
        const auto groupDoesNotContainSetting = !groups.contains(
            appendSlotNumberToSettingsName( obj.settingsName(), i ).c_str() );
        if ( groupDoesNotContainSetting )
        {
            return i - 1;
        }
    }
}

} // namespace settings
