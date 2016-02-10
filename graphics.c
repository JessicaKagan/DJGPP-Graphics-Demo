#include <stdio.h>
#include <stdlib.h>
#include <dpmi.h> //DOS protected mode?
#include <go32.h>
#include <sys/farptr.h>
#include <pc.h>
#include <dos.h> //For now, just used for delays.

//Code from a tutorial on the DJGPP website.
void set_mode_13h()
{
  __dpmi_regs r;

  r.x.ax = 0x13; //19 
  __dpmi_int(0x10, &r);
}

void return_to_text_mode()
{
  __dpmi_regs r;

  r.x.ax = 3;
  __dpmi_int(0x10, &r);
}

void putpixel_13h(int x, int y, int color)
{
  _farpokeb(_dos_ds, 0xA0000+y*320+x, color);
}

void rectangle_13h(int x, int y, int w, int h, int color)
   {
        int i, j;

        _farsetsel(_dos_ds);

        for (j=y; j<y+h; j++){
            for (i=x; i<x+w; i++){
                _farnspokeb(0xA0000+j*320+i, color);
            }
        }
}

 void set_color(int color, int red, int green, int blue)
 {
    outportb(0x3C8, color);
    outportb(0x3C9, red);
    outportb(0x3C9, green);
    outportb(0x3C9, blue);
 }

//Basically a series of simple graphic effects.
//Try implementing buffering!
int main(int argc, char **argv)
{
    //It is probably best to keep desired palette information in memory somewhere.
    //For instance, you could define a palette struct based off the code here.
    //You could follow this up by defining a 'default' palette and maybe some subpalettes.
    //Not reading the registers (0x3C7) might create a performance penalty, though.
    int currentPalette[256][4];
    char defaultPalette[768];
    for(int i = 0; i < 256; ++i ){
      currentPalette[i][0] = i;
      currentPalette[i][1] = i%64;
      currentPalette[i][2] = (i + 31)%64;
      currentPalette[i][3] = (i + 63)%64;
      //printf("%d %d %d %d \n", currentPalette[i][0], currentPalette[i][1], currentPalette[i][2], currentPalette[i][3]);
    }
    //Just a test.
    
    void get_palette(char * pal){
      outportb(0x3c7, 0);
      for(int i = 0; i < 768; ++i){
      pal[i] = inportb(0x3c9); 
      }
    }

    set_mode_13h();

    get_palette(defaultPalette);
    /*
    for(int i = 0; i < 768; i+=3){
      printf("%d %d %d \n", defaultPalette[i], defaultPalette[i+1],defaultPalette[i+2]);
    } 
    */
    char str[10];
    printf("Is this not nifty?\n");
    gets(str); //I need a better "Press ENTER to continue" condition.

    

    //First we "fill" the screen in every default color.
    rectangle_13h(0, 0, 320, 200, 0);
    /*Instead of manually painting a new rectangle each time,
      we change the palette of the current one.
      This is so much faster than calling new rectangle calls
      that a delay has to be implemented in order to render the effect visible.
    */
    for(int i = 0; i < 255; ++i){
        set_color(0, defaultPalette[i*3],defaultPalette[(i*3)+1],defaultPalette[(i*3)+2]);
        delay(1000/60); //~16 milliseconds per frame. A more robust program would make this more dynamic.
    }
    set_color(0, 0, 0, 0); //Reset the color to default black after we're done.
    printf("Let's have more than one color\n on the screen now.\n");
    gets(str);   

    //Then we paint the screen pixel by pixel similarly, except we increment to the next color every 64 pixels.
    //It looks mildly interesting.
    for(int i = 0; i < 320; ++i){
      for(int j = 0; j < 200; ++j){
        int computedColor = (int) ((( (200*j) + i)%(256*64)/64));
        putpixel_13h(i, j, computedColor);
      }
    }
    printf("Color change time!\n");
    gets(str);
    //How about, for our next trick, we start messing around with the palette?
    for(int i = 0; i < 65536; ++i){
      currentPalette[i%256][1] = (currentPalette[i%256][1] + rand()% 4) % 64;
      currentPalette[i%256][2] = (currentPalette[i%256][2] + rand()% 4) % 64;
      currentPalette[i%256][3] = (currentPalette[i%256][3] + rand()% 4) % 64;
      set_color(i%256, currentPalette[i%256][1], currentPalette[i%256][2], currentPalette[i%256][3]);
    }
    //Some of this code should be functionalized for education's sake.
    printf("Let's draw more rectangles.\n");
    gets(str);
    int rectangleCoords[256][4];
    //Generate all our coordinates in one go.
    for(int i = 0; i < 256; ++i){
        rectangleCoords[i][0] = rand()% 320; 
        rectangleCoords[i][1] = rand()% 200;
        rectangleCoords[i][2] = rand()% 320; 
        rectangleCoords[i][3] = rand()% 200;
        /*
        int rectangleCoordsXSize = rand()% 320;
        int rectangleCoordsYSize = rand()% 200;
        //If the 'size' of the rectangle side means it would fall off the screen, truncate it.  
        rectangleCoords[i][2] = rectangleCoordsXSize < 320 - rectangleCoords[i][0] ? rectangleCoordsXSize : 320 - rectangleCoords[i][0];
        rectangleCoords[i][3] = rectangleCoordsYSize < 200 - rectangleCoords[i][1] ? rectangleCoordsYSize : 200 - rectangleCoords[i][1];
        */
    }
    //Then, draw the rectangles with the coords and colors we generated earlier.
    //To make things look more random, we'll occasionally flip coordinates.
    for(int i = 0; i < 256; ++i){
      switch(i%4){
        case 0:
          rectangle_13h(rectangleCoords[i][0], rectangleCoords[i][1], rectangleCoords[i][2], rectangleCoords[i][3], i);
          break;
        case 1:
          rectangle_13h(320 - rectangleCoords[i][0], rectangleCoords[i][1], 320 - rectangleCoords[i][2], rectangleCoords[i][3], i);
          break;
        case 2:
          rectangle_13h(rectangleCoords[i][0], 200 - rectangleCoords[i][1], rectangleCoords[i][2], 200 - rectangleCoords[i][3], i);
          break;
        case 3:
          rectangle_13h(320 - rectangleCoords[i][0], 200 -rectangleCoords[i][1], 320 - rectangleCoords[i][2], 200 - rectangleCoords[i][3], i);
          break;
        default: //Should not happen.
          break;
      }
    }

    printf("I'm done with these copypasted routines.\n");
    gets(str);
    return_to_text_mode();
    return 0;
}