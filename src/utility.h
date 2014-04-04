#ifndef UTILITY_H
#define UTILITY_H

#include <QtCore/QObject>
#include <QtCore/QSettings>
#include <QtCore/QUrl>

#include "QJson/parser.h"

class Utility : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString appVersion READ appVersion CONSTANT FINAL)
    Q_PROPERTY(int qtVersion READ qtVersion CONSTANT FINAL)

public:
    static Utility* Instance();
    ~Utility();

    QString appVersion() const;
    int qtVersion() const;
    void clearSettings();
    QVariant jsonParse(const QByteArray& jsonData, bool* ok = 0);
    QByteArray gUncompress(const QByteArray &data);

    Q_INVOKABLE QVariant getValue(const QString &key, const QVariant defaultValue = QVariant());
    Q_INVOKABLE void setValue(const QString &key, const QVariant &value);
    Q_INVOKABLE QUrl fixUrl(const QUrl &url);

private:
    explicit Utility(QObject *parent = 0);

private:
    QSettings settings;
    QJson::Parser parser;
};

#endif // UTILITY_H
