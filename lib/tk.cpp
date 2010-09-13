/*
 * (c) Copyright 1993, Silicon Graphics, Inc.
 * ALL RIGHTS RESERVED
 * Permission to use, copy, modify, and distribute this software for
 * any purpose and without fee is hereby granted, provided that the above
 * copyright notice appear in all copies and that both the copyright notice
 * and this permission notice appear in supporting documentation, and that
 * the name of Silicon Graphics, Inc. not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission.
 *
 * THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS"
 * AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR
 * FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL SILICON
 * GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY
 * KIND, OR ANY DAMAGES WHATSOEVER, INCLUDING WITHOUT LIMITATION,
 * LOSS OF PROFIT, LOSS OF USE, SAVINGS OR REVENUE, OR THE CLAIMS OF
 * THIRD PARTIES, WHETHER OR NOT SILICON GRAPHICS, INC.  HAS BEEN
 * ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE
 * POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * US Government Users Restricted Rights
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions set forth in FAR 52.227.19(c)(2) or subparagraph
 * (c)(1)(ii) of the Rights in Technical Data and Computer Software
 * clause at DFARS 252.227-7013 and/or in similar or successor
 * clauses in the FAR or the DOD or NASA FAR Supplement.
 * Unpublished-- rights reserved under the copyright laws of the
 * United States.  Contractor/manufacturer is Silicon Graphics,
 * Inc., 2011 N.  Shoreline Blvd., Mountain View, CA 94039-7311.
 *
 * OpenGL(TM) is a trademark of Silicon Graphics, Inc.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/keysym.h>
#include <string>
#include "tk.h"

#if defined(__cplusplus) || defined(c_plusplus)
#define class c_class
#endif


/******************************************************************************/

Window window = 0;

extern int visualize;

