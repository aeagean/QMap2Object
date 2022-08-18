// 首发Qt君公众号
#include <QCoreApplication>
#include <QVariantMap>
#include <QColor>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

#include "Object2Json.h"

static QString toStringGlobal(const QPoint &p)
{
    return QString("%1,%2").arg(p.x()).arg(p.y());
}

static QPoint toPointGlobal(const QString &pointString)
{
    QStringList result = pointString.split(",");
    if(result.count() != 2) {
        return {-1, -1};
    }

    return {result[0].toInt(), result[1].toInt()};
}

#define t(tt, ...) qDebug() << tt;

struct SkinConfig
{
    QPoint pos     = {0, 0};
    int    width   = 0;
    int    height  = 0;
    bool   visible = false;
    float  opacity = 1.0f;
    QColor color   = Qt::white;

    static bool isEqual(const SkinConfig &c1, const SkinConfig &c2)
    {
        return c1.pos == c2.pos &&
               c1.width == c2.width &&
               c1.height == c2.height &&
               c1.visible == c2.visible &&
               c1.opacity == c2.opacity &&
               c1.color == c2.color;
    }

    static SkinConfig fromMap(const QVariantMap &map)
    {
        SkinConfig config;
        MAP_TO_OBJECT_ITEM(map, config, pos, toPointGlobal);
        MAP_TO_OBJECT_ITEM(map, config, pos, &SkinConfig::toPoint);
        MAP_TO_OBJECT_ITEM(map, config, width);
        MAP_TO_OBJECT_ITEM(map, config, height);
        MAP_TO_OBJECT_ITEM(map, config, opacity);
        MAP_TO_OBJECT_ITEM(map, config, visible);
        MAP_TO_OBJECT_ITEM(map, config, color);

        return config;
    }

    QVariantMap toMap() const
    {
        QVariantMap map;
        OBJECT_TO_MAP_ITEM(this, map, pos, toStringGlobal);
        OBJECT_TO_MAP_ITEM(this, map, pos, &SkinConfig::toString);
        OBJECT_TO_MAP_ITEM(this, map, width);
        OBJECT_TO_MAP_ITEM(this, map, height);
        OBJECT_TO_MAP_ITEM(this, map, opacity);
        OBJECT_TO_MAP_ITEM(this, map, visible);
        OBJECT_TO_MAP_ITEM(this, map, color);

        return map;
    }

    QString toString(const QPoint &p) const
    {
        return QString("%1,%2").arg(p.x()).arg(p.y());
    }

    QPoint toPoint(const QString &pointString) const
    {
        QStringList result = pointString.split(",");
        if(result.count() != 2) {
            return {-1, -1};
        }

        return {result[0].toInt(), result[1].toInt()};
    }
};

static void test()
{
    SkinConfig config;
    config.pos = QPoint(100, 100);
    config.width = 100;
    config.height = 100;
    config.visible = true;
    config.opacity = 0.1f;
    config.color = Qt::black;

    SkinConfig config2 = SkinConfig::fromMap(config.toMap());

    Q_ASSERT(SkinConfig::isEqual(config, config2));
    qDebug() << "pass";
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    test();
    t(1, 2 3 4);

    return a.exec();
}
