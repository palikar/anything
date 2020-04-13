#pragma once

#define AY_COMPONENT(name)                                              \
    static ComponentType* static_type()                                 \
    {                                                                   \
        static ComponentType type({ #name });                           \
        return &type;                                                   \
    }                                                                   \
    inline virtual ComponentType* type() const override { return static_type(); }



#define AY_RAII_OBJECT(TypeName)                  \
    TypeName(TypeName &&) = default;                 \
    TypeName &operator=(TypeName &&) = default;      \
    TypeName &operator=(const TypeName &) = default; \
    TypeName(const TypeName &)            = delete