static struct _WINDOWINFO {
    int x, y;
    int width, height;
    GLenum type;
} windInfo = {
    -1, -1, 100, 100, 0
};
Display *display = 0;
static XVisualInfo *visualInfo = 0;
/*static Window window = 0;*/
static int screen = 0;
static GLXContext context = 0;
static void (*ExposeFunc)(int, int) = 0;
static void (*ReshapeFunc)(int, int) = 0;
static void (*DisplayFunc)(void) = 0;
static GLenum (*KeyDownFunc)(int, GLenum) = 0;
static GLenum (*MouseDownFunc)(int, int, GLenum) = 0;
static GLenum (*MouseUpFunc)(int, int, GLenum) = 0;
static GLenum (*MouseMoveFunc)(int, int, GLenum) = 0;
static void (*IdleFunc)(void) = 0;
static int lastEventType = -1;
static Colormap colorMap;
static float colorMaps[] = {
    0.000000, 1.000000, 0.000000, 1.000000, 0.000000, 1.000000,
    0.000000, 1.000000, 0.333333, 0.776471, 0.443137, 0.556863,
    0.443137, 0.556863, 0.219608, 0.666667, 0.666667, 0.333333,
    0.666667, 0.333333, 0.666667, 0.333333, 0.666667, 0.333333,
    0.666667, 0.333333, 0.666667, 0.333333, 0.666667, 0.333333,
    0.666667, 0.333333, 0.039216, 0.078431, 0.117647, 0.156863,
    0.200000, 0.239216, 0.278431, 0.317647, 0.356863, 0.400000,
    0.439216, 0.478431, 0.517647, 0.556863, 0.600000, 0.639216,
    0.678431, 0.717647, 0.756863, 0.800000, 0.839216, 0.878431,
    0.917647, 0.956863, 0.000000, 0.000000, 0.000000, 0.000000,
    0.000000, 0.000000, 0.000000, 0.000000, 0.247059, 0.247059,
    0.247059, 0.247059, 0.247059, 0.247059, 0.247059, 0.247059,
    0.498039, 0.498039, 0.498039, 0.498039, 0.498039, 0.498039,
    0.498039, 0.498039, 0.749020, 0.749020, 0.749020, 0.749020,
    0.749020, 0.749020, 0.749020, 0.749020, 1.000000, 1.000000,
    1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000,
    0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
    0.000000, 0.000000, 0.247059, 0.247059, 0.247059, 0.247059,
    0.247059, 0.247059, 0.247059, 0.247059, 0.498039, 0.498039,
    0.498039, 0.498039, 0.498039, 0.498039, 0.498039, 0.498039,
    0.749020, 0.749020, 0.749020, 0.749020, 0.749020, 0.749020,
    0.749020, 0.749020, 1.000000, 1.000000, 1.000000, 1.000000,
    1.000000, 1.000000, 1.000000, 1.000000, 0.000000, 0.000000,
    0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
    0.247059, 0.247059, 0.247059, 0.247059, 0.247059, 0.247059,
    0.247059, 0.247059, 0.498039, 0.498039, 0.498039, 0.498039,
    0.498039, 0.498039, 0.498039, 0.498039, 0.749020, 0.749020,
    0.749020, 0.749020, 0.749020, 0.749020, 0.749020, 0.749020,
    1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000,
    1.000000, 1.000000, 0.000000, 0.000000, 0.000000, 0.000000,
    0.000000, 0.000000, 0.000000, 0.000000, 0.247059, 0.247059,
    0.247059, 0.247059, 0.247059, 0.247059, 0.247059, 0.247059,
    0.498039, 0.498039, 0.498039, 0.498039, 0.498039, 0.498039,
    0.498039, 0.498039, 0.749020, 0.749020, 0.749020, 0.749020,
    0.749020, 0.749020, 0.749020, 0.749020, 1.000000, 1.000000,
    1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000,
    0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
    0.000000, 0.000000, 0.247059, 0.247059, 0.247059, 0.247059,
    0.247059, 0.247059, 0.247059, 0.247059, 0.498039, 0.498039,
    0.498039, 0.498039, 0.498039, 0.498039, 0.498039, 0.498039,
    0.749020, 0.749020, 0.749020, 0.749020, 0.749020, 0.749020,
    0.749020, 0.749020, 1.000000, 1.000000, 1.000000, 1.000000,
    1.000000, 1.000000, 1.000000, 1.000000, 0.000000, 0.000000,
    1.000000, 1.000000, 0.000000, 0.000000, 1.000000, 1.000000,
    0.333333, 0.443137, 0.776471, 0.556863, 0.443137, 0.219608,
    0.556863, 0.666667, 0.666667, 0.333333, 0.666667, 0.333333,
    0.666667, 0.333333, 0.666667, 0.333333, 0.666667, 0.333333,
    0.666667, 0.333333, 0.666667, 0.333333, 0.666667, 0.333333,
    0.039216, 0.078431, 0.117647, 0.156863, 0.200000, 0.239216,
    0.278431, 0.317647, 0.356863, 0.400000, 0.439216, 0.478431,
    0.517647, 0.556863, 0.600000, 0.639216, 0.678431, 0.717647,
    0.756863, 0.800000, 0.839216, 0.878431, 0.917647, 0.956863,
    0.000000, 0.141176, 0.282353, 0.427451, 0.568627, 0.713726,
    0.854902, 1.000000, 0.000000, 0.141176, 0.282353, 0.427451,
    0.568627, 0.713726, 0.854902, 1.000000, 0.000000, 0.141176,
    0.282353, 0.427451, 0.568627, 0.713726, 0.854902, 1.000000,
    0.000000, 0.141176, 0.282353, 0.427451, 0.568627, 0.713726,
    0.854902, 1.000000, 0.000000, 0.141176, 0.282353, 0.427451,
    0.568627, 0.713726, 0.854902, 1.000000, 0.000000, 0.141176,
    0.282353, 0.427451, 0.568627, 0.713726, 0.854902, 1.000000,
    0.000000, 0.141176, 0.282353, 0.427451, 0.568627, 0.713726,
    0.854902, 1.000000, 0.000000, 0.141176, 0.282353, 0.427451,
    0.568627, 0.713726, 0.854902, 1.000000, 0.000000, 0.141176,
    0.282353, 0.427451, 0.568627, 0.713726, 0.854902, 1.000000,
    0.000000, 0.141176, 0.282353, 0.427451, 0.568627, 0.713726,
    0.854902, 1.000000, 0.000000, 0.141176, 0.282353, 0.427451,
    0.568627, 0.713726, 0.854902, 1.000000, 0.000000, 0.141176,
    0.282353, 0.427451, 0.568627, 0.713726, 0.854902, 1.000000,
    0.000000, 0.141176, 0.282353, 0.427451, 0.568627, 0.713726,
    0.854902, 1.000000, 0.000000, 0.141176, 0.282353, 0.427451,
    0.568627, 0.713726, 0.854902, 1.000000, 0.000000, 0.141176,
    0.282353, 0.427451, 0.568627, 0.713726, 0.854902, 1.000000,
    0.000000, 0.141176, 0.282353, 0.427451, 0.568627, 0.713726,
    0.854902, 1.000000, 0.000000, 0.141176, 0.282353, 0.427451,
    0.568627, 0.713726, 0.854902, 1.000000, 0.000000, 0.141176,
    0.282353, 0.427451, 0.568627, 0.713726, 0.854902, 1.000000,
    0.000000, 0.141176, 0.282353, 0.427451, 0.568627, 0.713726,
    0.854902, 1.000000, 0.000000, 0.141176, 0.282353, 0.427451,
    0.568627, 0.713726, 0.854902, 1.000000, 0.000000, 0.141176,
    0.282353, 0.427451, 0.568627, 0.713726, 0.854902, 1.000000,
    0.000000, 0.141176, 0.282353, 0.427451, 0.568627, 0.713726,
    0.854902, 1.000000, 0.000000, 0.141176, 0.282353, 0.427451,
    0.568627, 0.713726, 0.854902, 1.000000, 0.000000, 0.141176,
    0.282353, 0.427451, 0.568627, 0.713726, 0.854902, 1.000000,
    0.000000, 0.141176, 0.282353, 0.427451, 0.568627, 0.713726,
    0.854902, 1.000000, 0.000000, 0.000000, 0.000000, 0.000000,
    1.000000, 1.000000, 1.000000, 1.000000, 0.333333, 0.443137,
    0.443137, 0.219608, 0.776471, 0.556863, 0.556863, 0.666667,
    0.666667, 0.333333, 0.666667, 0.333333, 0.666667, 0.333333,
    0.666667, 0.333333, 0.666667, 0.333333, 0.666667, 0.333333,
    0.666667, 0.333333, 0.666667, 0.333333, 0.039216, 0.078431,
    0.117647, 0.156863, 0.200000, 0.239216, 0.278431, 0.317647,
    0.356863, 0.400000, 0.439216, 0.478431, 0.517647, 0.556863,
    0.600000, 0.639216, 0.678431, 0.717647, 0.756863, 0.800000,
    0.839216, 0.878431, 0.917647, 0.956863, 0.000000, 0.000000,
    0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
    0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
    0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
    0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
    0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
    0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
    0.000000, 0.000000, 0.247059, 0.247059, 0.247059, 0.247059,
    0.247059, 0.247059, 0.247059, 0.247059, 0.247059, 0.247059,
    0.247059, 0.247059, 0.247059, 0.247059, 0.247059, 0.247059,
    0.247059, 0.247059, 0.247059, 0.247059, 0.247059, 0.247059,
    0.247059, 0.247059, 0.247059, 0.247059, 0.247059, 0.247059,
    0.247059, 0.247059, 0.247059, 0.247059, 0.247059, 0.247059,
    0.247059, 0.247059, 0.247059, 0.247059, 0.247059, 0.247059,
    0.498039, 0.498039, 0.498039, 0.498039, 0.498039, 0.498039,
    0.498039, 0.498039, 0.498039, 0.498039, 0.498039, 0.498039,
    0.498039, 0.498039, 0.498039, 0.498039, 0.498039, 0.498039,
    0.498039, 0.498039, 0.498039, 0.498039, 0.498039, 0.498039,
    0.498039, 0.498039, 0.498039, 0.498039, 0.498039, 0.498039,
    0.498039, 0.498039, 0.498039, 0.498039, 0.498039, 0.498039,
    0.498039, 0.498039, 0.498039, 0.498039, 0.749020, 0.749020,
    0.749020, 0.749020, 0.749020, 0.749020, 0.749020, 0.749020,
    0.749020, 0.749020, 0.749020, 0.749020, 0.749020, 0.749020,
    0.749020, 0.749020, 0.749020, 0.749020, 0.749020, 0.749020,
    0.749020, 0.749020, 0.749020, 0.749020, 0.749020, 0.749020,
    0.749020, 0.749020, 0.749020, 0.749020, 0.749020, 0.749020,
    0.749020, 0.749020, 0.749020, 0.749020, 0.749020, 0.749020,
    0.749020, 0.749020, 1.000000, 1.000000, 1.000000, 1.000000,
    1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000,
    1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000,
    1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000,
    1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000,
    1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000,
    1.000000, 1.000000, 1.000000, 1.000000, 1.000000, 1.000000,
};
float tkRGBMap[8][3] = {
    {
        0, 0, 0
    },
    {
        1, 0, 0
    },
    {
        0, 1, 0
    },
    {
        1, 1, 0
    },
    {
        0, 0, 1
    },
    {
        1, 0, 1
    },
    {
        0, 1, 1
    },
    {
        1, 1, 1
    }
};

