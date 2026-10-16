#pragma once
#include <QString>

namespace translations
{
// Source language of all qsTr()/tr() strings. Needs no translator.
constexpr auto kSourceLanguage = "en";

// Installs the translator for the language saved in settings, or the system
// locale if none is saved. Must be called after the QApplication is created.
void initialize();

// Swaps the active translator to the given locale (e.g. "de") and saves it.
// Returns false if no translation exists for it, falling back to the source
// language. Callers must call QQmlEngine::retranslate() afterwards.
bool setLanguage( const QString& locale );

// Locale of the active translation, kSourceLanguage if none is installed.
[[nodiscard]] QString currentLanguage();
} // namespace translations
