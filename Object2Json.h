// @Qthub.com
#pragma once

#include <QPoint>
#include <QRect>
#include <QString>
#include <QObject>
#include <QJsonObject>

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

// 函数
template <bool condition, typename Map, typename Obj, typename F>
struct IF_Variable
{
    static inline auto exec(QString key, Map &m, F f)
    {
        using Arg0Type = GetFirstArgument<F>::Type;

        auto defaultValue = Arg0Type();
        auto mapValue = m.value(key, defaultValue).value<Arg0Type>();
        return f(mapValue);
    }
};

// 变量
template <typename Map, typename Obj, typename F>
struct IF_Variable<false, Map, Obj, F>
{
    static inline auto exec(QString key, Map &m, F f)
    {
        auto defaultValue = f;
        return m.value(key, defaultValue).value<F>();
    }
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

template<typename R, typename... Params>
struct IsFunctionT
{
    enum { value = false };
};

template<typename R, typename... Params>
struct IsFunctionT<R (*)(Params ...)>
{
    enum { value = true};
};

// 函数/变量
template <typename Map, typename Obj, typename F>
struct IF_<false, Map, Obj, F>
{
    static inline auto exec(QString key, Map &m, Obj &, F f)
    {
        return IF_Variable<IsFunctionT<F>::value, Map, Obj, F>::exec(key, m, f);
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

static QPoint string2Point(const QString &pointString)
{
    QStringList result = pointString.split(",");
    if(result.count() != 2) {
        return QPoint();
    }

    return {result[0].toInt(), result[1].toInt()};
}

static QString point2String(const QPoint &p)
{
    return QString("%1,%2").arg(p.x()).arg(p.y());
}

static QRect string2Rect(const QString &rectString)
{
    QStringList result = rectString.split(",");
    if(result.count() != 4) {
        return QRect();
    }

    return QRect(result[0].toInt(),
                 result[1].toInt(),
                 result[2].toInt(),
                 result[3].toInt()
            );
}

static QString rect2String(const QRect &p)
{
    return QString("%1,%2,%3,%4").arg(p.x()).arg(p.y()).arg(p.width()).arg(p.height());
}

#define MAP_TO_OBJECT_ITEM(map, obj, key, ...) \
    point2Ref(obj).key = \
    getValue(\
        #key, \
        point2Ref(map), \
        point2Ref(obj), \
        std::remove_pointer<decltype(obj)>::type().key, \
        __VA_ARGS__);

#define OBJECT_TO_MAP_ITEM(obj, map, key, ...) point2Ref(map)[#key] = getValue(point2Ref(obj), point2Ref(obj).key, __VA_ARGS__);

#define JSON_TO_OBJECT_ITEM(map, obj, key, ...) MAP_TO_OBJECT_ITEM(map, obj, key, ...)
#define OBJECT_TO_JSON_ITEM(obj, map, key, ...) OBJECT_TO_MAP_ITEM(obj, map, key, ...)