/******************************************************************************/

void tkCloseWindow(void)
{

    if (display) {
        glFlush();
        glFinish();
        XDestroyWindow(display, window);
        glXDestroyContext(display, context);
        XFreeColormap(display, colorMap);
        XFree((char *)visualInfo);
        XCloseDisplay(display);
        display = 0;

        ExposeFunc = 0;
        ReshapeFunc = 0;
        IdleFunc = 0;
        DisplayFunc = 0;
        KeyDownFunc = 0;
        MouseDownFunc = 0;
        MouseUpFunc = 0;
        MouseMoveFunc = 0;

        lastEventType = -1;
    }
}

/******************************************************************************/

static GLenum DoNextEvent(void)
{
    XEvent current, ahead;
    char buf[1000];
    char hist[1000]; static int hist_ptr=0;
    KeySym ks;
    int key;

    XNextEvent(display, &current);
    switch (current.type) {
      case MappingNotify:
        XRefreshKeyboardMapping((XMappingEvent *)&current);
        lastEventType = MappingNotify;
        return GL_FALSE;

      case Expose:
        while (XEventsQueued(current.xexpose.display, QueuedAfterReading) > 0) {
            XPeekEvent(current.xexpose.display, &ahead);
            if (ahead.xexpose.window != current.xexpose.window ||
                ahead.type != Expose) {
                break;
            }
            XNextEvent(display, &current);
        }
        if (current.xexpose.count == 0) {
            if (ExposeFunc) {
                (*ExposeFunc)(windInfo.width, windInfo.height);
                if (lastEventType == ConfigureNotify) {
                    lastEventType = Expose;
                    return GL_FALSE;
                } else {
                    lastEventType = Expose;
                    return GL_TRUE;
                }
            }
        }
        return GL_FALSE;

      case ConfigureNotify:
        lastEventType = ConfigureNotify;
        windInfo.width = current.xconfigure.width;
        windInfo.height = current.xconfigure.height;
        if (ReshapeFunc) {
            (*ReshapeFunc)(windInfo.width, windInfo.height);
            return GL_TRUE;
        } else {
            return GL_FALSE;
        }

      case MotionNotify:
        lastEventType = MotionNotify;
        if (MouseMoveFunc) {
            GLenum mask;
            /*
              printf ("state = %d, time %d\n", ((XMotionEvent&)current).state,
              ((XMotionEvent&)current).time);
            */
            // mask = 0;
            mask = current.xbutton.state;
            // if (current.xmotion.state & Button1Mask) {
            //     mask |= TK_LEFTBUTTON;
            // }
            // if (current.xmotion.state & Button2Mask) {
            //     mask |= TK_MIDDLEBUTTON;
            // }
            // if (current.xmotion.state & Button3Mask) {
            //     mask |= TK_RIGHTBUTTON;
            // }
            // if (current.xmotion.state & ShiftMask) {
            //     mask |= 8;
            // }
            return (*MouseMoveFunc)(current.xmotion.x, current.xmotion.y, mask);
        } else {
            return GL_FALSE;
        }

      case ButtonPress:
        lastEventType = ButtonPress;
        if (MouseDownFunc) {
            GLenum mask;

            mask = 0;
            if (current.xbutton.button == 1) {
                mask |= TK_LEFTBUTTON;
            }
            if (current.xbutton.button == 2) {
                mask |= TK_MIDDLEBUTTON;
            }
            if (current.xbutton.button == 3) {
                mask |= TK_RIGHTBUTTON;
            }
            mask |= (current.xbutton.state << 3);
            return (*MouseDownFunc)(current.xbutton.x, current.xbutton.y, mask);
        } else {
            return GL_FALSE;
        }
      case ButtonRelease:
        lastEventType = ButtonRelease;
        if (MouseUpFunc) {
            GLenum mask;

            mask = 0;
            if (current.xbutton.button == 1) {
                mask |= TK_LEFTBUTTON;
            }
            if (current.xbutton.button == 2) {
                mask |= TK_MIDDLEBUTTON;
            }
            if (current.xbutton.button == 3) {
                mask |= TK_RIGHTBUTTON;
            }
            mask |= (current.xbutton.state << 3);
            return (*MouseUpFunc)(current.xbutton.x, current.xbutton.y, mask);
        } else {
            return GL_FALSE;
        }

      case KeyPress:
        lastEventType = KeyPress;
        XLookupString(&current.xkey, buf, sizeof(buf), &ks, 0);

        // save the keystrokes
        switch (ks) {
        case XK_Left:
          sprintf(hist+hist_ptr,"%s","~l"); hist_ptr+=2;
          break;
        case XK_Right:
          sprintf(hist+hist_ptr,"%s","~r"); hist_ptr+=2;
          break;
        case XK_Up:
          sprintf(hist+hist_ptr,"%s","~u"); hist_ptr+=2;
          break;
        case XK_Down:
          sprintf(hist+hist_ptr,"%s","~d"); hist_ptr+=2;
          break;
        case XK_KP_Decimal:
        case XK_period:
          sprintf(hist+hist_ptr,"%s","~."); hist_ptr+=2;
          break;
        case XK_KP_Enter:
        case TK_RETURN:
          sprintf(hist+hist_ptr,"%s","~E"); hist_ptr+=2;
          break;
        default:
          if ( (((int)ks - XK_KP_0) < 10) && (((int)ks - XK_KP_9) >= 0) ) {
             sprintf(hist+hist_ptr,"%d",(int)ks - XK_KP_0); hist_ptr+=1;
          } else {
            sprintf(hist+hist_ptr,"%s",buf); hist_ptr+=strlen(buf);
          }
        }

        switch (ks) {
          case XK_0:            key = TK_0;		break;
          case XK_1:            key = TK_1;		break;
          case XK_2:            key = TK_2;		break;
          case XK_3:            key = TK_3;		break;
          case XK_4:            key = TK_4;		break;
          case XK_5:            key = TK_5;		break;
          case XK_6:            key = TK_6;		break;
          case XK_7:            key = TK_7;		break;
          case XK_8:            key = TK_8;		break;
          case XK_9:            key = TK_9;		break;
          case XK_A:            key = TK_A;		break;
          case XK_B:            key = TK_B;		break;
          case XK_C:            key = TK_C;		break;
          case XK_D:            key = TK_D;		break;
          case XK_E:            key = TK_E;		break;
          case XK_F:            key = TK_F;		break;
          case XK_G:            key = TK_G;		break;
          case XK_H:            key = TK_H;		break;
          case XK_I:            key = TK_I;		break;
          case XK_J:            key = TK_J;		break;
          case XK_K:            key = TK_K;		break;
          case XK_L:            key = TK_L;		break;
          case XK_M:            key = TK_M;		break;
          case XK_N:            key = TK_N;		break;
          case XK_O:            key = TK_O;		break;
          case XK_P:            key = TK_P;		break;
          case XK_Q:            key = TK_Q;		break;
          case XK_R:            key = TK_R;		break;
          case XK_S:            key = TK_S;		break;
          case XK_T:            key = TK_T;		break;
          case XK_U:            key = TK_U;		break;
          case XK_V:            key = TK_V;		break;
          case XK_W:            key = TK_W;		break;
          case XK_X:            key = TK_X;		break;
          case XK_Y:            key = TK_Y;		break;
          case XK_Z:            key = TK_Z;		break;
          case XK_a:            key = TK_a;		break;
          case XK_b:            key = TK_b;		break;
          case XK_c:            key = TK_c;		break;
          case XK_d:            key = TK_d;		break;
          case XK_e:            key = TK_e;		break;
          case XK_f:            key = TK_f;		break;
          case XK_g:            key = TK_g;		break;
          case XK_h:            key = TK_h;		break;
          case XK_i:            key = TK_i;		break;
          case XK_j:            key = TK_j;		break;
          case XK_k:            key = TK_k;		break;
          case XK_l:            key = TK_l;		break;
          case XK_m:            key = TK_m;		break;
          case XK_n:            key = TK_n;             break;
          case XK_o:            key = TK_o;		break;
          case XK_p:            key = TK_p;		break;
          case XK_q:            key = TK_q;		break;
          case XK_r:            key = TK_r;		break;
          case XK_s:            key = TK_s;		break;
          case XK_t:            key = TK_t;		break;
          case XK_u:            key = TK_u;		break;
          case XK_v:            key = TK_v;		break;
          case XK_w:            key = TK_w;		break;
          case XK_x:            key = TK_x;		break;
          case XK_y:            key = TK_y;		break;
          case XK_z:            key = TK_z;		break;
          case XK_space:	key = TK_SPACE;		break;
          case XK_Return:       key = TK_RETURN;	break;

          case XK_Escape:       key = TK_ESCAPE;
                                // exit(1);
                                break;

          case XK_Left:		key = TK_LEFT;		break;
          case XK_Up:		key = TK_UP;		break;
          case XK_Right:        key = TK_RIGHT;		break;
          case XK_Down:		key = TK_DOWN;		break;

          case XK_plus:		key = XK_plus;		break;
          case XK_minus:	key = XK_minus;		break;
          case XK_asterisk:     key = XK_asterisk;      break;
          case XK_slash:        key = XK_slash;         break;
          case XK_equal:	key = XK_equal;		break;

          case XK_comma:	key = XK_comma;		break;
          case XK_less:         key = XK_less;		break;
          case XK_backslash:    key = XK_backslash;     break;
          case XK_grave:        key = XK_grave;         break;
          case XK_asciitilde:   key = XK_asciitilde;    break;
          case XK_exclam:       key = XK_exclam;        break;
          case XK_at:           key = XK_at;            break;
          case XK_bracketleft:  key = XK_bracketleft;   break;
          case XK_bracketright: key = XK_bracketright;  break;
          case XK_parenleft:    key = XK_parenleft;     break;
          case XK_parenright:   key = XK_parenright;    break;

          case XK_F1:		key = XK_F1;
                                printf("display: %p\n",(void *)display);
                                printf("window:  %p\n",(void *)window);
                                printf("keys:    %s\n",hist);
                                                        break;

          case XK_F2:		key = XK_F2;		break;
          case XK_F3:		key = XK_F3;		break;
          case XK_F4:		key = XK_F4;		break;
          case XK_F5:		key = XK_F5;		break;
          case XK_F6:		key = XK_F6;		break;
          case XK_F7:		key = XK_F7;		break;
          case XK_F8:		key = XK_F8;		break;
          case XK_F9:		key = XK_F9;		break;
          case XK_F10:		key = XK_F10;		break;
          case XK_F11:		key = XK_F11;		break;
          case XK_F12:		key = XK_F12;		break;

          case XK_Num_Lock:	key = XK_Num_Lock;      break;
          case XK_KP_0:         key = XK_KP_0;          break;
          case XK_KP_1:         key = XK_KP_1;          break;
          case XK_KP_2:         key = XK_KP_2;          break;
          case XK_KP_3:         key = XK_KP_3;          break;
          case XK_KP_4:         key = XK_KP_4;          break;
          case XK_KP_5:         key = XK_KP_5;          break;
          case XK_KP_6:         key = XK_KP_6;          break;
          case XK_KP_7:         key = XK_KP_7;          break;
          case XK_KP_8:         key = XK_KP_8;          break;
          case XK_KP_9:         key = XK_KP_9;          break;

          case XK_KP_Enter:     key = XK_KP_Enter;      break;
          case XK_KP_Home:      key = XK_KP_Home;       break;
          case XK_KP_Left:      key = XK_KP_Left;       break;
          case XK_KP_Up	:       key = XK_KP_Up;         break;
          case XK_KP_Right:     key = XK_KP_Right;      break;
          case XK_KP_Down:      key = XK_KP_Down;       break;
          case XK_KP_Page_Up:   key = XK_KP_Page_Up;    break;
          case XK_KP_Page_Down: key = XK_KP_Page_Down;  break;
          case XK_KP_End:       key = XK_KP_End	;       break;
          case XK_KP_Begin:     key = XK_KP_Begin;      break;
          case XK_KP_Insert:    key = XK_KP_Insert;     break;
          case XK_KP_Delete:    key = XK_KP_Delete;     break;
          case XK_KP_Equal:     key = XK_KP_Equal;      break;
          case XK_KP_Multiply:  key = XK_KP_Multiply;   break;
          case XK_KP_Add:       key = XK_KP_Add;        break;
          case XK_KP_Separator: key = XK_KP_Separator;  break;
          case XK_KP_Subtract:  key = XK_KP_Subtract;   break;
          case XK_KP_Decimal:   key = XK_KP_Decimal;    break;
          case XK_KP_Divide:    key = XK_KP_Divide;     break;
          case XK_period:       key = XK_period;        break;

          default:              key = GL_FALSE;		break;
        }
        if (key && KeyDownFunc) {
            GLenum mask;

            mask = 0;
            if (current.xkey.state & ControlMask) {
                mask |= TK_CONTROL;
            }
            if (current.xkey.state & ShiftMask) {
                mask |= TK_SHIFT;
            }
            // printf("key: 0x%04X   mask: 0x%04X\n", key, mask); fflush(stdout);
            return (*KeyDownFunc)(key, mask);
        } else {
            return GL_FALSE;
        }
    }
    return GL_FALSE;
}

