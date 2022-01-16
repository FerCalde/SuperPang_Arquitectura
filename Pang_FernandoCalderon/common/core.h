#ifndef _CORE_H_
#define _CORE_H_

//-----------------------------------------------------------------------------
typedef unsigned       char byte;
typedef unsigned short int  word;
typedef unsigned       int  dword;
typedef                int  sdword;

typedef unsigned       int  uint;

// Forward definition
class vec2;

//-----------------------------------------------------------------------------
struct rgba_t { byte a, r, g, b; };
inline rgba_t rgbamake(byte r, byte g, byte b, byte a)
  { rgba_t c; c.r = r; c.g = g; c.b = b; c.a = a; return c; }
#define RGBA(rrr,ggg,bbb,aaa) rgbamake(rrr,ggg,bbb,aaa)
#define GETA(_rgba_) _rgba_.a
#define GETR(_rgba_) _rgba_.r
#define GETG(_rgba_) _rgba_.g
#define GETB(_rgba_) _rgba_.b

//-----------------------------------------------------------------------------
#define SIZE_ARRAY(__a__) (sizeof(__a__)/sizeof(__a__[0]))

//-----------------------------------------------------------------------------
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#define M_PIf 3.14159265358979323846f

//-----------------------------------------------------------------------------

#define RAD2DEG(_rad_) (360.f * (_rad_) / (2.f * M_PIf))
#define DEG2RAD(_deg_) ((2.f * M_PIf) * (_deg_) / 360.f)

//-----------------------------------------------------------------------------
// Generate a random number.
float  CORE_FRand     (float from, float to);
//-----------------------------------------------------------------------------
// Load BMP
GLuint CORE_LoadBmp   (const char filename[], bool wrap);
//-----------------------------------------------------------------------------
// Load PNG
GLuint CORE_LoadPNG   (const char filename[], bool wrap);
//-----------------------------------------------------------------------------
// Unload BMP
void   CORE_UnloadBmp (GLuint texid);
//-----------------------------------------------------------------------------
// Unload PNG
void   CORE_UnloadPNG (GLuint texid);
//-----------------------------------------------------------------------------
// Render a Sprite.
// p0, p1:	Min and max window position to draw sprite.
void   CORE_RenderSprite(const vec2 &p0, const vec2 &p1, GLuint texid);
//-----------------------------------------------------------------------------
// Render a Sprite.
// pos:	Window position to draw sprite center.
void   CORE_RenderCenteredSprite(const vec2 &pos, const vec2 &size, GLuint texid);
//-----------------------------------------------------------------------------
// Render a Sprite in a window position with rotation in radians.
void   CORE_RenderCenteredRotatedSprite(const vec2 &pos, const vec2 &size, float angle, GLuint texid, rgba_t color = RGBA(255,255,255,255));

//-----------------------------------------------------------------------------
// Sound.
#ifdef SWALIB_SOUND
bool CORE_InitSound();
void CORE_EndSound();
uint CORE_LoadWav     (const char filename[]);
void CORE_PlaySound (uint snd, float volume, float pitch);
void CORE_UnloadWav (uint snd);
void CORE_PlayMusic (uint snd, float volume = 1.f);
void CORE_StopMusic ();
#endif //SWALIB_SOUND

#endif
