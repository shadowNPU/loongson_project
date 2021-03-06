#pragma once

#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtCore/QVariant>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonValue>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonParseError>


QJsonValue readJsonData(QString const &jsonPath) {
    QFile file(jsonPath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QByteArray byteArray = file.readAll();
        QJsonParseError jsonError;
        QJsonDocument doc = QJsonDocument::fromJson(byteArray, &jsonError);  // 转化为 JSON 文档
        if (!doc.isNull() && jsonError.error == QJsonParseError::NoError) {  // 解析未发生错误
            if (doc.isObject()) { // JSON 文档为对象
                QJsonObject object = doc.object();  // 转化为对象
                if (object.contains("Name")) {  // 包含指定的 key
                    QJsonValue value = object.value("Name");  // 获取指定 key 对应的 value
                    if (value.isString()) {  // 判断 value 是否为字符串
                        QString strName = value.toString();  // 将 value 转化为字符串
                        qDebug() << "Name : " << strName;
                    }
                }
                if (object.contains("From")) {
                    QJsonValue value = object.value("From");
                    if (value.isDouble()) {
                        int nFrom = value.toVariant().toInt();
                        qDebug() << "From : " << nFrom;
                    }
                }
                if (object.contains("Cross Platform")) {
                    QJsonValue value = object.value("Cross Platform");
                    if (value.isBool()) {
                        bool bCrossPlatform = value.toBool();
                        qDebug() << "CrossPlatform : " << bCrossPlatform;
                    }
                }
                return {};
            }
        }
    }
    return {};
}
