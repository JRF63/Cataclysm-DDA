#if defined(TILES)

#include "sdl_wrappers.h"

#include <mutex>
#include <ostream>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

#include "cata_assert.h"
#include "debug.h"
#include "point.h"

#if defined(_MSC_VER) && defined(USE_VCPKG)
#   include <SDL2/SDL_image.h>
#else
#   include <SDL_image.h>
#endif

#define dbg(x) DebugLog((x),D_SDL) << __FILE__ << ":" << __LINE__ << ": "

bool printErrorIf( const bool condition, const char *const message )
{
    if( !condition ) {
        return false;
    }
    dbg( D_ERROR ) << message << ": " << SDL_GetError();
    return true;
}

void throwErrorIf( const bool condition, const char *const message )
{
    if( !condition ) {
        return;
    }
    throw std::runtime_error( std::string( message ) + ": " + SDL_GetError() );
}

void RenderCopy( const SDL_Renderer_Ptr &renderer, const SDL_Texture_Ptr &texture,
                 const SDL_Rect *srcrect, const SDL_Rect *dstrect )
{
    if( !renderer ) {
        dbg( D_ERROR ) << "Tried to render to a null renderer";
        return;
    }
    if( !texture ) {
        dbg( D_ERROR ) << "Tried to render a null texture";
        return;
    }
    printErrorIf( SDL_RenderCopy( renderer.get(), texture.get(), srcrect, dstrect ) != 0,
                  "SDL_RenderCopy failed" );
}

SDL_Texture_Ptr CreateTexture( const SDL_Renderer_Ptr &renderer, Uint32 format, int access,
                               int w, int h )
{
    if( !renderer ) {
        dbg( D_ERROR ) << "Tried to create texture with a null renderer";
        return SDL_Texture_Ptr();
    }
    SDL_Texture_Ptr result( SDL_CreateTexture( renderer.get(), format, access, w, h ) );
    printErrorIf( !result, "SDL_CreateTexture failed" );
    return result;
}

SDL_Texture_Ptr CreateTextureFromSurface( const SDL_Renderer_Ptr &renderer,
        const SDL_Surface_Ptr &surface )
{
    if( !renderer ) {
        dbg( D_ERROR ) << "Tried to create texture with a null renderer";
        return SDL_Texture_Ptr();
    }
    if( !surface ) {
        dbg( D_ERROR ) << "Tried to create texture from a null surface";
        return SDL_Texture_Ptr();
    }
    SDL_Texture_Ptr result( SDL_CreateTextureFromSurface( renderer.get(), surface.get() ) );
    printErrorIf( !result, "SDL_CreateTextureFromSurface failed" );
    return result;
}

void SetRenderDrawColor( const SDL_Renderer_Ptr &renderer, const Uint8 r, const Uint8 g,
                         const Uint8 b, const Uint8 a )
{
    if( !renderer ) {
        dbg( D_ERROR ) << "Tried to use a null renderer";
        return;
    }
    printErrorIf( SDL_SetRenderDrawColor( renderer.get(), r, g, b, a ) != 0,
                  "SDL_SetRenderDrawColor failed" );
}

void RenderDrawPoint( const SDL_Renderer_Ptr &renderer, const point &p )
{
    printErrorIf( SDL_RenderDrawPoint( renderer.get(), p.x, p.y ) != 0, "SDL_RenderDrawPoint failed" );
}

void RenderFillRect( const SDL_Renderer_Ptr &renderer, const SDL_Rect *const rect )
{
    if( !renderer ) {
        dbg( D_ERROR ) << "Tried to use a null renderer";
        return;
    }
    printErrorIf( SDL_RenderFillRect( renderer.get(), rect ) != 0, "SDL_RenderFillRect failed" );
}

void FillRect( const SDL_Surface_Ptr &surface, const SDL_Rect *const rect, Uint32 color )
{
    if( !surface ) {
        dbg( D_ERROR ) << "Tried to use a null surface";
        return;
    }
    printErrorIf( SDL_FillRect( surface.get(), rect, color ) != 0, "SDL_FillRect failed" );
}

void SetTextureBlendMode( const SDL_Texture_Ptr &texture, SDL_BlendMode blendMode )
{
    if( !texture ) {
        dbg( D_ERROR ) << "Tried to use a null texture";
    }

    throwErrorIf( SDL_SetTextureBlendMode( texture.get(), blendMode ) != 0,
                  "SDL_SetTextureBlendMode failed" );
}

bool SetTextureColorMod( const SDL_Texture_Ptr &texture, Uint32 r, Uint32 g, Uint32 b )
{
    if( !texture ) {
        dbg( D_ERROR ) << "Tried to use a null texture";
        return true;
    }
    return printErrorIf( SDL_SetTextureColorMod( texture.get(), r, g, b ) != 0,
                         "SDL_SetTextureColorMod failed" );
}

void SetRenderDrawBlendMode( const SDL_Renderer_Ptr &renderer, const SDL_BlendMode blendMode )
{
    if( !renderer ) {
        dbg( D_ERROR ) << "Tried to use a null renderer";
        return;
    }
    printErrorIf( SDL_SetRenderDrawBlendMode( renderer.get(), blendMode ) != 0,
                  "SDL_SetRenderDrawBlendMode failed" );
}

