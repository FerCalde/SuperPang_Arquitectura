#ifndef _FONT_H_
#define _FONT_H_

// Forward definition
class vec2;

//-----------------------------------------------------------------------------
// Load fonts.
void FONT_Init();
//-----------------------------------------------------------------------------
// Unload fonts.
void FONT_End();
//-----------------------------------------------------------------------------
// Draw a char. in a position.
// p0, p1:	Min and max window position to draw a char.
void FONT_DrawChar(const vec2 &p0, const vec2 &p1, unsigned char ch);
//-----------------------------------------------------------------------------
// Draw a string in a window position.
void FONT_DrawString(const vec2 &p0, const char string[]);

#endif
