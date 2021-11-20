// 首发Qt君公众号
#include <QCoreApplication>
#include <QVariantMap>
#include <QColor>

template <typename T> T &point2Ref(T &t) { return  t; }
template <typename T> T &point2Ref(T *t) { assert(t != nullptr); return *t; }

#define MAP_TO_OBJECT_ITEM(map, obj, key) \
    point2Ref(obj).key = \
    point2Ref(map).value(#key, std::remove_pointer<decltype(obj)>::type().key) \
                  .value<decltype(point2Ref(obj).key)>();

#define OBJECT_TO_MAP_ITEM(obj, map, key) \
    point2Ref(map)[#key] = point2Ref(obj).key;

struct SkinConfig
{
    int    width   = 0;
    int    height  = 0;
    bool   visible = false;
    float  opacity = 1.0;
    QColor color   = Qt::white;

    static SkinConfig fromMap(const QVariantMap &map)
    {
        SkinConfig config;
        config.width   = map.value("width",  0).toInt();
        config.height  = map.value("height", 0).toInt();
        config.opacity = map.value("opacity", false).toFloat();
        config.visible = map.value("visible", false).toBool();
        config.color   = map.value("color", QColor(Qt::white)).value<QColor>();

        return config;
    }

    static SkinConfig fromMap1(const QVariantMap &map)
    {
        SkinConfig config;
        MAP_TO_OBJECT_ITEM(map, config, width);
        MAP_TO_OBJECT_ITEM(map, config, height);
        MAP_TO_OBJECT_ITEM(map, config, opacity);
        MAP_TO_OBJECT_ITEM(map, config, visible);
        MAP_TO_OBJECT_ITEM(map, config, color);

        return config;
    }

    QVariantMap toMap1() const
    {
        QVariantMap map;
        OBJECT_TO_MAP_ITEM(this, map, width);
        OBJECT_TO_MAP_ITEM(this, map, height);
        OBJECT_TO_MAP_ITEM(this, map, opacity);
        OBJECT_TO_MAP_ITEM(this, map, visible);
        OBJECT_TO_MAP_ITEM(this, map, color);

        return map;
    }

    QVariantMap toMap() const
    {
        QVariantMap map;
        map["width"]   = width;
        map["height"]  = height;
        map["opacity"] = opacity;
        map["visible"] = visible;
        map["color"]   = color;

        return map;
    }
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    return a.exec();
}
