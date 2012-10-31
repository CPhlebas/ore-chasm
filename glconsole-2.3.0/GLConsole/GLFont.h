/*
    \file GLFont.h 

    This Code is covered under the LGPL.  See COPYING file for the license.

    $Id: GLFont.h 183 2010-07-18 15:20:20Z effer $
 */

#ifndef __GL_FONT_H__
#define __GL_FONT_H__

#ifdef HAVE_APPLE_OPENGL_FRAMEWORK
#    include <OpenGL/gl.h>
#    include <OpenGL/glu.h>
#    include <GLUT/glut.h>
#else
#    ifdef WIN32
#      include <windows.h>
#    endif
#    include <GL/gl.h>
#    include <GL/glu.h> 
#    include <GL/glut.h>
#endif

#include <SFML/Graphics.hpp>

//#include <glutint.h>
//#include <GLUT/glutbitmap.h>

#include <assert.h>
#include <stdio.h>

#include <cstdarg>
#include <string>
#include <string.h>


#define MAX_TEXT_LENGTH 512

///
class GLFont
{
    friend inline bool GLFontCheckInit( GLFont* pFont );
	public:
        GLFont(sf::RenderWindow *_window)
        {
            window = _window;
            m_nNumLists = 96;
            m_nCharWidth = 8;
            m_nCharHeight = 13;
            m_bInitDone = false;
        }
        ~GLFont();        

        // printf style function take position to print to as well
        // NB: coordinates start from bottom left
        void glPrintf(int x, int y, const char *fmt, ...);
        void glPrintf(int x, int y, const std::string fmt, ...){ glPrintf(x,y, fmt.c_str()); }

        unsigned int   CharWidth() { return m_nCharWidth; }
        unsigned int   CharHeight() { return m_nCharHeight; }

    private:
        sf::RenderWindow *window;
        unsigned int   m_nCharWidth; // fixed width
        unsigned int   m_nCharHeight; // fixed width
        int            m_nNumLists;        // number of display lists
        int            m_nDisplayListBase; // base number for display lists
        bool           m_bInitDone;
};

typedef struct {
    const GLsizei width;
    const GLsizei height;
    const GLfloat xorig;
    const GLfloat yorig;
    const GLfloat advance;
    const GLubyte *bitmap;
} BitmapCharRec, *BitmapCharPtr;

typedef struct {
    const char *name;
    const int num_chars;
    const int first;
    const BitmapCharRec * const *ch;
} BitmapFontRec, *BitmapFontPtr;

typedef void *GLUTbitmapFont;

////////////////////////////////////////////////////////////////////////////////
///
inline bool GLFontCheckInit( GLFont* pFont = NULL )
{
    // make sure glutInit has been called
    if( glutGet(GLUT_ELAPSED_TIME) <= 0 ){
        //fprintf( stderr, "WARNING: GLFontCheckInit failed after 'glutGet(GLUT_ELAPSED_TIME) <= 0' check\n" );
        //FIXME: GLUT FAILURE, SINCE I DON"T AND CANNOT USE GLUT...NEED A WAY TO SHOW A FONT
//        return false;
    }

    static int nDisplayListBase = -1;
    if( !pFont->m_bInitDone ) {
        assert( pFont != NULL );
        // GLUT bitmapped fonts...  
        pFont->m_nDisplayListBase = glGenLists( pFont->m_nNumLists );
        if( pFont->m_nDisplayListBase == 0 ) {
//    hmm, commented out for now because on my linux box w get here sometimes
//    even though glut hasn't been initialized.
//            fprintf( stderr, "%i", pFont->m_nNumLists );
            fprintf( stderr, "GLFontCheckInit() -- out of display lists\n");
            return false;
        }
        for( int nList = pFont->m_nDisplayListBase; 
                nList < pFont->m_nDisplayListBase + pFont->m_nNumLists; nList++ ) {
 //           glNewList( nList, GL_COMPILE );
//            glutBitmapCharacter( GLUT_BITMAP_8_BY_13, nList+32-pFont->m_nDisplayListBase );
  //          glEndList();
        }

        nDisplayListBase = pFont->m_nDisplayListBase;
        pFont->m_bInitDone = true;
        return false;
    }
    else{
        assert( nDisplayListBase > 0 );
        pFont->m_nDisplayListBase = nDisplayListBase;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
inline GLFont::~GLFont()
{
    if( m_bInitDone && GLFontCheckInit(this) ) {
//        glDeleteLists( m_nDisplayListBase, m_nDisplayListBase + m_nNumLists );
    } 
}
 
////////////////////////////////////////////////////////////////////////////////
// printf style print function
// NB: coordinates start from bottom left
inline void GLFont::glPrintf(int x, int y, const char *fmt, ...)   
{
    GLFontCheckInit(this);

    if( fmt == NULL ) {                                 // If There's No Text
        return;                                         // Do Nothing
    }


}

#endif