void GetRenderDrawBlendMode( const SDL_Renderer_Ptr &renderer, SDL_BlendMode &blend_mode )
{
    if( !renderer ) {
        dbg( D_ERROR ) << "Tried to use a null renderer";
        return;
    }
    printErrorIf( SDL_GetRenderDrawBlendMode( renderer.get(), &blend_mode ) != 0,
                  "SDL_GetRenderDrawBlendMode failed" );
}

SDL_Surface_Ptr load_image( const char *const path )
{
    cata_assert( path );
    SDL_Surface_Ptr result( IMG_Load( path ) );
    if( !result ) {
        throw std::runtime_error( "Could not load image \"" + std::string( path ) + "\": " +
                                  IMG_GetError() );
    }
    return result;
}

void SetRenderTarget( const SDL_Renderer_Ptr &renderer, const SDL_Texture_Ptr &texture )
{
    if( !renderer ) {
        dbg( D_ERROR ) << "Tried to use a null renderer";
        return;
    }
    // a null texture is fine for SDL
    printErrorIf( SDL_SetRenderTarget( renderer.get(), texture.get() ) != 0,
                  "SDL_SetRenderTarget failed" );
}

void RenderClear( const SDL_Renderer_Ptr &renderer )
{
    if( !renderer ) {
        dbg( D_ERROR ) << "Tried to use a null renderer";
        return;
    }
    printErrorIf( SDL_RenderClear( renderer.get() ) != 0, "SDL_RenderClear failed" );
}

SDL_Surface_Ptr CreateRGBSurface( const Uint32 flags, const int width, const int height,
                                  const int depth, const Uint32 Rmask, const Uint32 Gmask, const Uint32 Bmask, const Uint32 Amask )
{
    SDL_Surface_Ptr surface( SDL_CreateRGBSurface( flags, width, height, depth, Rmask, Gmask, Bmask,
                             Amask ) );
    throwErrorIf( !surface, "Failed to create surface" );
    return surface;
}

struct render_copy_ex_command {
    std::shared_ptr<SDL_Texture> sdl_texture_ptr;
    SDL_Rect srcrect;
    SDL_Rect dstrect;
    double angle;
    SDL_RendererFlip flip;
};

static std::vector<render_copy_ex_command> command_buffer;
static std::mutex command_buffer_mutex;

int RenderCopyEx( const SDL_Renderer_Ptr &renderer, std::shared_ptr<SDL_Texture> sdl_texture_ptr,
                  const SDL_Rect *srcrect, const SDL_Rect *dstrect, const double angle,
                  const SDL_Point *center, const SDL_RendererFlip flip )
{
    //dbg(D_INFO) << "-- RenderCopyEx Log Start ---";
    //dbg(D_INFO) << "Pointer address: " << sdl_texture_ptr.get();
    //dbg(D_INFO) << "Source rect: {" << srcrect->x << ", " << srcrect->y << ", " << srcrect->w << ", " << srcrect->h << "}";
    //dbg(D_INFO) << "Dest rect: {" << dstrect->x << ", " << dstrect->y << ", " << dstrect->w << ", " << dstrect->h << "}";
    //dbg(D_INFO) << "Angle: " << angle;
    //dbg(D_INFO) << "Center: {" << center->x << ", " << center->y << "}";
    //dbg(D_INFO) << "Flip: {" << flip;
    const std::lock_guard<std::mutex> lock( command_buffer_mutex );
    command_buffer.push_back( render_copy_ex_command{ sdl_texture_ptr, *srcrect, *dstrect, angle, flip } );
    return 0;
}

void RenderCopyExSubmit( const SDL_Renderer_Ptr &renderer )
{
    const std::lock_guard<std::mutex> lock( command_buffer_mutex );
    for( auto c : command_buffer ) {
        //dbg(D_INFO) << "-- RenderCopyEx Cont. ---";
        //dbg(D_INFO) << "Pointer address: " << c.sdl_texture_ptr.get();
        //dbg(D_INFO) << "Source rect: {" << c.srcrect.x << ", " << c.srcrect.y << ", " << c.srcrect.w << ", " << c.srcrect.h << "}";
        //dbg(D_INFO) << "Dest rect: {" << c.dstrect.x << ", " << c.dstrect.y << ", " << c.dstrect.w << ", " << c.dstrect.h << "}";
        //dbg(D_INFO) << "Angle: " << c.angle;
        //dbg(D_INFO) << "Center: {" << c.center.x << ", " << c.center.y << "}";
        //dbg(D_INFO) << "Flip: {" << c.flip;

        int ret = SDL_RenderCopyEx( renderer.get(), c.sdl_texture_ptr.get(), &c.srcrect,
                                    &c.dstrect, c.angle, nullptr, c.flip );
        if( ret != 0 ) {
            dbg( D_ERROR ) << "Error copying texture";
            break;
        }
    }
    command_buffer.clear();
}

#endif
