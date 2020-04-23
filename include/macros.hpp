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


#define EVENT_TYPE(t_type)                          \
    static EventType static_type()                  \
    {                                               \
        return EventType::t_type;                   \
    }                                               \
    virtual EventType type() const override         \
    {                                               \
        return static_type();                       \
    }                                               \
    virtual const std::string name() const override \
    {                                               \
        return #t_type;                             \
    }


#define MEMBER(mem_fun) [this](auto &event) { return mem_fun(event); }
