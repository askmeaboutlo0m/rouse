#define XL_UNUSED(X) ((void)(X))

typedef struct XL_EnumEntry {
    const char  *key;
    lua_Integer value;
} XL_EnumEntry;

int           XL_checkbool  (lua_State *L, int index);
int           XL_checkint   (lua_State *L, int index);
float         XL_checkfloat (lua_State *L, int index);
unsigned int  XL_checkuint  (lua_State *L, int index);
unsigned char XL_checkuchar (lua_State *L, int index);
uint8_t       XL_checkuint8 (lua_State *L, int index);
int32_t       XL_checkint32 (lua_State *L, int index);
uint32_t      XL_checkuint32(lua_State *L, int index);

void *XL_checkpptype_nullable(lua_State *L, int index, const char *tname);

void *XL_checkpptype(lua_State *L, int index, const char *tname);
void *XL_checkutype (lua_State *L, int index, const char *tname);

void XL_pushint   (lua_State *L, int           value);
void XL_pushfloat (lua_State *L, float         value);
void XL_pushuint  (lua_State *L, unsigned int  value);
void XL_pushuchar (lua_State *L, unsigned char value);
void XL_pushuint8 (lua_State *L, uint8_t       value);
void XL_pushint32 (lua_State *L, int32_t       value);
void XL_pushuint32(lua_State *L, uint32_t      value);

void **XL_pushnewpptypeuv(lua_State *L, void *value,
                          const char *tname, int nuvalue);

void **XL_pushnewpptypeuv_nullable(lua_State *L, void *value,
                                   const char *tname, int nuvalue);

void *XL_pushnewutypeuv(lua_State *L, const void *value,
                        size_t size, const char *tname,
                        int nuvalue);

void XL_getiuservalue(lua_State *L, int index, int n);
void XL_setiuservalue(lua_State *L, int index, int n);

int XL_index_fallback(lua_State *L, const char *tname,
                      int selfindex, int keyindex);

int XL_index(lua_State *L, const char *tname, void *reg,
             int selfindex, int keyindex);

int XL_newindex(lua_State *L, const char *tname, void *reg,
                int selfindex, int keyindex, int valueindex);

int XL_staticindex(lua_State *L, void *reg, int keyindex);

int XL_staticnewindex(lua_State *L, void *reg, int selfindex,
                      int keyindex, int valueindex);

void XL_initmetatable    (lua_State *L, const char *tname, luaL_Reg *methods);
void XL_initindextable   (lua_State *L, void       *key,   luaL_Reg *methods);
void XL_initnewindextable(lua_State *L, void       *key,   luaL_Reg *methods);

void XL_initfunctions(lua_State *L, luaL_Reg *functions, ...);

void XL_initenum(lua_State *L, XL_EnumEntry *entries, ...);

void XL_initstaticmetatable(lua_State *L, ...);

int XL_setfromtable(lua_State *L, const char *tname,
                    int selfindex, int tableindex);
