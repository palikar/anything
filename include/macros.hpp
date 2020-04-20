#pragma once

#define AY_COMPONENT(name)                                   \
    static gmt::ComponentType *static_type()                 \
    {                                                        \
        static gmt::ComponentType type({ #name });           \
        return &type;                                        \
    }                                                        \
    inline virtual gmt::ComponentType *type() const override \
    {                                                        \
        return static_type();                                \
    }


#define AY_RAII_OBJECT(TypeName)                     \
    TypeName(TypeName &&) = default;                 \
    TypeName &operator=(TypeName &&) = default;      \
    TypeName &operator=(const TypeName &) = default; \
    TypeName(const TypeName &)            = delete


#define FACTORY_UNIQE(name, type)                                   \
    template<typename... Args>                                      \
    type##Ptr name(Args &&... args)                                 \
    {                                                               \
        return std::make_unique<type>(std::forward<Args>(args)...); \
    }


#define FACTORY_SHARED(name, type)                                  \
    template<typename... Args>                                      \
    type##Ptr name(Args &&... args)                                 \
    {                                                               \
        return std::make_shared<type>(std::forward<Args>(args)...); \
    }
