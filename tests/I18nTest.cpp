#include "core/I18n.h"

#include <QCoreApplication>
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

QTEST_GUILESS_MAIN(I18nTest)

#include "I18nTest.moc"