void tkExec(void)
{
   GLenum flag;

   visualize = 1;
   while (visualize)
   {
      if (IdleFunc)
      {
         (*IdleFunc)();
         flag = GL_FALSE;
         while (XPending(display))
            flag |= DoNextEvent();
         if (flag == GL_TRUE)
            if (DisplayFunc)
               (*DisplayFunc)();
      }
      else
      {
         if (DoNextEvent() == GL_TRUE)
            if (DisplayFunc)
               (*DisplayFunc)();
      }
   }
}

void tkExposeFunc(void (*Func)(int, int))
{

    ExposeFunc = Func;
}

void tkReshapeFunc(void (*Func)(int, int))
{

    ReshapeFunc = Func;
}

void tkDisplayFunc(void (*Func)(void))
{

    DisplayFunc = Func;
}

void tkKeyDownFunc(GLenum (*Func)(int, GLenum))
{

    KeyDownFunc = Func;
}

void tkMouseDownFunc(GLenum (*Func)(int, int, GLenum))
{

    MouseDownFunc = Func;
}

void tkMouseUpFunc(GLenum (*Func)(int, int, GLenum))
{

    MouseUpFunc = Func;
}

void tkMouseMoveFunc(GLenum (*Func)(int, int, GLenum))
{

    MouseMoveFunc = Func;
}

