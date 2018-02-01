#include "LuaBuffer.h"
#include "Buffer.h"
#include "lua.hpp"
#include "LuaIntf/LuaIntf.h"
#include "Logger.h"

//TODO
int32_t LuaBuffer::luaWriteVector(lua_State* L)
{
    LuaIntf::LuaRef table = LuaIntf::Lua::pop<LuaIntf::LuaRef>(L);
    if (!table.isTable())
    {
        ERR("[LuaBuffer](luaWriteVector) param not a table!");
        return 0;
    }
    return 0;
}

//TODO
int32_t LuaBuffer::luaReadVector(lua_State* L)
{
    return 0;
}

int32_t LuaBuffer::luaWriteFormat(lua_State* L)
{
    int32_t narg = lua_gettop( L );
    if( narg < 3 ) {
        ERR( "[Buffer] c_write_format, narg < 2" );
        return 0;
    }

    size_t len;
    const char *fmt = luaL_checklstring( L, 2, &len );
    if( !fmt ) {
        ERR( "[Buffer] c_write_format, invalid format" );
        return 0;
    }

    if( len != size_t( narg-2 ) ) {
        ERR( "[Buffer] c_write_format, format: %s not match nargs: %d", fmt, narg );
        return 0;
    }

    for( size_t i = 0; i < len; ++i ) {
        switch( fmt[i] ) {
            case 'b': 
                *this << ( int8_t )lua_tonumber( L, i+3 );
                break;
            case 'B':
                *this << ( uint8_t )lua_tonumber( L, i+3 );
                break;
            case 'w':
                *this << ( int16_t )lua_tonumber( L, i+3 );
                break;
            case 'W':
                *this << ( uint16_t )lua_tonumber( L, i+3 );
                break;
            case 'i':
                *this << ( int32_t )lua_tonumber( L, i+3 );
                break;
            case 'I':
                *this << ( uint32_t )lua_tonumber( L, i+3 );
                break;
            case 'l':
                *this << ( int64_t )lua_tonumber( L, i+3 );
                break;
            case 'L':
                *this << ( uint64_t )lua_tonumber( L, i+3 );
                break;
            case 'f':
                *this << ( float )lua_tonumber( L, i+3 );
                break;
            case 'd':
                *this << ( double )lua_tonumber( L, i+3 );
                break;
            case 's':
                {
                    size_t len2;
                    const char* str = luaL_checklstring( L, i+3, &len2 );
                    *this << str;
                }
                break;
            default:
                ERR( "[Buffer] c_write_format, invalid format: %c", fmt[i] );
                return 0;
        }
    }
    return 0;
}

int32_t LuaBuffer::luaReadFormat(lua_State* L)
{
    size_t len;
    const char *fmt = luaL_checklstring( L, 2, &len );
    if( !fmt ) {
        ERR( "[BufferNode] c_read_format, invalid format" );
        return 0;
    }

    lua_checkstack( L, len + 2 ); // ret value need + 1
    for( size_t i = 0; i < len; ++i ) {
        switch( fmt[i] ) {
            case 'b': 
                {
                    int8_t c;
                    *this >> c;
                    lua_pushinteger( L, c );
                }
                break;
            case 'B':
                {
                    uint8_t b;
                    *this >> b;
                    lua_pushinteger( L, b );
                }
                break;
            case 'w':
                {
                    int16_t s;
                    *this >> s;
                    lua_pushinteger( L, s );
                }
                break;
            case 'W':
                {
                    uint16_t w;
                    *this >> w;
                    lua_pushinteger( L, w );
                }
                break;
            case 'i':
                {
                    int32_t i;
                    *this >> i;
                    lua_pushinteger( L, i );
                }
                break;
            case 'I':
                {
                    uint32_t d;
                    *this >> d;
                    lua_pushinteger( L, d );
                }
                break;
            case 'l':
                {
                    int64_t l;
                    *this >> l;
                    lua_pushinteger( L, l );
                }
                break;
            case 'L':
                {
                    uint64_t q;
                    *this >> q;
                    lua_pushinteger( L, q );
                }
                break;
            case 'f':
                {
                    float f;
                    *this >> f;
                    lua_pushnumber( L, f );
                }
                break;
            case 'd':
                {
                    double o;
                    *this >> o;
                    lua_pushnumber( L, o );
                }
                break;
            case 's':
                {
                    size_t len;
                    this->readVaruint<size_t>(len);
                    if (len <= 512)
                    {
                        char buf[512];
                        if (this->readBuffer(buf, len))
                            lua_pushlstring(L, buf, len);
                        else
                            lua_pushnil(L);
                    }
                    else
                    {
                        char* buf = new char[len];
                        if (this->readBuffer(buf, len))
                            lua_pushlstring(L, buf, len);
                        else
                            lua_pushnil(L);
                    }
                }
                break;
            default:
                ERR( "[BufferNode] c_read_format, invalid format: %c", fmt[i] );
                return 0;
        }
    }
    return len;
}

