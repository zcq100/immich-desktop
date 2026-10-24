#include "core/AppSettings.h"
#include "core/I18n.h"

#include <QSettings>
#include <QTemporaryDir>
#include <QtTest>

using Aurora::I18n::AppLanguage;

class AppSettingsTest final : public QObject {
    Q_OBJECT

private slots:
    void languageRoundTrip();
    void unknownLanguageFallsBackToSystem();
};

void AppSettingsTest::languageRoundTrip()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, dir.path());

    Aurora::LanguageSettings language;
    language.language = AppLanguage::SimplifiedChinese;
    Aurora::AppSettings().saveLanguage(language);

    QCOMPARE(Aurora::AppSettings().loadLanguage().language, AppLanguage::SimplifiedChinese);
}

void AppSettingsTest::unknownLanguageFallsBackToSystem()
{
    QTemporaryDir dir;
    QVERIFY(dir.isValid());
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, dir.path());

    {
        QSettings raw(QSettings::IniFormat, QSettings::UserScope,
                      QStringLiteral("Immich"), QStringLiteral("immich"));
        raw.setValue(QStringLiteral("appearance/language"), QStringLiteral("fr"));
        raw.sync();
    }

    QCOMPARE(Aurora::AppSettings().loadLanguage().language, AppLanguage::System);
}

QTEST_GUILESS_MAIN(AppSettingsTest)

#include "AppSettingsTest.moc"
