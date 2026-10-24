#include "core/I18n.h"

#include <QCoreApplication>
#include <QLibraryInfo>
#include <QLocale>
#include <QTranslator>

namespace Aurora::I18n {
namespace {

QString systemLocaleName()
{
    return QLocale::system().name().startsWith(QStringLiteral("zh"))
               ? QStringLiteral("zh_CN")
               : QStringLiteral("en");
}

} // namespace

QString resolveLocaleName(AppLanguage language)
{
    switch (language) {
    case AppLanguage::English:
        return QStringLiteral("en");
    case AppLanguage::SimplifiedChinese:
        return QStringLiteral("zh_CN");
    case AppLanguage::System:
        break;
    }
    return systemLocaleName();
}

AppLanguage languageFromKey(const QString &key)
{
    if (key == QStringLiteral("en"))
        return AppLanguage::English;
    if (key == QStringLiteral("zh_CN") || key == QStringLiteral("zh"))
        return AppLanguage::SimplifiedChinese;
    return AppLanguage::System;
}

QString languageKey(AppLanguage language)
{
    switch (language) {
    case AppLanguage::English:
        return QStringLiteral("en");
    case AppLanguage::SimplifiedChinese:
        return QStringLiteral("zh_CN");
    case AppLanguage::System:
        break;
    }
    return QStringLiteral("system");
}

QString displayName(AppLanguage language)
{
    switch (language) {
    case AppLanguage::English:
        return QStringLiteral("English");
    case AppLanguage::SimplifiedChinese:
        return QString::fromUtf8("简体中文");
    case AppLanguage::System:
        break;
    }
    return QCoreApplication::translate("I18n", "Follow system");
}

QString installTranslators(AppLanguage language)
{
    const QString localeName = resolveLocaleName(language);
    // Following the system language must not clobber the user's formatting
    // locale: on e.g. a German system there is no German translation, so the
    // resolved name is "en", but dates and numbers should stay German.
    if (language != AppLanguage::System)
        QLocale::setDefault(QLocale(localeName));
    if (localeName == QStringLiteral("en"))
        return localeName;

    auto *application = QCoreApplication::instance();
    if (!application)
        return QStringLiteral("en");

    auto *appTranslator = new QTranslator(application);
    if (appTranslator->load(QStringLiteral(":/i18n/immich_%1.qm").arg(localeName)))
        application->installTranslator(appTranslator);
    else
        delete appTranslator;

    // Qt's built-in standard dialog strings (qtbase_*.qm). Stay in English if
    // they cannot be found.
    auto *qtTranslator = new QTranslator(application);
    const QString qtTranslationsPath = QLibraryInfo::path(QLibraryInfo::TranslationsPath);
    if (qtTranslator->load(QStringLiteral("qtbase_%1").arg(localeName), qtTranslationsPath))
        application->installTranslator(qtTranslator);
    else
        delete qtTranslator;

    return localeName;
}

} // namespace Aurora::I18n
