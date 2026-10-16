#include "translations.h"
#include "paths.h"
#include "../settings/settings.h"
#include <QCoreApplication>
#include <QFont>
#include <QFontDatabase>
#include <QGuiApplication>
#include <QLocale>
#include <QTranslator>
#include <array>
#include <string>
#include <QtLogging>
#include <QtDebug>

namespace
{
// Must match RESOURCE_PREFIX and the .ts file base name in CMakeLists.txt.
constexpr auto kResourceDir = ":/i18n";
constexpr auto kFileBaseName = "AdvancedSettings";

QTranslator g_translator;
QString g_currentLanguage = translations::kSourceLanguage;

// Bundled CJK fonts, copied to "fonts/" next to the executable by CMake.
struct CjkFont
{
    const char* locale;
    const char* fileName;
    QString family; // Set once loaded.
};
std::array<CjkFont, 3> g_cjkFonts{ {
    { "ja", "NotoSansJP-Regular.otf", {} },
    { "zh_CN", "NotoSansSC-Regular.otf", {} },
    { "zh_TW", "NotoSansTC-Regular.otf", {} },
} };
QString g_baseFontFamily;

void loadCjkFonts()
{
    g_baseFontFamily = QGuiApplication::font().family();
    for ( auto& font : g_cjkFonts )
    {
        const auto path = paths::binaryDirectoryFindFile(
            std::string( "fonts/" ) + font.fileName );
        if ( !path.has_value() )
        {
            continue;
        }
        const auto id
            = QFontDatabase::addApplicationFont( QString::fromStdString( *path ) );
        const auto families = QFontDatabase::applicationFontFamilies( id );
        if ( families.isEmpty() )
        {
            qWarning() << "Could not load font" << path->c_str();
            continue;
        }
        font.family = families.first();
    }
}

// Keeps the platform UI font for Latin text and falls back to the CJK fonts
// for everything else. The active language's font goes first, because Han
// characters shared between Japanese and Chinese are drawn differently in each.
void applyFont()
{
    QStringList families{ g_baseFontFamily };
    for ( const auto& font : g_cjkFonts )
    {
        if ( !font.family.isEmpty() && g_currentLanguage == font.locale )
        {
            families.append( font.family );
        }
    }
    for ( const auto& font : g_cjkFonts )
    {
        if ( !font.family.isEmpty() && g_currentLanguage != font.locale )
        {
            families.append( font.family );
        }
    }
    auto appFont = QGuiApplication::font();
    appFont.setFamilies( families );
    QGuiApplication::setFont( appFont );
}

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

void installFromSettingsOrSystem()
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
} // namespace

namespace translations
{
void initialize()
{
    loadCjkFonts();
    installFromSettingsOrSystem();
    applyFont();
}

bool setLanguage( const QString& locale )
{
    const bool ok = install( locale );
    applyFont();
    settings::setSetting( settings::StringSetting::APPLICATION_language,
                          g_currentLanguage.toStdString() );
    return ok;
}

QString currentLanguage()
{
    return g_currentLanguage;
}
} // namespace translations
