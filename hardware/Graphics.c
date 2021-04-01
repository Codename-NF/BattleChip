#include "Graphics.h"

/*******************************************************************************************
* This function writes a single pixel to the x,y coords specified using the specified colour
* Note colour is a byte and represents a palette number (0-255) not a 24 bit RGB value
********************************************************************************************/
void WriteAPixel(int x, int y, int Colour)
{
	WAIT_FOR_GRAPHICS;				// is graphics ready for new command

	GraphicsX1Reg = x;				// write coords to x1, y1
	GraphicsY1Reg = y;
	GraphicsColourReg = Colour;			// set pixel colour
	GraphicsCommandReg = PutAPixel;			// give graphics "write pixel" command
}

void squaremapper(int x, int y, int player, int colour)
{
    WAIT_FOR_GRAPHICS;              // is graphics ready for new command
    GraphicsX1Reg = x;              // write coords to x1, y1
    GraphicsY1Reg = y;
    GraphicsX2Reg = player - 1;
    GraphicsColourReg = colour;
    GraphicsCommandReg = Squaremapper; 
}

void squaremappership(int player, int x, int y, int length, int dir, int done, int colour)
{
    //dir 1 is down, dir 2 is horizontal
    //left player is 1, right player is 2
    WAIT_FOR_GRAPHICS;              // is graphics ready for new command
    GraphicsX1Reg = x;              // write coords to x1, y1
    GraphicsY1Reg = y;
    GraphicsX2Reg = player - 1;
    GraphicsColourReg = colour;
    GraphicsCommandReg = CrossBox;
    int i = 1;
    for (i = 1; i < length; i++) {
        if (dir == VERTICAL) {
            WAIT_FOR_GRAPHICS;
            GraphicsY1Reg = y + i;
            GraphicsCommandReg = CrossBox;
        } else {
            WAIT_FOR_GRAPHICS;
            GraphicsX1Reg = x + i;
            GraphicsCommandReg = CrossBox;
        }
    }

}

void outgraphicschar(int x, int y, int colour, int backgroundcolour, int c, int Erase) {
    register int    row,
                    column,
                    theX = x,
                    theY = y ;
    register int    pixels ;
    register char   theColour = colour  ;
    register int    BitMask,
                    theCharacter = c,
                    j,
                    theRow, theColumn;


    if(((short)(x) > (short)(XRES-1)) || ((short)(y) > (short)(YRES-1)))  // if start off edge of screen don't bother
        return ;

    if(((short)(theCharacter) >= (short)(' ')) && ((short)(theCharacter) <= (short)('~'))) {            // if printable character
        theCharacter -= 0x20 ;                                                                          // subtract hex 20 to get index of first printable character (the space character)
        theRow = FONT2_YPIXELS;
        theColumn = FONT2_XPIXELS;

        for(row = 0; row < theRow ; row ++) {
            pixels = Font10x14[theCharacter][row] ;                                         // get the pixels for row 0 of the character to be displayed
            BitMask = 512 ;                                                                     // set of hex 200 i.e. bit 7-0 = 0010 0000 0000
            for(column = 0; column < theColumn;   )     {
                if((pixels & BitMask))                                                      // if valid pixel, then write it
                    WriteAPixel(theX+column, theY+row, theColour) ;
                else {                                                                      // if not a valid pixel, do we erase or leave it along (no erase)
                    if(Erase == 1)
                        WriteAPixel(theX+column, theY+row, backgroundcolour) ;
                    // else leave it alone
                }
                    column ++ ;
                BitMask = BitMask >> 1 ;
            }
        }
    }

}

void winnermessage(int winner, int colour, int background) {
    int current_x = 0;
    int current_y = YBASE - (2 * FONT2_YPIXELS);
    int i = 0;

    if (winner == 1) {
        current_x = XBASE1 + 2 * (SQUARESIZE + SPACESIZE);
        while (P1WINS[i] != '\0') {
            outgraphicschar(current_x, current_y, colour, background, P1WINS[i], 0);
            i++;
            current_x += FONT2_XPIXELS;
        }
    } else {
        current_x = XBASE2 + 2 * (SQUARESIZE + SPACESIZE);
        while (P2WINS[i] != '\0') {
            outgraphicschar(current_x, current_y, colour, background, P2WINS[i], 0);
            i++;
            current_x += FONT2_XPIXELS;
        }
    }
}

void displaywinner(int winner) {
    winnermessage(winner, LIME, BLACK);
}