void tkIdleFunc(void (*Func)(void))
{

    IdleFunc = Func;
}

/******************************************************************************/

GLint tkGetColorMapSize(void)
{

    if (!display) {
        return 0;
    } else {
        return visualInfo->colormap_size;
    }
}

void tkGetMouseLoc(int *x, int *y)
{
    int junk;

    *x = 0;
    *y = 0;
    XQueryPointer(display, window, (Window *)&junk, (Window *)&junk,
                  &junk, &junk, x, y, (unsigned int *)&junk);
}

Display *tkGetXDisplay(void)
{

    return display;
}

Window tkGetXWindow(void)
{

    return window;
}

/******************************************************************************/

static XVisualInfo *FindVisual(GLenum type)
{
    GLenum list[50];
    int i;

    i = 0;

    if (TK_IS_DOUBLE(type)) {
        list[i++] = GLX_DOUBLEBUFFER;
    }

    if (TK_IS_RGB(type)) {
        list[i++] = GLX_RGBA;
        list[i++] = GLX_RED_SIZE;
        list[i++] = 1;
        list[i++] = GLX_GREEN_SIZE;
        list[i++] = 1;
        list[i++] = GLX_BLUE_SIZE;
        list[i++] = 1;
        if (TK_HAS_ALPHA(type)) {
            list[i++] = GLX_ALPHA_SIZE;
            list[i++] = 1;
        }
        if (TK_HAS_ACCUM(type)) {
            list[i++] = GLX_ACCUM_RED_SIZE;
            list[i++] = 1;
            list[i++] = GLX_ACCUM_GREEN_SIZE;
            list[i++] = 1;
            list[i++] = GLX_ACCUM_BLUE_SIZE;
            list[i++] = 1;
            list[i++] = GLX_ACCUM_ALPHA_SIZE;
            list[i++] = 1;
        }
    } else {
        list[i++] = GLX_BUFFER_SIZE;
        list[i++] = 1;
    }

    if (TK_HAS_DEPTH(type)) {
        list[i++] = GLX_DEPTH_SIZE;
        list[i++] = 1;
    }

    if (TK_HAS_STENCIL(type)) {
        list[i++] = GLX_STENCIL_SIZE;
        list[i++] = 1;
    }

    // multisampling
#ifdef GLVIS_MULTISAMPLE
#ifdef GLX_SAMPLE_BUFFERS_ARB
    std::string s = glXQueryExtensionsString(display, screen);
    if (s.find("GLX_ARB_multisample") != std::string::npos)
    {
       list[i++] = GLX_SAMPLE_BUFFERS_ARB;
       list[i++] = 1;
       list[i++] = GLX_SAMPLES_ARB;
       list[i++] = GLVIS_MULTISAMPLE;
    }
#else
#error GLX_SAMPLE_BUFFERS_ARB is not defined!
#endif
#endif

    list[i] = (int)None;

    return glXChooseVisual(display, screen, (int *)list);
}

