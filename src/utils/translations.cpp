#include "translations.h"
#include "../settings/settings.h"
#include <QCoreApplication>
#include <QLocale>
#include <QTranslator>
#include <QtLogging>
#include <QtDebug>

namespace
{
// Must match RESOURCE_PREFIX and the .ts file base name in CMakeLists.txt.
constexpr auto kResourceDir = ":/i18n";
constexpr auto kFileBaseName = "AdvancedSettings";

QTranslator g_translator;
QString g_currentLanguage = translations::kSourceLanguage;

bool install( const QString& locale )
{
    QCoreApplication::removeTranslator( &g_translator );
    g_currentLanguage = translations::kSourceLanguage;

    if ( locale == translations::kSourceLanguage )
    {
        return true;
    }

    if ( !g_translator.load(
             QStringLiteral( "%1_%2" ).arg( kFileBaseName, locale ),
             kResourceDir ) )
    {
        qWarning() << "No translation found for language" << locale;
        return false;
    }

    QCoreApplication::installTranslator( &g_translator );
    g_currentLanguage = locale;
    qInfo() << "Loaded translation" << locale;
    return true;
}
} // namespace

namespace translations
{
void initialize()
{
    const auto saved = QString::fromStdString(
        settings::getSetting( settings::StringSetting::APPLICATION_language ) );
    if ( !saved.isEmpty() )
    {
        install( saved );
        return;
    }

    // No saved choice: try the full system locale (e.g. "pt_BR"), then just
    // its language ("pt").
    const auto system = QLocale::system();
    if ( install( system.name() ) )
    {
        return;
    }
    // Chinese translations are split by script rather than language, so e.g.
    // zh_HK/zh_MO use Traditional and zh_SG uses Simplified.
    if ( system.language() == QLocale::Chinese )
    {
        install( system.script() == QLocale::TraditionalChineseScript
                     ? QStringLiteral( "zh_TW" )
                     : QStringLiteral( "zh_CN" ) );
        return;
    }
    install( QLocale::languageToCode( system.language() ) );
}

bool setLanguage( const QString& locale )
{
    const bool ok = install( locale );
    settings::setSetting( settings::StringSetting::APPLICATION_language,
                          g_currentLanguage.toStdString() );
    return ok;
}

QString currentLanguage()
{
    return g_currentLanguage;
}
} // namespace translations
