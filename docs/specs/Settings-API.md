## Problem

The current solution for storing and getting settings is to directly use the Qt QSettings object.
This leads to extremely verbose code for something that really should just be a single function call.

While the Qt settings management abstracts low level details very well, an even higher level API should be sought after.

The current system has stringly typed settings, which means it's possible to save the `desktopWidth` setting, but attempt to use the `deskWidth` setting somewhere else without noticing.

Currently, there are many different ways for a variable to be initialized depending on what goes wrong (QML, header file initialization, constructor, QSettings loading).
There should only default option when none other are available.

## Considerations for the new Settings API

The public facing API should be decoupled from the Qt API, in case we want to use another API later.

There should be a single correct way of referring to a setting.
The public API should not concern itself with how the settings are stored (.ini file/database/PROM).

The public API should be simple, and return error checked, ready values.

The settings should be logically grouped.

Ideally, the type system is used to ensure that a setting isn't used as the wrong type (at compile time?).

Ideally, it should not be possible to attempt to retrieve non-existent settings.

## Requirements

The API MUST transparently allow the storage of persistent values. The performance impact during runtime SHOULD be as small as possible.

Settings set using the API MUST be recoverable in the event of an unexpected program termination.

The API MUST allow for easy addition of new settings, with minimal vectors for error.

The API MUST type check the values, so that it is not possible to misinterpret data.

The API MUST have a default value for all settings that is used in case previously stored values are unavailable.

## Public API

The public API is in the `settings.h` file in the `settings` namespace.

The settings are represented as enums according to type.
Each option is prefixed with their general category (`PLAYSPACE_`, etc.).
```cpp
enum class BoolSetting
{
    PLAYSPACE_lockXToggle,
    PLAYSPACE_lockYToggle,
    PLAYSPACE_lockZToggle,

    [...]

    CHAPERONE_disableChaperone,
    // LAST_ENUMERATOR must always be set to the last value
    LAST_ENUMERATOR = CHAPERONE_disableChaperone,
};

enum class DoubleSetting
{
    UTILITY_desktopWidth,

    [...]

    CHAPERONE_fadeDistanceRemembered,
    // LAST_ENUMERATOR must always be set to the last value
    LAST_ENUMERATOR = CHAPERONE_fadeDistanceRemembered,
};

enum class StringSetting
{
    KEYBOARDSHORTCUT_keyboardOne,
    KEYBOARDSHORTCUT_keyboardTwo,
    KEYBOARDSHORTCUT_keyboardThree,
    // LAST_ENUMERATOR must always be set to the last value
    LAST_ENUMERATOR = KEYBOARDSHORTCUT_keyboardThree,
};

enum class IntSetting
{
    UTILITY_alarmHour,
    UTILITY_alarmMinute,
    // LAST_ENUMERATOR must always be set to the last value
    LAST_ENUMERATOR = UTILITY_alarmMinute,
};
```
The order of the of the enumerators matter, and they can't be reordered without leading to a runtime error.
The `LAST_ENUMERATOR` is a result of C++ enums not knowing how many enumerators they have, and it is required for the implementation to work correctly.

Values can be accessed with imperative functions.
```cpp
[[nodiscard]] bool getSetting( const BoolSetting setting );
void setSetting( const BoolSetting setting, const bool value );

[[nodiscard]] double getSetting( const DoubleSetting setting );
void setSetting( const DoubleSetting setting, const double value );

[[nodiscard]] int getSetting( const IntSetting setting );
void setSetting( const IntSetting setting, const int value );

[[nodiscard]] std::string getSetting( const StringSetting setting );
void setSetting( const StringSetting setting, const std::string value );
```
The `[[nodiscard]]` means that it's a compiler error to not use the return value of the function.
Since none of the getters have side effects it would always be wrong to call the function without using the value.

Additionally, there are three utility functions.
```cpp
std::string initializeAndGetSettingsPath();

void saveChangedSettings();

void saveAllSettings();
```
`saveChangedSettings()` only writes settings to disk that have had their values changed through the API.
`saveAllSettings()` writes all known settings to disk.

`initializeAndGetSettingsPath()` initializes the API and returns the full path of the settings file.
It is not a requirement for this function to be called for the API to work.
If it is not called, the API will be initialized with the first call to the API.
The function is used as a means of controlling when the API will be initialized, as well as getting the current file path.

