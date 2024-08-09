#include <QSettings>
#include <type_traits>
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
    auto& qset = settings::getQSettings();

    qset.beginGroup( structName.c_str() );
    qset.beginWriteArray( typeName.c_str() );

    for ( int i = 0; !values.empty(); ++i )
    {
        qset.setArrayIndex( i );
        if constexpr ( std::is_same_v<std::string, Value> )
        {
            qset.setValue( typeName.c_str(), values.front().c_str() );
        }
        else
        {
            qset.setValue( typeName.c_str(), values.front() );
        }
        values.pop_front();
    }

    qset.endArray();
    qset.endGroup();
}
template <typename Value>
std::list<Value> loadListFromDisk( const std::string structName,
                                   const std::string typeName )
{
    const auto isBool = std::is_same_v<bool, Value>;
    const auto isInt = std::is_same_v<int, Value>;
    const auto isDouble = std::is_same_v<double, Value>;
    const auto isFloat = std::is_same_v<float, Value>;
    const auto isString = std::is_same_v<std::string, Value>;

    static_assert( !isFloat,
                   "'float' is not supported. Use 'double' instead." );

    static_assert( isBool || isInt || isDouble || isString,
                   "Type is not supported for the settings object." );

    auto& qset = settings::getQSettings();

    qset.beginGroup( structName.c_str() );
    auto size = qset.beginReadArray( typeName.c_str() );

    std::list<Value> list;
    // NOLINTNEXTLINE(altera-id-dependent-backward-branch)
    for ( int index = 0; index < size; index++ )
    {
        qset.setArrayIndex( index );
        auto val = qset.value( typeName.c_str() );

        if constexpr ( isBool )
        {
            list.push_back( val.toBool() );
        }
        else if constexpr ( isInt )
        {
            list.push_back( val.toInt() );
        }
        else if constexpr ( isDouble )
        {
            list.push_back( val.toDouble() );
        }
        else if constexpr ( isString )
        {
            list.push_back( val.toString().toStdString() );
        }
    }

    qset.endArray();
    qset.endGroup();

    return list;
}

settings::SettingsObjectData loadSettingsObject( std::string objName )
{
    settings::SettingsObjectData sod;

    auto boolValues = loadListFromDisk<bool>( objName, "bools" );
    addListToObject( boolValues, sod );

    auto intValues = loadListFromDisk<int>( objName, "ints" );
    addListToObject( intValues, sod );

    auto doubleValues = loadListFromDisk<double>( objName, "doubles" );
    addListToObject( doubleValues, sod );

    auto stringValues = loadListFromDisk<std::string>( objName, "strings" );
    addListToObject( stringValues, sod );

    return sod;
}

void saveSettingsObject( settings::SettingsObjectData& sod,
                         std::string objName )
{
    auto boolValues = createListFromObject<bool>( sod );
    saveListToDisk( boolValues, objName, "bools" );

    auto intValues = createListFromObject<int>( sod );
    saveListToDisk( intValues, objName, "ints" );

    auto doubleValues = createListFromObject<double>( sod );
    saveListToDisk( doubleValues, objName, "doubles" );

    auto stringValues = createListFromObject<std::string>( sod );
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
    auto sod = obj.saveSettings();
    saveSettingsObject( sod, obj.settingsName() );
}

void loadObject( ISettingsObject& obj )

{
    auto sod = loadSettingsObject( obj.settingsName() );
    obj.loadSettings( sod );
}

void saveNumberedObject( const ISettingsObject& obj, const int slot )
{
    auto sod = obj.saveSettings();
    saveSettingsObject(
        sod, appendSlotNumberToSettingsName( obj.settingsName(), slot ) );
}

void loadNumberedObject( ISettingsObject& obj, const int slot )
{
    auto sod = loadSettingsObject(
        appendSlotNumberToSettingsName( obj.settingsName(), slot ) );
    obj.loadSettings( sod );
}

int getAmountOfSavedObjects( ISettingsObject& obj )
{
    auto& sod = getQSettings();

    auto groups = sod.childGroups();

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
