#pragma once
#include <QString>

namespace translations
{
// Source language of all qsTr()/tr() strings. Needs no translator.
constexpr auto kSourceLanguage = "en";

// Registers the bundled CJK fonts and installs the translator for the language
// saved in settings, or the system locale if none is saved. Must be called
// after the QApplication is created and before any QML is loaded.
void initialize();

// Swaps the active translator to the given locale (e.g. "de") and saves it.
// Returns false if no translation exists for it, falling back to the source
// language. Callers must call QQmlEngine::retranslate() afterwards.
bool setLanguage( const QString& locale );

// Locale of the active translation, kSourceLanguage if none is installed.
[[nodiscard]] QString currentLanguage();
} // namespace translations
