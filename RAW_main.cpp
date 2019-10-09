#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
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
    vector<unsigned char> channels[4];
    // unsigned char red,green1,green2,blue;

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
//
            // green2 = (uint8_t(buffer[0]));
            // channels[GREEN2].push_back(green2);
            // green2 = ((uint8_t(buffer[1]) >> 4) & 0x0F);
            // channels[GREEN2].push_back(green2);

            // blue = ((uint8_t(buffer[1]  & 0x0F ) << 4));
            // channels[BLUE].push_back(blue);
            // blue =  uint8_t(buffer[2]);
            // channels[BLUE].push_back(blue);
        }
        else{

        	r = (uint16_t(buffer[0])<<4 | ((uint16_t(buffer[1]) >> 4) >> 4));
            channels[RED].push_back(char(r/16));
            g = ((uint16_t(buffer[1] & 0x0F) << 8) | uint8_t(buffer[2]));
            channels[GREEN1].push_back(char(g/16));

            // red = (uint8_t(buffer[0]));
            // channels[RED].push_back(red);
            // red =  (uint8_t(buffer[1]) >> 4) & 0x0F;
            // channels[RED].push_back(red);

            // green1 = (uint8_t(buffer[1] & 0x0F) << 4);
            // channels[GREEN1].push_back(green1);
            // green1 =  uint8_t(buffer[2]);
            // channels[GREEN1].push_back(green1);
        }
    }

    cout << channels[RED].size() << endl
         << channels[GREEN1].size() << endl
         << channels[GREEN2].size() << endl
         << channels[BLUE].size() << endl;

    ofstream redimage,green1image,green2image,blueimage;
    redimage.open("redImage.ppm", ios::binary);
    green1image.open("green1Image.ppm",ios::binary);
    green2image.open("green2Image.ppm",ios::binary);
    blueimage.open("blueImage.ppm",ios::binary);

   	redimage << "P6\n2048 1536\n255\n";
    green1image << "P6\n2048 1536\n255\n";
    green2image << "P6\n2048 1536\n255\n";
	blueimage << "P6\n2048 1536\n255\n";

	//all channels have same size
	int size = channels[RED].size();
   	for(int i=0;i<size;i++){
   		redimage << channels[RED][i]<<0<<0;
   		green1image << 0<<channels[GREEN1][i]<<0;
   		green2image << 0<<channels[GREEN2][i]<<0;
   		blueimage << 0<<0<<channels[BLUE][i];
   	}
	
	//close files
	redimage.close();
	green1image.close();
	green2image.close();
	blueimage.close();
   	data.close();


}
