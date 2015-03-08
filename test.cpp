#include "Display.h"
#include <string.h>
#include <iostream>
#include <vector>
#include <math.h>

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


int main( int argc, char **argv )
{
	//Order of parameters ramp.exe display tl tr [bl] [br]
	argv[1] = "display";
	argv[2] = "90";
	argv[3] = "90";
	argv[4] = "10";
	argv[5] = "10";

	argc = 6;
		
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

	double x1=0,x2= width-1;
	double y1=0,y2=height-1;

	for(int x =0;x<width;x++) // loop columns 
	{ 
		for(int y =0;y<height;y++) // loop rows
		{  
			// calculate new color using bilinear interpolation algorithmus 
			for(int i =0;i<3;i++) 
			{ 
				double val; //rgb is only short
				val =
				(1.0/ ((x2 - x1)*(y2-y1)) ) * 
				(rgb[2][i] * (x2-x )*(y2-y) + 
				 rgb[3][i]* (x-x1)* (y2-y)  + 
				 rgb[0][i] *(x2-x) *(y-y1)+
				 rgb[1][i]* (x-x1)*(y-y1));

				if(val != (int)val)
				{
					newrgb[i] = round(val);
				}
				else
				{
					newrgb[i] = val;
				}

				//set pixel color
				color = rgb2color(newrgb);
				pixels[x+ y*width]  = color;
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