void writecoords() {
    int current_x = 0;
    int current_y = 0;
    int i = 0;
    int colour = LIME;
    int background = BLACK;

    //LEFT BOARD
    current_x = XBASE1 + 5;
    current_y = YBASE;
    while (COLCOOR[i] != '\0') {
        outgraphicschar(current_x, current_y, colour, background, COLCOOR[i], 0);
        i++;
        current_x += (SQUARESIZE + SPACESIZE);
    }
    current_x = XBASE1 - SQUARESIZE;
    current_y = YBASE + SQUARESIZE + 3;
    i = 0;
    while (ROWCOOR[i] != '\0') {
        outgraphicschar(current_x, current_y, colour, background, ROWCOOR[i], 0);
        i++;
        current_y += (SQUARESIZE + SPACESIZE);
    }
    outgraphicschar(current_x - FONT2_XPIXELS, current_y, colour, background, '1', 0);
    outgraphicschar(current_x, current_y, colour, background, '0', 0);

    //RIGHT BOARD
    i = 0;
    current_x = XBASE2 + 5;
    current_y = YBASE;
    while (COLCOOR[i] != '\0') {
        outgraphicschar(current_x, current_y, colour, background, COLCOOR[i], 0);
        i++;
        current_x += (SQUARESIZE + SPACESIZE);
    }
    current_x = XBASE2 - SQUARESIZE;
    current_y = YBASE + SQUARESIZE + 3;
    i = 0;
    while (ROWCOOR[i] != '\0') {
        outgraphicschar(current_x, current_y, colour, background, ROWCOOR[i], 0);
        i++;
        current_y += (SQUARESIZE + SPACESIZE);
    }
    outgraphicschar(current_x - FONT2_XPIXELS, current_y, colour, background, '1', 0);
    outgraphicschar(current_x, current_y, colour, background, '0', 0);


}

void blankscreen(int Colour)
{
    WAIT_FOR_GRAPHICS;              // is graphics ready for new command

    GraphicsX1Reg = 0;              // write coords to x1, y1
    GraphicsY1Reg = 0;
    GraphicsColourReg = Colour;
    GraphicsCommandReg = Blankboard;
    writecoords();
    winnermessage(1, BLACK, BLACK);
    winnermessage(2, BLACK, BLACK); 
}

/*********************************************************************************************
* This function read a single pixel from the x,y coords specified and returns its colour
* Note returned colour is a byte and represents a palette number (0-255) not a 24 bit RGB value
*********************************************************************************************/

int ReadAPixel(int x, int y)
{
	WAIT_FOR_GRAPHICS;			// is graphics ready for new command

	GraphicsX1Reg = x;			// write coords to x1, y1
	GraphicsY1Reg = y;
	GraphicsCommandReg = GetAPixel;		// give graphics a "get pixel" command

	WAIT_FOR_GRAPHICS;			// is graphics done reading pixel
	return (int)(GraphicsColourReg) ;	// return the palette number (colour)
}


/**********************************************************************************
** subroutine to program a hardware (graphics chip) palette number with an RGB value
** e.g. ProgramPalette(RED, 0x00FF0000) ;
**
************************************************************************************/

void ProgramPalette(int PaletteNumber, int RGB)
{
    WAIT_FOR_GRAPHICS;
    GraphicsColourReg = PaletteNumber;
    GraphicsX1Reg = RGB >> 16   ;        // program red value in ls.8 bit of X1 reg
    GraphicsY1Reg = RGB ;                // program green and blue into ls 16 bit of Y1 reg
    GraphicsCommandReg = ProgramPaletteColour; // issue command
}

/********************************************************************************************* This function draw a horizontal line, 1 pixel at a time starting at the x,y coords specified
*********************************************************************************************/

void HLine(int x1, int y1, int length, int Colour)
{
	int i;

	for(i = x1; i < x1+length; i++ )
		WriteAPixel(i, y1, Colour);
}

/********************************************************************************************* This function draw a vertical line, 1 pixel at a time starting at the x,y coords specified
*********************************************************************************************/

void VLine(int x1, int y1, int length, int Colour)
{
	int i;

	for(i = y1; i < y1+length; i++ )
		WriteAPixel(x1, i, Colour);
}

/*******************************************************************************
** Implementation of Bresenhams line drawing algorithm
*******************************************************************************/
int abs(int a)
{
    if(a < 0)
        return -a ;
    else
        return a ;
}

int sign(int a)
{
    if(a < 0)
        return -1 ;
    else if (a == 0)
        return 0 ;
    else
        return 1 ;
}



void Line(int x1, int y1, int x2, int y2, int Colour)
{
    int x = x1;
    int y = y1;
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);

    int s1 = sign(x2 - x1);
    int s2 = sign(y2 - y1);
    int i, temp, interchange = 0, error ;

// if x1=x2 and y1=y2 then it is a line of zero length so we are done

    if(dx == 0 && dy == 0)
        return ;

 // must be a complex line so use Bresenhams algorithm
    else    {

// swap delta x and delta y depending upon slop of line

        if(dy > dx) {
            temp = dx ;
            dx = dy ;
            dy = temp ;
            interchange = 1 ;
        }

// initialise the error term to compensate for non-zero intercept

        error = (dy << 1) - dx ;    // error = (2 * dy) - dx

// main loop
        for(i = 1; i <= dx; i++)    {
            WriteAPixel(x, y, Colour);

            while(error >= 0)   {
                if(interchange == 1)
                    x += s1 ;
                else
                    y += s2 ;

                error -= (dx << 1) ;    // error = error - (dx * 2)
            }

            if(interchange == 1)
                y += s2 ;
            else
                x += s1 ;

            error += (dy << 1) ;    // error = error + (dy * 2)

        }
    }
}
/*
int main(void)
{
    int i;
    printf("Starting...\n");
    blankscreen( BLUE );
    squaremapper(0,0,0,RED); // RED - HIT
    squaremapper(0,0,1,WHITE); // WHITE - MISS
    squaremapper(1,1,0,MAGENTA); // MAGENTA - SUNK 
    squaremapper(1,1,1,RED);
    printf("Done...\n");
    return 0 ;
}
*/
