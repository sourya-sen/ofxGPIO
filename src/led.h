#ifndef LED_H
    #define LED_H

#ifndef COMPILE_WITHOUT_OPENFRAMEWORKS
	#include "ofMain.h"
#endif
#include <iostream>
#include <vector>


class LED {
	public:
	int xnumLed;
	SPI2 spi;
	void setupAPA102(int port=0, int speed=8000000){
		int state = spi.setup(port,speed);
                if(state < 0)
		{
       		    #ifndef COMPILE_WITHOUT_OPENFRAMEWORKS
	             	ofLog()<<"Failed to setup SPI!";
		    #else
			std::cout<<"Failed to setup SPI!"<<std::endl;
		    #endif
		}
                else{
		    #ifndef COMPILE_WITHOUT_OPENFRAMEWORKS
                    	ofLog()<<"Open setup SPI!";
		    #else
			std::cout<<"Open setup SPI!"<<std::endl;
		    #endif
		}
	}

	void setAPA102(int numLed, vector<ofVec3f> colors, int BRIGHTNESS){
		int a;
                uint8_t buffer0[1], buffer1[4];
                srand(time(NULL));
		xnumLed = numLed;
		if(BRIGHTNESS>29)
			BRIGHTNESS=30;

                for(a=0; a<4; a++){
                       buffer0[0]=0x00;//0b00000000;
                      if( spi.readWrite(0, (unsigned char*)buffer0, 1) == -1)
		       	std::cout << "Error: SPI START failed!" << std::endl;
                }
                for(a=0; a<numLed; a++){
                       buffer1[0]=0b11100000 | (0b00011111 & BRIGHTNESS);//(BRIGHTNESS & 0b00011111) | 0b11100000;
                       buffer1[1]=static_cast<uint8_t>(colors[a].z);  //blue
                       buffer1[2]=static_cast<uint8_t>(colors[a].y);  //green
                       buffer1[3]=static_cast<uint8_t>(colors[a].x);  //red
                       if( spi.readWrite(0, (unsigned char*)buffer1, 4) == -1)
		       	std::cout << "Error: SPI RGB data failed!" << std::endl;
                }
                for(a=0; a<4; a++){
                       buffer0[0]=0xFF;//0b11111111;
                       if( spi.readWrite(0, (unsigned char*)buffer0, 1) == -1)
		       	std::cout << "Error: SPI STOP failed!" << std::endl;
                }

	}

	void clearAPA102(int numLed){
                int a;
		uint8_t buffer0[1], buffer1[4];
		srand(time(NULL));
		for(a=0; a<4; a++){
			buffer0[0]=0b00000000;
			spi.readWrite(0, (unsigned char*)buffer0, 1);
		}
		for(a=0; a<numLed; a++){
			buffer1[0]= 0b11100000 | (0b00011111 & 16);
			buffer1[1]=static_cast<uint8_t>(0);  //green
			buffer1[2]=static_cast<uint8_t>(0);  //blue
			buffer1[3]=static_cast<uint8_t>(0);  //red
			spi.readWrite(0, (unsigned char*)buffer1, 4);
		}
		for(a=0; a<4; a++){
			buffer0[0]=0b11111111;
			spi.readWrite(0, (unsigned char*)buffer0, 1);
		}
	}

};
#endif
