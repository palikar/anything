
#define AY_COMPONENT(name)                                              \
    static ComponentType* static_type()                                 \
    {                                                                   \
        static ComponentType type({ #name });                           \
        return &type;                                                   \
    }                                                                   \
    inline virtual ComponentType* type() const override { return static_type(); }

