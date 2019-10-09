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

int main()
{
    vector<uint16_t> channels[4];
    ifstream data("portrait-gainx2-offset2047-20ms-02.raw12", ios::binary);
    unsigned char buffer[3];
    uint16_t r, g, g2, b;

    vector<unsigned char> red,green1,green2,blue;

    memset(buffer, '\0', sizeof(buffer));

    int i = 0;
    while (data.read((char *)buffer, sizeof(buffer)))
    {
        if (i < 2048)
        {
            r = (uint8_t(buffer[0]) | ((uint8_t(buffer[1]) >> 4) & 0x0F));
            channels[RED].push_back(r);
            g = ((uint8_t(buffer[1] & 0x0F) << 4) | uint8_t(buffer[2]));
            channels[GREEN1].push_back(g);
            i += 2;
        }
        else if (i >= 2048)
        {
            g2 = (uint8_t(buffer[0]) | ((uint8_t(buffer[1]) >> 4) & 0x0F));
            channels[GREEN2].push_back(r);
            b = ((uint8_t(buffer[1]  & 0x0F ) << 4) | uint8_t(buffer[2]));
            channels[BLUE].push_back(g);
            i += 2;
            if (i == 4096)
                i = 0;
        }
    }
    for (int i = 0; i < 85; i++)
    {
        channels[RED].pop_back();
        channels[GREEN1].pop_back();
    }

//     cout << channels[RED].size() << endl
//          << channels[GREEN1].size() << endl
//          << channels[GREEN2].size() << endl
//          << channels[BLUE].size() << endl;

}