## Implementation

The public imperative API is an abstraction for the implementation, which is contained in a `SettingsController`.
The `SettingsController` object is a `static` object inside the `settings.cpp` compilation unit.

This is done to remove the limitations that an object oriented design would bring, namely lifetime and scope management.
Since the API is supposed to be globally available throughout the lifetime of the program, creating it as an object in the `OverlayController` and passing it to subcontrollers would only introduce unnecessary parameter passing.
The `SettingsController` also relies on containing the settings as mutable state, and doing so in a purely imperative way would be cumbersome.

The API currently supports `int`, `double`, `std::string` and `bool` values. Below the `int` version is examined but the others are identical in implementation.

Settings are stored as an array of `*SettingValue`s the size of the `*Setting` enum.
```cpp
constexpr static auto intSettingsSize
    = static_cast<int>( IntSetting::LAST_ENUMERATOR ) + 1;

std::array<IntSettingValue, intSettingsSize> m_intSettings{
        IntSettingValue{ IntSetting::UTILITY_alarmHour,
        SettingCategory::Utility,
        QtInfo{ "alarmHour" },
        0 },
    IntSettingValue{ IntSetting::UTILITY_alarmMinute,
        SettingCategory::Utility,
        QtInfo{ "alarmMinute" },
        0 },
};
```
Here, for each setting, the following is defined:
* `setting`: Which enumerator the setting has.
* `category`: Which category the setting belongs to (used for QSettings categories).
* `QtInfo`: A struct that contains Qt specific information. For now only a string value containing the name of the setting.
* The default value of the setting.

The `*SettingValue` classes are template specializations of the `SpecificSettingValue` class.
```cpp
using IntSettingValue = SpecificSettingValue<int, IntSetting>;
```

The `SpecificSettingValue` class contains getters for the `setting`, `category` and `qtInfo` fields, as well as a getter+setter for the `value` field.
```cpp
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
    // Attempt to load saved setting
    // Use default if none saved
    // Save setting if not saved to make it appear in .ini file
}
```
The templates used are not very complex. For the `IntSettingValue` you would simply mentally replace `Setting` with `IntSetting` and `Value` with `int`.
The templates reduce code duplication by a very large amount, which is why they are used.

The `SpecificSettingValue` inherits from the `SettingValue` base class. This class only contains fields for `category` and `qtInfo`.
The inheritance from `SettingValue` is necessary in order to make the `saveChangedValues()` easier to implement.

When a setting has changed value it is added to an array of `SettingValue`s (the base class).
It is possible to call `saveValue()` on all derived classes since it has been defined as a `virtual` function in the base class.
This is done in the `saveChangedValues()` function.

Templates might seem daunting if when they are encountered for the first time, but the usage here is not very advanced.
```cpp
template <typename ReturnType, typename Setting>
[[nodiscard]] ReturnType getSetting( const Setting setting ) const noexcept
{
    const auto index = static_cast<std::size_t>( setting );

    if constexpr ( std::is_same<Setting, BoolSetting>::value )
    {
        return m_boolSettings[index].value();
    }
    else if constexpr ( std::is_same<Setting, DoubleSetting>::value )
    {
        return m_doubleSettings[index].value();
    }
    else if constexpr ( std::is_same<Setting, IntSetting>::value )
    {
        return m_intSettings[index].value();
    }
    else if constexpr ( std::is_same<Setting, StringSetting>::value )
    {
        return m_stringSettings[index].value();
    }
}
```
The above is the generalized `getSetting()` function that takes a generalized `Setting` as a parameter, and returns a generalized `ReturnType`.

Following the `IntSettingValue` example we would pass an `IntSetting` enum to `Setting` and `int` to `ReturnType`.

All our enums can be cast as integers, so we do that outside of the big `if` block.

Next we need to specify which array we index in to get the relevant setting, so we use `if constexpr` and `std::is_same<A, B>::value` to find which type we are dealing with.
This will compile different functions for the different types, so the `if constexpr` statement would not get executed.

The alternative to this would be having `n` amount of specific functions with predefined types.
This is done in the public API for explicitness, but internally it is much easier to maintain a single function.

The above function compiles down to the equivalent of four manually typed functions.

