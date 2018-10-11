typedef struct R_Input R_Input;

typedef void (*R_InputKeyFn)(SDL_KeyboardEvent *, R_UserData, R_UserData);
typedef void (*R_InputFreeFn)(R_UserData);

typedef enum R_InputHandled {
    R_INPUT_UNHANDLED     = 0x0,
    R_INPUT_BOUND         = 0x1,
    R_INPUT_UNBOUND_KEY   = 0x2,
    R_INPUT_BOUND_KEY     = 0x3,
    R_INPUT_UNBOUND_MOUSE = 0x4,
    R_INPUT_BOUND_MOUSE   = 0x5,
} R_InputHandled;


R_Input *R_input_new(void);

void R_input_clear(R_Input *in);
void R_input_free(R_Input *in);


void R_input_key_bind(R_Input *in, unsigned int key, R_InputKeyFn fn,
                      R_UserData user, R_InputFreeFn on_free);
bool R_input_key_unbind(R_Input *in, unsigned int key);
void R_input_key_clear(R_Input *in);


R_InputHandled R_input_handle_event(R_Input *in, SDL_Event *event,
                                    R_UserData context);