static int MakeVisualType(XVisualInfo *vi)
{
    GLenum mask;
    int x, y, z;

    mask = 0;

    glXGetConfig(display, vi, GLX_RGBA, &x);
#ifdef GLVIS_DEBUG
    printf("GLX_RGBA : %d\n", x);
#endif
    if (x) {
        mask |= TK_RGB;
#ifdef GLVIS_DEBUG
        glXGetConfig(display, vi, GLX_RED_SIZE, &x);
        glXGetConfig(display, vi, GLX_GREEN_SIZE, &y);
        glXGetConfig(display, vi, GLX_BLUE_SIZE, &z);
        printf("GLX_RED_SIZE   : %d\n", x);
        printf("GLX_GREEN_SIZE : %d\n", y);
        printf("GLX_BLUE_SIZE  : %d\n", z);
#endif
        glXGetConfig(display, vi, GLX_ALPHA_SIZE, &x);
#ifdef GLVIS_DEBUG
        printf("GLX_ALPHA_SIZE : %d\n", x);
#endif
        if (x > 0) {
            mask |= TK_ALPHA;
        }
        glXGetConfig(display, vi, GLX_ACCUM_RED_SIZE, &x);
        glXGetConfig(display, vi, GLX_ACCUM_GREEN_SIZE, &y);
        glXGetConfig(display, vi, GLX_ACCUM_BLUE_SIZE, &z);
#ifdef GLVIS_DEBUG
        printf("GLX_ACCUM_RED_SIZE   : %d\n", x);
        printf("GLX_ACCUM_GREEN_SIZE : %d\n", y);
        printf("GLX_ACCUM_BLUE_SIZE  : %d\n", z);
#endif
        if (x > 0 && y > 0 && z > 0) {
            mask |= TK_ACCUM;
        }
#ifdef GLVIS_DEBUG
        glXGetConfig(display, vi, GLX_ACCUM_ALPHA_SIZE, &x);
        printf("GLX_ACCUM_ALPHA_SIZE : %d\n", x);
#endif
    } else {
        mask |= TK_INDEX;
    }

    glXGetConfig(display, vi, GLX_DOUBLEBUFFER, &x);
#ifdef GLVIS_DEBUG
    printf("GLX_DOUBLEBUFFER : %d\n", x);
#endif
    if (x) {
        mask |= TK_DOUBLE;
    } else {
        mask |= TK_SINGLE;
    }

    glXGetConfig(display, vi, GLX_DEPTH_SIZE, &x);
#ifdef GLVIS_DEBUG
    printf("GLX_DEPTH_SIZE : %d\n", x);
#endif
    if (x > 0) {
        mask |= TK_DEPTH;
    }

    glXGetConfig(display, vi, GLX_STENCIL_SIZE, &x);
#ifdef GLVIS_DEBUG
    printf("GLX_STENCIL_SIZE : %d\n", x);
#endif
    if (x > 0) {
        mask |= TK_STENCIL;
    }

#ifdef GLVIS_MULTISAMPLE
#ifdef GLVIS_DEBUG
#ifdef GLX_SAMPLE_BUFFERS_ARB
    glXGetConfig(display, vi, GLX_SAMPLE_BUFFERS_ARB, &x);
    printf("GLX_SAMPLE_BUFFERS_ARB : %d\n", x);
    glXGetConfig(display, vi, GLX_SAMPLES_ARB, &x);
    printf("GLX_SAMPLES_ARB : %d\n", x);
#endif
#endif
#endif

    if (glXIsDirect(display, context)) {
        mask |= TK_DIRECT;
    } else {
        mask |= TK_INDIRECT;
    }
#ifdef GLVIS_DEBUG
    printf("glXIsDirect : %d\n", glXIsDirect(display, context));
#endif

    return mask;
}

static int WaitForMapNotify(Display *d, XEvent *e, char *arg)
{

    if (e->type == MapNotify && e->xmap.window == window) {
        return GL_TRUE;
    }
    return GL_FALSE;
}

void tkInitPosition(int x, int y, int width, int height)
{

    windInfo.x = x;
    windInfo.y = y;
    windInfo.width = width;
    windInfo.height = height;
}

void tkInitDisplayMode(GLenum type)
{

    windInfo.type = type;
}

GLenum tkInitWindow(const char *title)
{
    XSetWindowAttributes wa;
    XSizeHints sh;
    XEvent e;
    int erb, evb;
    unsigned long mask;

    if (!display) {
        display = XOpenDisplay(0);
        if (!display) {
            fprintf(stderr, "Can't connect to display!\n");
            return GL_FALSE;
        }
        if (!glXQueryExtension(display, &erb, &evb)) {
            fprintf(stderr, "No glx extension!\n");
            return GL_FALSE;
        }
        screen = DefaultScreen(display);
    }

    visualInfo = FindVisual(windInfo.type);
    if (!visualInfo) {
        fprintf(stderr, "Window type not found!\n");
        return GL_FALSE;
    }

    context = glXCreateContext(display, visualInfo, None,
                               (TK_IS_DIRECT(windInfo.type)) ? GL_TRUE :
                               GL_FALSE);
    if (!context) {
        fprintf(stderr, "Can't create a context!\n");
        return GL_FALSE;
    }

    windInfo.type = MakeVisualType(visualInfo);

    if (TK_IS_INDEX(windInfo.type)) {
        if (visualInfo->class != StaticColor &&
            visualInfo->class != StaticGray) {
            colorMap = XCreateColormap(display, RootWindow(display, screen),
                                       visualInfo->visual, AllocAll);
        } else {
            colorMap = XCreateColormap(display, RootWindow(display, screen),
                                       visualInfo->visual, AllocNone);
        }
        wa.colormap = colorMap;
        tkSetRGBMap(256, colorMaps);
        wa.background_pixel = 7;
        wa.border_pixel = 0;
    } else {
        colorMap = XCreateColormap(display, RootWindow(display, screen),
                                   visualInfo->visual, AllocNone);
        wa.colormap = colorMap;
        tkSetRGBMap(256, colorMaps);
        wa.background_pixel = 0xFFFFFFFF;
        wa.border_pixel = 0;
    }
    wa.event_mask = StructureNotifyMask | ExposureMask | KeyPressMask |
                    ButtonPressMask | ButtonReleaseMask | PointerMotionMask;
    mask = CWBackPixel | CWBorderPixel | CWEventMask | CWColormap;
    window = XCreateWindow(display, RootWindow(display, screen), windInfo.x,
                           windInfo.y,
                           windInfo.width, windInfo.height, 0,
                           visualInfo->depth, InputOutput, visualInfo->visual,
                           mask, &wa);

    if (windInfo.x != -1 && windInfo.y != -1) {
        sh.flags = USPosition;
        sh.x = windInfo.x + 10;
        sh.y = windInfo.y + 10;
        XSetStandardProperties(display, window, title, title, None, 0, 0, &sh);
    } else {
        XSetStandardProperties(display, window, title, title, None, 0, 0, 0);
    }

    XMapWindow(display, window);
    XIfEvent(display, &e, WaitForMapNotify, 0);

    XSetWMColormapWindows(display, window, &window, 1);
    if (!glXMakeCurrent(display, window, context)) {
        return GL_FALSE;
    }
    XFlush(display);

    return GL_TRUE;
}

