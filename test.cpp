#include "Display.h"
#include <string.h>
#include <iostream>
#include <vector>

using namespace std;

void color2rgb(unsigned short color,unsigned short rgb[3]) 
{ 
	unsigned short bitmaskRed = 0xF800, bitmaskGreen = 0x7E0,bitmaskBlue = 0x1F;
	rgb[0]= (color & bitmaskRed) >>11 ; 
	rgb[1]= (color & bitmaskGreen) >>5; 
	rgb[2]= color & bitmaskBlue; 
} 

unsigned int rgb2color(unsigned short rgb[3]) 
{ 
   unsigned short color = 0;
   color = (rgb[0]<< 11) + (rgb[1] << 5) + rgb[2];
   return color ;
} 

int main( int argc, char **argv )
{
	argv[1] = "display";
	argv[2] = "0";
	argv[3] = "2";

	argc = 4;
		
	Display display;
	unsigned short arrColors[4] = {0};
	int width,height;
	unsigned short* pixels;	
	
	//Too few parameters
	if(argc < 4)
	{
		cout<<"Please enter at least 2 colours";
		return 0;
	}

	//read parameters
	for(int i = 2; i < argc ;i++)
	{
		  int color = atoi(argv[i]);
		  if(color != 0 )
		  {
			arrColors[i-2] = color;
		  }
	}

	if(argc == 4)
	{
		arrColors[2] = arrColors[0];
		arrColors[3] = arrColors[1];
	}

	if(argc == 5)
	{
		arrColors[3] = arrColors[1];
	}
     
	//set display
	display.connect(argv[1]);
	display.get_size(width,height);

	pixels = new unsigned short[width * height]; //16 * 9

	//convert colors to RGB
	unsigned short rgb[4][3];
	unsigned short newrgb[3];
	unsigned short color = 0 ;

	for(int i = 0 ; i<4 ;i++)
	{
		color2rgb(arrColors[i],rgb[i]);
	}

	color2rgb (arrColors[0],newrgb);  //tl start value

	for(int x =0;x<width;x++) // loop columns 
	{ 
		for(int y =0;y<height;y++) // loop rows
		{  
			//set pixel color
			color = rgb2color(newrgb);
			pixels[x+ y*width]  = color;
			// calculate new color  
			for(int i =0;i<=2;i++) 
			{ 
				newrgb[i]= 
				rgb[0][i]*((width-x)*(height-y)/(width*height)) + 
				rgb[1][i]*(x *(height-y)/(width*height)) + 
				rgb[2][i]*((width-x)*y     /(width*height)) + 
				rgb[3][i]*(x     *y     /(width*height)); 
			} 
		}
	}

	//write pixels into frameBuffer
	for(int i =0;i<height;i++)
	{
		display.draw_raster(0,i,&pixels[i*width],width);
	}

	////Unit Test Examples

}
