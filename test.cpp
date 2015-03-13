#include "Display.h"
#include <string.h>
#include <iostream>
#include <vector>
#include <math.h>

//Brief description
/*
To calculate all the pixels I chose bilinear algorithm. This is a very general solution which works fine
for all cases. If you had only 2 colors linear algorithms would have better performance but this is more 
about correct behavior.
*/

//Not solved Issues 
/*
The Code right now only works for color difference of about 16 . So tl = 0 tr = 16 or tl = 0 tr = 8 works.
But tl = 100 tr = 200 doesn't work. I couldn't figure out where to scale so far. 

*/

using namespace std;

//Converts a color into RGB values
void color2rgb(unsigned short color,unsigned short rgb[3]) 
{ 
	unsigned short bitmaskRed = 0xF800, bitmaskGreen = 0x7E0,bitmaskBlue = 0x1F;
	rgb[0]= (color & bitmaskRed) >>11 ; 
	rgb[1]= (color & bitmaskGreen) >>5; 
	rgb[2]= color & bitmaskBlue; 
} 

//Converts RGB into color value
unsigned int rgb2color(unsigned short rgb[3]) 
{ 
   unsigned short color = 0;
   color = (rgb[0]<< 11) + (rgb[1] << 5) + rgb[2];
   return color ;
} 

//Rounds a double value up or down 
int round(double number)
{
	double x = ceil(number);
	if((x-number)<0.5)
	{
		number = x;

	}
	else
	{
		number = (int)number;
	}

	return number;
}

//Calculates bilinear interpolation 
void bilinearInterpolation(int height,int width, unsigned short rgb[4][3],unsigned short* pixels)
{
	double x1=0,x2= width-1;
	double y1=height-1,y2=0;
	double val;
	unsigned short newrgb[3];
	unsigned short color = 0 ;
     
	// loop columns 
	for(int x =0;x<width;x++) 
	{ 
		// loop rows
		for(int y =0;y<height;y++)
		{  
			// calculate new color using bilinear interpolation algorithmus 
			for(int i =0;i<3;i++) 
			{ 
				 val =
				 (1.0/ ((x2 - x1)*(y2-y1)) ) * 
				 (rgb[2][i] * (x2-x )*(y2-y) + 
				 rgb[3][i]* (x-x1)* (y2-y)  + 
				 rgb[0][i] *(x2-x) *(y-y1)+
				 rgb[1][i]* (x-x1)*(y-y1));
				
				//Round if appropriate
				if(val != (int)val)
				{
					newrgb[i] = round(val);
				}
				else
				{
					newrgb[i] = val;
				}
			} 
			
			//set pixel color
			color = rgb2color(newrgb);
			pixels[x+ y*width]  = color;

		}
	}
}

int main( int argc, char **argv )
{	
	argv[1] ="display";
	argv[2] ="100";  // ab Differenz 28 Fehler sowohl horizontal als auch vertikal
	argv[3] ="110";
	argv[4] ="95";
	argv[5] ="115";
    argc = 6;
	Display display;
	unsigned short arrColors[4] = {0};
	unsigned short rgb[4][3];
	unsigned short newrgb[3];
	int width,height;

	//set display
	display.connect(argv[1]);
	display.get_size(width,height);
	
	//Too few parameters
	if(argc < 4)
	{
		cout<<"Please enter at least 2 colours";
		return 0;
	}

	//Read parameters and convert to integer
	for(int i = 2; i < argc ;i++)
	{
		  int color = atoi(argv[i]);
		  if(color != 0 )
		  {
			arrColors[i-2] = color;
		  }
	}

	//Add missing parameters
	if(argc == 4 || argc == 5)
	{
		if(argc == 4)
		{
			arrColors[2] = arrColors[0];
			arrColors[3] = arrColors[1];
		}
		else
		{
			arrColors[3] = arrColors[1];
		}
	}
     
	unsigned short* pixels = new unsigned short[width * height]; //16 * 9

	//Convert colors to RGB
	for(int i = 0 ; i<4 ;i++)
	{
		color2rgb(arrColors[i],rgb[i]);
	}
	
	bilinearInterpolation(height,width,rgb,pixels);

	//write pixels into frameBuffer
	for(int i =0;i<height;i++)
	{
		display.draw_raster(0,i,&pixels[i*width],width);
	}

	delete [] pixels;
}

