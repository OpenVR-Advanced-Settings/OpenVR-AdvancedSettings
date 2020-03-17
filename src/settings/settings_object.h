#pragma once
#include "internal/settings_object_data.h"

namespace settings
{
/*!
   \brief The ISettingsObject class is used for permanently storing objects.

   Example:
   \code
    struct A : settings::ISettingsObject
    {
        bool val1 = false;
        bool val2 = true;
        double val3 = 3.0;

        void loadSettings( settings::SettingsObjectData& obj ) override
        {
            val1 = obj.getNextValueOrDefault( false );
            val2 = obj.getNextValueOrDefault( true);
            val3 = obj.getNextValueOrDefault( 2.0 );
        }

        settings::SettingsObjectData saveSettings() const override
        {
            settings::SettingsObjectData o;

            o.addValue( val1 );
            o.addValue( val2 );
            o.addValue( val3 );

            return o;
        }
    };
   \endcode
 */
class ISettingsObject
{
public:
    // Destructor does not have to be implemented.
    virtual ~ISettingsObject() {}
    /*!
      \brief Function to convert from parent object to \c
       SettingsObjectData.
      \return Returns a \SettingsObjectData containing
       object member values.

      See \c ISettingsObject class for an example.
     */
    virtual SettingsObjectData saveSettings() const = 0;

    /*!
       \brief Function to convert from \c SettingsObjectData to parent object.
       \param obj \c SettingsObjectData to load from.

       See \c ISettingsObject class for an example.
     */
    virtual void loadSettings( SettingsObjectData& obj ) = 0;

    /*!
       \brief Name used as an identifier when saving/loading. Should be unique
       globally.
       \return Internal identifier for saving/loading.
     */
    virtual std::string settingsName() const = 0;
};

/*!
   \brief Saves \a obj to permanent storage.
   \param obj Object to save.
 */
void saveObject( const ISettingsObject& obj );

/*!
   \brief Loads \a obj from permanent storage.
   \param obj Object to load.
 */
void loadObject( ISettingsObject& obj );

/*!
   \brief Saves \a obj to permanent storage in a numbered \a slot.
   \param obj Object to save.
   \param slot Specific object to save.

   Saves an object as the settings name, but with \a slot appended to
   allow multiple versions of the same object to be saved.

   Objects can be saved to arbitrary numbers, they do not have to be
   consecutive, although not saving consecutively breaks the
   \c getAmountOfSavedObjects function.
 */
void saveNumberedObject( const ISettingsObject& obj, const int slot );

/*!
   \brief Saves \a vec of \c ISettingsObject.
   \param vec Vector of \c ISettingsObject derived objects.

   Saves the objects starting from \c slot 1 to \c slot \c{1 + vec.size()}.
*/
template <class T> void saveAllObjects( const std::vector<T> vec )
{
    static_assert(
        std::is_base_of<ISettingsObject, T>::value,
        "Only objects that inherit from ISettingsObject can be saved." );

    auto i = 1;
    for ( auto& p : vec )
    {
        saveNumberedObject( p, i );
        ++i;
    }
}

/*!
   \brief Loads \a vec of \c ISettingsObject.
   \param vec Vector of \c ISettingsObject derived objects.

   Loads the objects starting from \c slot 1 to the last contiguous object.
*/
template <class T> void loadAllObjects( std::vector<T>& vec )
{
    static_assert(
        std::is_base_of<ISettingsObject, T>::value,
        "Only objects that inherit from ISettingsObject can be loaded." );

    vec.clear();

    // We'll need to know which name the object is saved under.
    // We could check vec.at(0), but it's not guaranteed to exist.
    auto o = T{};
    auto profileCount = getAmountOfSavedObjects( o );
    for ( int profileNumber = 1; profileNumber <= profileCount;
          ++profileNumber )
    {
        vec.emplace_back();

        const auto index = static_cast<size_t>( profileNumber - 1 );

        loadNumberedObject( vec.at( index ), profileNumber );
    }
}

/*!
   \brief Loads \a obj from permanent storage in a numbered \a slot.
   \param obj Object to load.
   \param slot Specific object to load.

   Loads an object with the settings name, but with \a slot appended to
   allow multiple versions of the same object to be saved.

   Objects can be loaded from arbitrary numbers, they do not have to be
   consecutive.
 */
void loadNumberedObject( ISettingsObject& obj, const int slot );

/*!
   \brief Gets amount of consecutive \a obj objects starting from 1.
   \param obj Object to check.
   \return Amount of consecutive \obj objects.

   The function will start at 1, and increment until it can't find any
   objects of the \a obj type.

   So if a certain object has been saved to \c slot 1, 2 and 3 the
   function will return 3.

   If objects have been saved to 100, 101 and 102 the function will
   return 0.

   If an object has been saved with the \c saveObject function this function
   will return 0.
 */
int getAmountOfSavedObjects( ISettingsObject& obj );

} // namespace settings
