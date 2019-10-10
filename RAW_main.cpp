#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <math.h>
using namespace std;

enum colors
{
    RED,
    GREEN1,
    GREEN2,
    BLUE
};

int row = 3072, col = 4096;

int main()
{
	// 16 bit version channels
    // vector<uint16_t> channels[4];
	uint16_t r, g, g2, b;
	
    ifstream data("portrait-gainx2-offset2047-20ms-02.raw12", ios::binary);
	unsigned char buffer[3];

    //8 bit version channels
    static vector<unsigned char> channels[4];

    memset(buffer, '\0', sizeof(buffer));

    int iter = 0;
    int columns_read = 0;
    while (data.read((char *)buffer, sizeof(buffer)))
    {
    	if(columns_read==col) iter++,columns_read=0;
        if(iter>=row) break;
    	columns_read+=2;

    	if(iter&1)
        {

        	g2 = (uint16_t(buffer[0])<<4 | ((uint16_t(buffer[1]) >> 4) >> 4));
            channels[GREEN2].push_back(char(g2/16));

            b = ((uint16_t(buffer[1] & 0x0F) << 8) | uint8_t(buffer[2]));
            channels[BLUE].push_back(char(b/16));
        }
        else{

        	r = (uint16_t(buffer[0])<<4 | ((uint16_t(buffer[1]) >> 4) >> 4));
            channels[RED].push_back(char(r/16));

            g = ((uint16_t(buffer[1] & 0x0F) << 8) | uint8_t(buffer[2]));
            channels[GREEN1].push_back(char(g/16));
        }
    }
    ofstream redimage,green1image,green2image,blueimage,image;

    redimage.open("redImage.ppm", ios::binary);
    green1image.open("green1Image.ppm",ios::binary);
    green2image.open("green2Image.ppm",ios::binary);
    blueimage.open("blueImage.ppm",ios::binary);
    image.open("image.ppm",ios::binary);

   	redimage << "P6\n2048 1536\n255\n";
    green1image << "P6\n2048 1536\n255\n";
    green2image << "P6\n2048 1536\n255\n";
	blueimage << "P6\n2048 1536\n255\n";
	image <<"P6\n4096 3072\n255\n";

	//all channels have same size
	//creating image for each channel
	int size = channels[RED].size();
   	for(int i=0;i<size;i++){
   		redimage << channels[RED][i]<<0<<0;
   		green1image << 0<<channels[GREEN1][i]<<0;
   		green2image<<0<<channels[GREEN2][i]<<0;
   		blueimage<<0<<0<<channels[BLUE][i];
   	}
   
   	//Linear interpolation
   	vector<char> result(3*row*col);
    for (unsigned int i = 0; i < row; i++) {
        for (unsigned int j = 0; j < col; j++) {
            int imOff = 3 * (i * col + j);
            int grOff = (i / 2) * col / 2 + (j / 2);
            result[imOff + 0] = channels[RED][grOff];
            result[imOff + 1] = (channels[GREEN1][grOff] + channels[GREEN2][grOff]) / 2;
            result[imOff + 2] = channels[BLUE][grOff];
            image<<result[imOff + 0]<<result[imOff+1]<<result[imOff+2];
        }
	}
	//close files
	image.close();
	redimage.close();
	green1image.close();
	green2image.close();
	blueimage.close();
   	data.close();


}
