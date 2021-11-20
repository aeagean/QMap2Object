# QMap2Object
QMap容器与对象互转的实现

```cpp
template <typename T> T &point2Ref(T &t) { return  t; }
template <typename T> T &point2Ref(T *t) { assert(t != nullptr); return *t; }

#define MAP_TO_OBJECT_ITEM(map, obj, key) \
    point2Ref(obj).key = \
    point2Ref(map).value(#key, std::remove_pointer<decltype(obj)>::type().key) \
                  .value<decltype(point2Ref(obj).key)>();

#define OBJECT_TO_MAP_ITEM(obj, map, key) \
    point2Ref(map)[#key] = point2Ref(obj).key;

static SkinConfig fromMap(const QVariantMap &map)
{
    SkinConfig config;
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
    OBJECT_TO_MAP_ITEM(this, map, width);
    OBJECT_TO_MAP_ITEM(this, map, height);
    OBJECT_TO_MAP_ITEM(this, map, opacity);
    OBJECT_TO_MAP_ITEM(this, map, visible);
    OBJECT_TO_MAP_ITEM(this, map, color);

    return map;
}
```
