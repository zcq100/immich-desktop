#pragma once

#include <QString>

namespace Aurora::I18n {

enum class AppLanguage {
    System,
    English,
    SimplifiedChinese,
};

// Resolves the effective locale name: "en" or "zh_CN".
// Only Simplified Chinese translations are provided, so every Chinese system
// resolves to zh_CN.
QString resolveLocaleName(AppLanguage language);

// Configuration value <-> enum. Unknown configuration values fall back to System.
AppLanguage languageFromKey(const QString &key);
QString languageKey(AppLanguage language);

// Dropdown display name. English / Simplified Chinese are written in their own
// language; the System entry uses the current UI language ("Follow system").
QString displayName(AppLanguage language);

// Installs the application translations and Qt's own translations, and sets the
// default QLocale. Returns the effective locale name. Missing translation files
// degrade silently.
QString installTranslators(AppLanguage language);

} // namespace Aurora::I18n
