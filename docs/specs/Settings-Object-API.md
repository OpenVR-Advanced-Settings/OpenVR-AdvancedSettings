## Problem

The current method of saving logically connected variables is to use the QSettings API directly.
This leads to large amounts of code duplication since every controller has to have their own implementation.

It also leads to slow development since new features will usually have to implement their own settings system.

## Considerations

The API should be resistant to internal changes.

The API should be simple to use.

## Requirements

The API MUST allow the permanent storage of objects. Internal changes to objects MUST NOT be a breaking change.

## Public API

The public API is in the `settings_object.h` file in the `settings` namespace.

The API consists of two parts, an imperative set of functions and a virtual class that objects must implement.

The imperative functions are straightforward.
```cpp
void saveObject( const ISettingsObject& obj );

void loadObject( ISettingsObject& obj );

void saveNumberedObject( const ISettingsObject& obj, const int slot );

void loadNumberedObject( ISettingsObject& obj, const int slot );

int getAmountOfSavedObjects( ISettingsObject& obj );
```
The `numberedObject` versions can be used for saving an arbitrary amount of objects, retrievable with the same `slot`.

`getAmountOfSavedObjects` will return the amount of objects, starting from _1_, that exist contiguously.
Will return `0` if no objects have been saved as `saveNumberedObject(obj, 1)`.

The `ISettingsObject`s will have to be implemented by the object to be saved.
```cpp
class ISettingsObject
{
    public:
        virtual ~ISettingsObject() {}

        virtual SettingsObjectData saveSettings() const = 0;

        virtual void loadSettings( SettingsObjectData& obj ) = 0;

        virtual std::string settingsName() const = 0;
};
```

The `settingsName()` is used internally for correctly identifying objects and it should be globally unique.

The `ISettingsObject` communicates with the API through a `SettingsObjectData` object, which contains the variables of the object in a First-In First-Out (FIFO) format.

The contents of the object can be set or retrieved through three templated functions.
```cpp
class SettingsObjectData
{
    public:
        template <typename Value> void addValue( const Value value );

        template <typename Value>
            Value getNextValueOrDefault( const Value defaultValue );

        template <typename Value> bool hasValuesOfType();
}
```
The different types are stored differently, so `addValue<int>()`, `addValue<double>()`, `getNextValueOrDefault<double>()` would return the value added second, but the first double value.

An example implementation for a struct can be found below.
```cpp
struct A : settings::ISettingsObject
{
    bool val1 = true;
    int val2 = 1123;
    double val3 = 1.0003;
    double val4 = 1.0004;
    double val5 = 1.0005;
    std::string val6 = "val6";

    std::string settingsName() const override
    {
        return "SettingsTestStruct";
    }

    void loadSettings( settings::SettingsObjectData& obj ) override
    {
        val1 = obj.getNextValueOrDefault( false );
        val2 = obj.getNextValueOrDefault( 1 );
        val3 = obj.getNextValueOrDefault( 3.0 );
        val4 = obj.getNextValueOrDefault( 4.0 );
        val5 = obj.getNextValueOrDefault( 5.0 );
        val6 = obj.getNextValueOrDefault<std::string>( "default" );
    }

    settings::SettingsObjectData saveSettings() const override
    {
        settings::SettingsObjectData o;
        o.addValue( val1 );
        o.addValue( val2 );
        o.addValue( val3 );
        o.addValue( val4 );
        o.addValue( val5 );
        o.addValue( val6 );

        return o;
    }
};
```
Values of different types can be returned in any order, although it is recommended to save and load settings in the same order.

## Other Solutions

Simply copying the contents of an object (think direct `memcpy()`) was considered but was not chosen because every addition or removal of a struct member would be a breaking change. 
Additionally, the C++ compiler is free to insert padding in the struct, meaning different compilations could interpret data differently.
