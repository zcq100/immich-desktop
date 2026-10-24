#include "core/I18n.h"

#include <QCoreApplication>
#include <QLocale>
#include <QtTest>

using Aurora::I18n::AppLanguage;

class I18nTest final : public QObject {
    Q_OBJECT

private slots:
    void explicitLanguagesResolve();
    void systemLanguageResolvesToSupportedLocale();
    void languageKeysRoundTrip();
    void unknownKeyFallsBackToSystem();
    void displayNames();
    void systemModePreservesSystemLocale();
    void explicitLanguageOverridesLocale();

private:
    static bool isSupportedLocale(const QString &name)
    {
        return name == QStringLiteral("en") || name == QStringLiteral("zh_CN");
    }
};

void I18nTest::explicitLanguagesResolve()
{
    QCOMPARE(Aurora::I18n::resolveLocaleName(AppLanguage::English), QStringLiteral("en"));
    QCOMPARE(Aurora::I18n::resolveLocaleName(AppLanguage::SimplifiedChinese),
             QStringLiteral("zh_CN"));
}

void I18nTest::systemLanguageResolvesToSupportedLocale()
{
    const QString name = Aurora::I18n::resolveLocaleName(AppLanguage::System);
    QVERIFY(isSupportedLocale(name));
}

void I18nTest::languageKeysRoundTrip()
{
    for (const AppLanguage language : {AppLanguage::System, AppLanguage::English,
                                       AppLanguage::SimplifiedChinese}) {
        const QString key = Aurora::I18n::languageKey(language);
        QCOMPARE(Aurora::I18n::languageFromKey(key), language);
    }
}

void I18nTest::unknownKeyFallsBackToSystem()
{
    QCOMPARE(Aurora::I18n::languageFromKey(QStringLiteral("fr")), AppLanguage::System);
    QCOMPARE(Aurora::I18n::languageFromKey(QString()), AppLanguage::System);
}

void I18nTest::displayNames()
{
    QCOMPARE(Aurora::I18n::displayName(AppLanguage::English), QStringLiteral("English"));
    QCOMPARE(Aurora::I18n::displayName(AppLanguage::SimplifiedChinese),
             QString::fromUtf8("简体中文"));
    QVERIFY(!Aurora::I18n::displayName(AppLanguage::System).isEmpty());
}

void I18nTest::systemModePreservesSystemLocale()
{
    const QLocale original = QLocale();
    QLocale::setDefault(QLocale(QStringLiteral("de_DE")));
    Aurora::I18n::installTranslators(AppLanguage::System);
    QCOMPARE(QLocale().name(), QStringLiteral("de_DE"));
    QLocale::setDefault(original);
}

void I18nTest::explicitLanguageOverridesLocale()
{
    QLocale::setDefault(QLocale(QStringLiteral("de_DE")));
    Aurora::I18n::installTranslators(AppLanguage::English);
    // Qt resolves the bare "en" locale to en_US, so assert the language rather
    // than the territorial name.
    QCOMPARE(QLocale().language(), QLocale::English);

    Aurora::I18n::installTranslators(AppLanguage::SimplifiedChinese);
    QCOMPARE(QLocale().name(), QStringLiteral("zh_CN"));
    QLocale::setDefault(QLocale());
}

QTEST_GUILESS_MAIN(I18nTest)

#include "I18nTest.moc"
