#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <iostream>
#include <algorithm>
#include <vector>
#include <utility>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <cstring>
#include <sstream>

#define COLOR_NUM 15

using namespace std;
int x,y,n;
int RGB[COLOR_NUM][3];
int counts[COLOR_NUM];

int closestTo(int red, int green, int blue, unsigned char topColors[]){
    double minDiff = 10000000;
    int index = 0;
    for(int i = 0; i < COLOR_NUM; i ++){
        double currDiff = (red-(int)topColors[i*n+0])*(red-(int)topColors[i*n+0]) +
                          (green-(int)topColors[i*n+1])*(green-(int)topColors[i*n+1])+
                          (blue-(int)topColors[i*n+2])*(blue-(int)topColors[i*n+2]);
        if(currDiff < minDiff){
            minDiff = currDiff;
            index = i;
        }
    }
    return index;
}

void howCloseTo(unsigned char* data, unsigned char* topColors){
    memset(RGB,0,sizeof(RGB));
    memset(counts,0,sizeof(counts));
    for(int i = 0;i < x*y*n;i += n) {
        int toWhichColor = closestTo((int)data[i+0],(int)data[i+1],(int)data[i+2],topColors);
        RGB[toWhichColor][0] += (int) data[i+0];
        RGB[toWhichColor][1] += (int) data[i+1];
        RGB[toWhichColor][2] += (int) data[i+2];
        counts[toWhichColor] ++;
    }
}

int main(int argc, char *argv[]){
    srand (time(NULL));
    unsigned char *data = stbi_load("home.jpg", &x, &y, &n, 3);
    n = 3;
    unsigned char *convertedData;
    cout << "Loaded " << x << ' ' << y << ' ' << n << std::endl;
    unsigned char topColors[COLOR_NUM*3];

    cout << "CHOSEN INITIAL COLORS:" << endl;
    for(int i = 0; i < COLOR_NUM; i ++){
        int randIndexX = rand()%x, randIndexY = rand()%y;
        topColors[i + 0] = data[randIndexY*x*n+randIndexX*n+0];
        topColors[i + 1] = data[randIndexY*x*n+randIndexX*n+1];
        topColors[i + 2] = data[randIndexY*x*n+randIndexX*n+2];
        cout << (int)topColors[i + 0] << " " << (int)topColors[i + 1] << " " << (int)topColors[i + 2] << endl;
    }

    int count = 0;

    while(count < 500){
        howCloseTo(data, topColors);

        ///recalculate colours by finding average
        bool flag = false;
        for(int i = 0; i < COLOR_NUM; i ++){

            if(counts[i] == 0){
                continue;
            }
            if(abs((int)(RGB[i][0]/counts[i]) - (int)topColors[i*n+0]) > 2 ||
               abs((int)(RGB[i][1]/counts[i]) - (int)topColors[i*n+1]) > 2||
               abs((int)(RGB[i][2]/counts[i]) - (int)topColors[i*n+2]) > 2){
                ///if the clusters did change
                flag = true;
            }
            topColors[i*n+0] = (char)(RGB[i][0]/counts[i]);
            topColors[i*n+1] = (char)(RGB[i][1]/counts[i]);
            topColors[i*n+2] = (char)(RGB[i][2]/counts[i]);

        }
        if(!flag){
            //cout << "Stopping " << count <<  endl;
            break;
        }
        count ++;
    }

    unsigned char *result = new unsigned char[x*y*n + 1];
    for(int i = 0;i < x*y*n;i += n) {
        int toWhichColor = closestTo((int)data[i+0],(int)data[i+1],(int)data[i+2],topColors);
        result[i + 0] = topColors[toWhichColor*n + 0];
        result[i + 1] = topColors[toWhichColor*n + 1];
        result[i + 2] = topColors[toWhichColor*n + 2];
    }

    stbi_write_png("home15.jpg", x, y, n, result, n*x);

return 0;

}