/******************************************************************************/

void tkQuit(void)
{

    exit(0);
}

/******************************************************************************/

static int Ignore(Display *parm1, XErrorEvent *parm2)
{

    return 0;
}

void tkSetOneColor(int index, float r, float g, float b)
{
    XErrorHandler old_handler;
    XColor c;
    int rShift, gShift, bShift;

    old_handler = XSetErrorHandler(Ignore);

    switch (visualInfo->class) {
      case DirectColor:
        rShift = ffs((unsigned int)visualInfo->red_mask) - 1;
        gShift = ffs((unsigned int)visualInfo->green_mask) - 1;
        bShift = ffs((unsigned int)visualInfo->blue_mask) - 1;
        c.pixel = ((index << rShift) & visualInfo->red_mask) |
                  ((index << gShift) & visualInfo->green_mask) |
                  ((index << bShift) & visualInfo->blue_mask);
        c.red = (unsigned short)(r * 65535.0 + 0.5);
        c.green = (unsigned short)(g * 65535.0 + 0.5);
        c.blue = (unsigned short)(b * 65535.0 + 0.5);
        c.flags = DoRed | DoGreen | DoBlue;
        XStoreColor(display, colorMap, &c);
        break;
      case GrayScale:
      case PseudoColor:
        if (index < visualInfo->colormap_size) {
            c.pixel = index;
            c.red = (unsigned short)(r * 65535.0 + 0.5);
            c.green = (unsigned short)(g * 65535.0 + 0.5);
            c.blue = (unsigned short)(b * 65535.0 + 0.5);
            c.flags = DoRed | DoGreen | DoBlue;
            XStoreColor(display, colorMap, &c);
        }
        break;
    }

    XSync(display, 0);
    XSetErrorHandler(old_handler);
}

void tkSetFogRamp(int density, int startIndex)
{
    XErrorHandler old_handler;
    XColor c[256];
    int rShift, gShift, bShift, intensity, fogValues, colorValues;
    int i, j, k;

    old_handler = XSetErrorHandler(Ignore);

    switch (visualInfo->class) {
      case DirectColor:
        fogValues = 1 << density;
        colorValues = 1 << startIndex;
        for (i = 0; i < colorValues; i++) {
            for (j = 0; j < fogValues; j++) {
                k = i * fogValues + j;
                intensity = i * fogValues + j * colorValues;
                if (intensity > visualInfo->colormap_size) {
                    intensity = visualInfo->colormap_size;
                }
                intensity = (intensity << 8) | intensity;
                rShift = ffs((unsigned int)visualInfo->red_mask) - 1;
                gShift = ffs((unsigned int)visualInfo->green_mask) - 1;
                bShift = ffs((unsigned int)visualInfo->blue_mask) - 1;
                c[k].pixel = ((k << rShift) & visualInfo->red_mask) |
                             ((k << gShift) & visualInfo->green_mask) |
                             ((k << bShift) & visualInfo->blue_mask);
                c[k].red = (unsigned short)intensity;
                c[k].green = (unsigned short)intensity;
                c[k].blue = (unsigned short)intensity;
                c[k].flags = DoRed | DoGreen | DoBlue;
            }
        }
        XStoreColors(display, colorMap, c, visualInfo->colormap_size);
        break;
      case GrayScale:
      case PseudoColor:
        fogValues = 1 << density;
        colorValues = 1 << startIndex;
        for (i = 0; i < colorValues; i++) {
            for (j = 0; j < fogValues; j++) {
                k = i * fogValues + j;
                intensity = i * fogValues + j * colorValues;
                if (intensity > visualInfo->colormap_size) {
                    intensity = visualInfo->colormap_size;
                }
                intensity = (intensity << 8) | intensity;
                c[k].pixel = k;
                c[k].red = (unsigned short)intensity;
                c[k].green = (unsigned short)intensity;
                c[k].blue = (unsigned short)intensity;
                c[k].flags = DoRed | DoGreen | DoBlue;
            }
        }
        XStoreColors(display, colorMap, c, visualInfo->colormap_size);
        break;
    }

    XSync(display, 0);
    XSetErrorHandler(old_handler);
}

void tkSetGreyRamp(void)
{
    XErrorHandler old_handler;
    XColor c[256];
    float intensity;
    int rShift, gShift, bShift, i;

    old_handler = XSetErrorHandler(Ignore);

    switch (visualInfo->class) {
      case DirectColor:
        for (i = 0; i < visualInfo->colormap_size; i++) {
            intensity = (float)i / (float)visualInfo->colormap_size *
                        65535.0 + 0.5;
            rShift = ffs((unsigned int)visualInfo->red_mask) - 1;
            gShift = ffs((unsigned int)visualInfo->green_mask) - 1;
            bShift = ffs((unsigned int)visualInfo->blue_mask) - 1;
            c[i].pixel = ((i << rShift) & visualInfo->red_mask) |
                         ((i << gShift) & visualInfo->green_mask) |
                         ((i << bShift) & visualInfo->blue_mask);
            c[i].red = (unsigned short)intensity;
            c[i].green = (unsigned short)intensity;
            c[i].blue = (unsigned short)intensity;
            c[i].flags = DoRed | DoGreen | DoBlue;
        }
        XStoreColors(display, colorMap, c, visualInfo->colormap_size);
        break;
      case GrayScale:
      case PseudoColor:
        for (i = 0; i < visualInfo->colormap_size; i++) {
            intensity = (float)i / (float)visualInfo->colormap_size *
                        65535.0 + 0.5;
            c[i].pixel = i;
            c[i].red = (unsigned short)intensity;
            c[i].green = (unsigned short)intensity;
            c[i].blue = (unsigned short)intensity;
            c[i].flags = DoRed | DoGreen | DoBlue;
        }
        XStoreColors(display, colorMap, c, visualInfo->colormap_size);
        break;
    }

    XSync(display, 0);
    XSetErrorHandler(old_handler);
}

