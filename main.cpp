// 首发Qt君公众号
//#pragma execution_character_set("utf-8")
#include <QCoreApplication>
#include <QVariantMap>
#include <QColor>
#include <QDebug>

template <typename F>
struct GetFirstArgument
{
    typedef std::remove_const_t<
        std::remove_reference_t<
        typename QtPrivate::List_Select<
        typename QtPrivate::FunctionPointer<F
        >::Arguments, 0
        >::Value
        >
        > Type;
};

// 类成员函数
template <bool condition, typename Map, typename Obj, typename F>
struct IF_
{
    static inline auto exec(QString key, Map &m, Obj &o, F f)
    {
        using Arg0Type = GetFirstArgument<F>::Type;
        auto defaultValue = Arg0Type();
        auto mapValue = m.value(key, defaultValue).value<Arg0Type>();
        return (o.*f)(mapValue);
    }
};

// 函数
template <typename Map, typename Obj, typename F>
struct IF_<false, Map, Obj, F>
{
    static inline auto exec(QString key, Map &m, Obj &, F f)
    {
        using Arg0Type = GetFirstArgument<F>::Type;

        auto defaultValue = Arg0Type();
        auto mapValue = m.value(key, defaultValue).value<Arg0Type>();
        return f(mapValue);
    }
};

// 判断有函数或类成员函数
template <typename Map, typename Obj, typename T, typename F>
auto getValue(QString key, Map &m, Obj &o, T, F f)
{
    typedef QtPrivate::FunctionPointer<F> FUNC;
    return IF_<FUNC::IsPointerToMemberFunction, Map, Obj, F>::exec(key, m, o, f);
}

// 判断没有函数
template <typename Map, typename Obj, typename T>
auto getValue(QString key, Map &m, Obj &, T defaultValue)
{
    auto mapValue = m.value(key, defaultValue).value<T>();
    return mapValue;
}

// 类成员函数
template <bool condition, typename Obj, typename T, typename F>
struct IF__
{
    static inline auto exec(Obj &o, T t, F f)
    {
//        auto oo = static_cast<std::remove_const<Obj>::type>(o);
//        return (oo.*f)(t);
        return (o.*f)(t);
    }
};

// 函数
template <typename Obj, typename T, typename F>
struct IF__ <false, Obj, T, F>
{
    static inline auto exec(Obj &, T t, F f)
    {
        return f(t);
    }
};

template <typename Obj, typename T, typename F>
auto getValue(Obj &o, T t, F f)
{
    typedef QtPrivate::FunctionPointer<F> FUNC;
    return IF__<FUNC::IsPointerToMemberFunction, Obj, T, F>::exec(o, t, f);
}

template <typename Obj, typename T>
auto getValue(Obj &, T t)
{
    return t;
}

template <typename T> T &point2Ref(T &t) { return t; }
template <typename T> T &point2Ref(T *t) { assert(t != nullptr); return *t; } // 为什么返回const T,因为和所在的函数是否const有关

#define MAP_TO_OBJECT_ITEM(map, obj, key, ...) \
    point2Ref(obj).key = \
    getValue(\
        #key, point2Ref(map), point2Ref(obj), std::remove_pointer<decltype(obj)>::type().key, \
        __VA_ARGS__);

#define OBJECT_TO_MAP_ITEM(obj, map, key, ...) \
    point2Ref(map)[#key] = getValue(point2Ref(obj), point2Ref(obj).key, __VA_ARGS__);

static QPoint toPointGlobal(const QString &pointString)
{
    QStringList result = pointString.split(",");
    if(result.count() != 2) {
        return {-1, -1};
    }

    return {result[0].toInt(), result[1].toInt()};
}

static QString toStringGlobal(const QPoint &p)
{
    return QString("%1,%2").arg(p.x()).arg(p.y());
}

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

    static SkinConfig fromMapDefault(const QVariantMap &map)
    {
        SkinConfig config;
        config.pos     = toPointGlobal(map.value("pos", "").toString());
        config.width   = map.value("width",  0).toInt();
        config.height  = map.value("height", 0).toInt();
        config.opacity = map.value("opacity", false).toFloat();
        config.visible = map.value("visible", false).toBool();
        config.color   = map.value("color", QColor(Qt::white)).value<QColor>();

        return config;
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

    QVariantMap toMapDefault() const
    {
        QVariantMap map;
        map["pos"]     = toString(pos);
        map["width"]   = width;
        map["height"]  = height;
        map["opacity"] = opacity;
        map["visible"] = visible;
        map["color"]   = color;

        return map;
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

template <int N>
struct Get
{
    enum {
        Result = Get<N-1>::Result + N
    };
};

template <>
struct Get<1>
{
    enum {
        Result = 1
    };
};

#include <QTextCodec>
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    test();

    QTextCodec *c = QTextCodec::codecForName("GB18030");
    QByteArray s = c->fromUnicode("你好");
    printf("你好123%s\r\n", s.data());
    std::cout << "你好" << s.data() << std::endl;
    qDebug() << "你好123";
    qDebug() << Get<5>::Result;

    return a.exec();
}
