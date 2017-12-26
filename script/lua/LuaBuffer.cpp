#include "LuaBuffer.h"
#include "Buffer.h"
#include "lua.hpp"
#include "Logger.h"


int32_t buffer_write_format(kit::Buffer* buffer, lua_State* L)
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
                *buffer << ( int8_t )lua_tonumber( L, i+3 );
                break;
            case 'B':
                *buffer << ( uint8_t )lua_tonumber( L, i+3 );
                break;
            case 'w':
                *buffer << ( int16_t )lua_tonumber( L, i+3 );
                break;
            case 'W':
                *buffer << ( uint16_t )lua_tonumber( L, i+3 );
                break;
            case 'i':
                *buffer << ( int32_t )lua_tonumber( L, i+3 );
                break;
            case 'I':
                *buffer << ( uint32_t )lua_tonumber( L, i+3 );
                break;
            case 'l':
                *buffer << ( int64_t )lua_tonumber( L, i+3 );
                break;
            case 'L':
                *buffer << ( uint64_t )lua_tonumber( L, i+3 );
                break;
            case 'f':
                *buffer << ( float )lua_tonumber( L, i+3 );
                break;
            case 'd':
                *buffer << ( double )lua_tonumber( L, i+3 );
                break;
            case 's':
                {
                    size_t len2;
                    const char* str = luaL_checklstring( L, i+3, &len2 );
                    *buffer << str;
                }
                break;
            default:
                ERR( "[Buffer] c_write_format, invalid format: %c", fmt[i] );
                return 0;
        }
    }
    return 0;
}

int32_t buffer_read_format(kit::Buffer* buffer, lua_State* L)
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
                    *buffer >> c;
                    lua_pushinteger( L, c );
                }
                break;
            case 'B':
                {
                    uint8_t b;
                    *buffer >> b;
                    lua_pushinteger( L, b );
                }
                break;
            case 'w':
                {
                    int16_t s;
                    *buffer >> s;
                    lua_pushinteger( L, s );
                }
                break;
            case 'W':
                {
                    uint16_t w;
                    *buffer >> w;
                    lua_pushinteger( L, w );
                }
                break;
            case 'i':
                {
                    int32_t i;
                    *buffer >> i;
                    lua_pushinteger( L, i );
                }
                break;
            case 'I':
                {
                    uint32_t d;
                    *buffer >> d;
                    lua_pushinteger( L, d );
                }
                break;
            case 'l':
                {
                    int64_t l;
                    *buffer >> l;
                    lua_pushinteger( L, l );
                }
                break;
            case 'L':
                {
                    uint64_t q;
                    *buffer >> q;
                    lua_pushinteger( L, q );
                }
                break;
            case 'f':
                {
                    float f;
                    *buffer >> f;
                    lua_pushnumber( L, f );
                }
                break;
            case 'd':
                {
                    double o;
                    *buffer >> o;
                    lua_pushnumber( L, o );
                }
                break;
            case 's':
                {
                    uint16_t len;
                    *buffer >> len;
                    if (len <= 512)
                    {
                        char buf[512];
                        if (buffer->readBuffer(buf, len))
                            lua_pushlstring(L, buf, len);
                        else
                            lua_pushnil(L);
                    }
                    else
                    {
                        char* buf = new char[len];
                        if (buffer->readBuffer(buf, len))
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

