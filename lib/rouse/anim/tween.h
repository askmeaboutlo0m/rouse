typedef struct R_TweenElement R_TweenElement;

typedef struct R_TweenCalcArgs {
    void       *elem;
    R_UserData user;
} R_TweenCalcArgs;

typedef struct R_TweenTickArgs {
    void       *elem;
    R_UserData user;
    float      ratio;
} R_TweenTickArgs;

typedef struct R_TweenFreeArgs {
    void       *elem;
    R_UserData user;
} R_TweenFreeArgs;

typedef void (*R_TweenElementCalcFn)(R_TweenCalcArgs);
typedef void (*R_TweenElementTickFn)(R_TweenTickArgs);
typedef void (*R_TweenElementFreeFn)(R_TweenFreeArgs);

typedef float (*R_TweenFloatGetFn)(R_TweenCalcArgs);
typedef void  (*R_TweenFloatSetFn)(R_TweenTickArgs, float);

typedef enum R_TweenValueType {
    R_TWEEN_VALUE_FIXED,
    R_TWEEN_VALUE_BETWEEN,
    R_TWEEN_VALUE_CUSTOM,
} R_TweenValueType;

typedef struct R_TweenFloat {
    R_TweenValueType type;
    union {
        float fixed;
        struct {
            float a, b;
        } between;
    };
} R_TweenFloat;


R_Step *R_tween_new_fixed(float seconds, R_EaseFn ease);


R_TweenFloat R_tween_float_fixed(float value);
R_TweenFloat R_tween_float_between(float a, float b);

void R_tween_add_float(R_Step *step, R_TweenFloat value, R_UserData user,
                       R_TweenFloatGetFn get, R_TweenFloatSetFn set,
                       R_TweenElementFreeFn on_free);
