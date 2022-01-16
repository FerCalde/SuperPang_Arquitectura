#include "stdafx.h"
#include "sys.h"
#include "core.h"
#include "vector2d.h"

//=============================================================================
// Font support
#define FONT_ROWS 8
#define FONT_COLS 8
#define MAX_FONT_CHARDEFS 256
#define FONT_CHAR_WIDTH (16.f/128.f)
#define FONT_CHAR_HEIGHT (16.f/128.f)

GLuint FONT_Tex;
char FONT_Def[FONT_ROWS][FONT_COLS+1] =
{
  { " !\"~*%-'" },
  { "   +, ./" },
  { "01234567" },
  { "89:;aib?" },
  { "*ABCDEFG" },
  { "HIJKLMNO" },
  { "PQRSTUVW" },
  { "XYZ     " },
};

struct FontCharDef
{
  char ch;
  vec2 p0;
  FontCharDef() :
      ch(0),
      p0(0.0f)
  {};
};

FontCharDef fontchardefs[MAX_FONT_CHARDEFS];

//-----------------------------------------------------------------------------
// Load fonts.
void FONT_Init()
{
  // Load texture
  FONT_Tex = CORE_LoadPNG("data/kromasky.png", false);

  // Preapre coords
  for (int i = 0; i < FONT_ROWS; i++) // One iteration per row
  {
    for (int j = 0; j < FONT_COLS; j++) // Inside row
    {
      unsigned char ch = FONT_Def[i][j];
      fontchardefs[ch].ch = ch;
      fontchardefs[ch].p0 = vec2(j * FONT_CHAR_WIDTH, (FONT_ROWS - i - 1) * FONT_CHAR_HEIGHT);
    }
  }
}

//-----------------------------------------------------------------------------
// Unload fonts.
void FONT_End()
{
  CORE_UnloadPNG(FONT_Tex);
}

//-----------------------------------------------------------------------------
// Draw a char. in a position.
// p0, p1:	Min and max window position to draw a char.
void FONT_DrawChar(const vec2 &p0, const vec2 &p1, unsigned char ch)
{
  if ((int)ch < MAX_FONT_CHARDEFS && fontchardefs[ch].ch == ch)
  {
    glBindTexture( GL_TEXTURE_2D, FONT_Tex );
    glBegin( GL_QUADS );
    glTexCoord2d(fontchardefs[ch].p0.x, fontchardefs[ch].p0.y); glVertex2f(p0.x, p0.y);
	GLdouble maxCoordX = static_cast<GLdouble>(fontchardefs[ch].p0.x) + static_cast<GLdouble>(FONT_CHAR_WIDTH);
	GLdouble maxCoordY = static_cast<GLdouble>(fontchardefs[ch].p0.y) + static_cast<GLdouble>(FONT_CHAR_HEIGHT);
    glTexCoord2d(maxCoordX, fontchardefs[ch].p0.y); glVertex2f(p1.x, p0.y);
    glTexCoord2d(maxCoordX, maxCoordY); glVertex2f(p1.x, p1.y);
    glTexCoord2d(fontchardefs[ch].p0.x, maxCoordY); glVertex2f(p0.x, p1.y);
    glEnd();
  }
}

//-----------------------------------------------------------------------------
// Draw a string in a window position.
void FONT_DrawString(const vec2 &p0, const char string[])
{
  int n = (int)strlen(string);
  vec2 vPos = p0;
  for (int i = 0; i < n; i++)
  {
    FONT_DrawChar(vPos, vPos + vec2(16.f, 16.f), string[i]);
    vPos = vPos + vec2(16.f, 0.f);
  }
}