void tkSetRGBMap(int size, float *rgb)
{
    XErrorHandler old_handler;
    XColor c;
    int rShift, gShift, bShift, max, i;

    old_handler = XSetErrorHandler(Ignore);

    switch (visualInfo->class) {
      case DirectColor:
        max = (size > visualInfo->colormap_size) ? visualInfo->colormap_size
                                                 : size;
        for (i = 0; i < max; i++) {
            rShift = ffs((unsigned int)visualInfo->red_mask) - 1;
            gShift = ffs((unsigned int)visualInfo->green_mask) - 1;
            bShift = ffs((unsigned int)visualInfo->blue_mask) - 1;
            c.pixel = ((i << rShift) & visualInfo->red_mask) |
                      ((i << gShift) & visualInfo->green_mask) |
                      ((i << bShift) & visualInfo->blue_mask);
            c.red = (unsigned short)(rgb[i] * 65535.0 + 0.5);
            c.green = (unsigned short)(rgb[size+i] *
                                       65535.0 + 0.5);
            c.blue = (unsigned short)(rgb[size*2+i] *
                                      65535.0 + 0.5);
            c.flags = DoRed | DoGreen | DoBlue;
            XStoreColor(display, colorMap, &c);
        }
        break;
      case GrayScale:
      case PseudoColor:
        max = (size > visualInfo->colormap_size) ? visualInfo->colormap_size
                                                 : size;
        for (i = 0; i < max; i++) {
            c.pixel = i;
            c.red = (unsigned short)(rgb[i] * 65535.0 + 0.5);
            c.green = (unsigned short)(rgb[size+i] *
                                       65535.0 + 0.5);
            c.blue = (unsigned short)(rgb[size*2+i] *
                                      65535.0 + 0.5);
            c.flags = DoRed | DoGreen | DoBlue;
            XStoreColor(display, colorMap, &c);
        }
        break;
    }

    XSync(display, 0);
    XSetErrorHandler(old_handler);
}

/******************************************************************************/

void tkSwapBuffers(void)
{

    if (display) {
        glXSwapBuffers(display, window);
    }
}

/******************************************************************************/





#define X11

#define MAX_FONTS 1000
static GLuint ListBase[MAX_FONTS];
static GLuint ListCount[MAX_FONTS];



/*
 * Load the named bitmap font as a sequence of bitmaps in a display list.
 * fontname may be one of the predefined fonts like TOGL_BITMAP_8_BY_13
 * or an X font name, or a Windows font name, etc.
 */
GLuint tkLoadBitmapFont( const char *name )
{
   static int FirstTime = 1;

   XFontStruct *fontinfo;

   int first, last, count;
   GLuint fontbase;

   /* Initialize the ListBase and ListCount arrays */
   if (FirstTime) {
      int i;
      for (i=0;i<MAX_FONTS;i++) {
         ListBase[i] = ListCount[i] = 0;
      }
      FirstTime = 0;
   }

   /*
    * This method of selecting X fonts according to a TOGL_ font name
    * is a kludge.  To be fixed when I find time...
    */
   /*
   if (fontname==TOGL_BITMAP_8_BY_13) {
      name = "8x13";
   }
   else if (fontname==TOGL_BITMAP_9_BY_15) {
      name = "9x15";
   }
   else if (fontname==TOGL_BITMAP_TIMES_ROMAN_10) {
      name = "-adobe-times-medium-r-normal--10-100-75-75-p-54-iso8859-1";
   }
   else if (fontname==TOGL_BITMAP_TIMES_ROMAN_24) {
      name = "-adobe-times-medium-r-normal--24-240-75-75-p-124-iso8859-1";
   }
   else if (fontname==TOGL_BITMAP_HELVETICA_10) {
      name = "-adobe-helvetica-medium-r-normal--10-100-75-75-p-57-iso8859-1";
   }
   else if (fontname==TOGL_BITMAP_HELVETICA_12) {
      name = "-adobe-helvetica-medium-r-normal--12-120-75-75-p-67-iso8859-1";
   }
   else if (fontname==TOGL_BITMAP_HELVETICA_18) {
      name = "-adobe-helvetica-medium-r-normal--18-180-75-75-p-98-iso8859-1";
   }
   else if (!fontname) {
      name = DEFAULT_FONTNAME;
   }
   else {
      name = (const char *) fontname;
   }

   assert( name );
   */


   fontinfo = XLoadQueryFont( display, name );
   if (!fontinfo) {
      return 0;
   }

   first = fontinfo->min_char_or_byte2;
   last = fontinfo->max_char_or_byte2;

   count = last-first+1;

   fontbase = glGenLists( (GLuint) (last+1) );
   if (fontbase==0) {
      return 0;
   }

   glXUseXFont( fontinfo->fid, first, count, (int) fontbase+first );


   /* Record the list base and number of display lists
    * for Togl_UnloadBitmapFont().
    */
   {
      int i;
      for (i=0;i<MAX_FONTS;i++) {
         if (ListBase[i]==0) {
            ListBase[i] = fontbase;
            ListCount[i] = last+1;
            break;
         }
      }
   }

   return fontbase;
}



/*
 * Release the display lists which were generated by Togl_LoadBitmapFont().
 */
void tkUnloadBitmapFont( GLuint fontbase )
{
   int i;
   for (i=0;i<MAX_FONTS;i++) {
      if (ListBase[i]==fontbase) {
         glDeleteLists( ListBase[i], ListCount[i] );
         ListBase[i] = ListCount[i] = 0;
         return;
      }
   }
}
