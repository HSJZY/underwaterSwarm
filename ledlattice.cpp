#include "ledlattice.h"

ledLattice::ledLattice()
{

}

unsigned char ziji[][4]={{0xff,0xff,0xff,0xff},
                         {0xff,0xff,0xff,0xff},
                         {0xff,0xff,0xff,0xff},
                         {0xff,0xff,0xff,0xff},
                         {0xff,0xff,0xff,0xff},
                         {0xff,0xff,0xff,0xff},
                         {0xff,0xff,0xff,0xff},
                         {0xff,0xff,0xff,0xff}};
unsigned char ziji1[][4]={{0xaa,0xaa,0x55,0x55},
                         {0xaa,0xaa,0x55,0x55},
                         {0xaa,0xaa,0x55,0x55},
                         {0xaa,0xaa,0x55,0x55},
                         {0xaa,0xaa,0x55,0x55},
                         {0xaa,0xaa,0x55,0x55},
                         {0xaa,0xaa,0x55,0x55},
                         {0xaa,0xaa,0x55,0x55}};

unsigned char ziji2[][4]={{0xff,0xff,0x80,0x01},
                         {0x80,0x01,0x80,0x01},
                         {0x80,0x01,0x80,0x01},
                         {0x80,0x01,0x80,0x01},
                         {0x80,0x01,0x80,0x01},
                         {0x80,0x01,0x80,0x01},
                         {0x80,0x01,0x80,0x01},
                         {0x80,0x01,0xff,0xff}};

unsigned char wang[][4]={{0x00,0x00,0x3f,0xf8},
                         {0x01,0x00,0x01,0x00},
                         {0x3f,0xe0,0x01,0x00},
                         {0x01,0x00,0x01,0x00},
                         {0x3f,0xe0,0x00,0x00},
                         {0x00,0x00,0x00,0x00},
                         {0xaa,0xaa,0x55,0x55},
                         {0xaa,0xaa,0x55,0x55}};

unsigned char zqkl[][4]=
{
// 中(0) 秋(1) 快(2) 乐(3)
{0x00,0x00,0x00,0x00},//1
{0x00,0x00,0x00,0x00},//2
{0x00,0x00,0x00,0x00},//3
{0x00,0x00,0x00,0x00},//4
{0x00,0x00,0x00,0x00},//5
{0x00,0x00,0x00,0x00},//6
{0x00,0x00,0x00,0x00},//7
{0x00,0x00,0x00,0x00},//8
{0x00,0x86,0x00,0x88},//9
{0x20,0x90,0x60,0xA0},//0
{0x20,0x80,0x20,0x80},//1
{0x17,0xFF,0x10,0x80},//2
{0x10,0x81,0x10,0xB0},//3
{0x1C,0x88,0x03,0x84},//4
{0x00,0x02,0x00,0x00},/*"乐",0*/
{0x00,0x00,0x00,0x40},//6
{0x00,0x40,0x0F,0xC1},//7
{0x08,0x46,0x08,0x58},//8
{0x7F,0xE0,0x08,0x58},//9
{0x08,0x46,0x08,0x41},//10
{0x02,0x40,0x04,0x00},//11
{0x7F,0xFF,0x00,0x00},//12
{0x03,0x80,0x00,0x40},/*"快",1*/
{0x00,0x00,0x00,0x00},//14
{0x06,0x01,0x01,0x02},//25
{0x00,0x8C,0x00,0x30},//26
{0x7F,0xC0,0x00,0x38},//27
{0x07,0x06,0x00,0x81},//28
{0x22,0x30,0x62,0x40},//29
{0x3F,0xFF,0x12,0xC0},//30
{0x12,0x30,0x12,0x08},/*"秋",2*/
{0x00,0x00,0x00,0x00},//32
{0x00,0x00,0x07,0xF8},//33
{0x04,0x10,0x04,0x10},//34
{0x04,0x10,0x04,0x10},//35
{0x7F,0xFF,0x04,0x10},//36
{0x04,0x10,0x04,0x10},//37
{0x04,0x10,0x07,0xF8},//38
{0xFF,0xFF,0xFF,0xFF},/*"中",3*/
{0xFF,0xFF,0xFF,0xFF},//40
{0xFF,0xFF,0xFF,0xFF},//41
{0xFF,0xFF,0xFF,0xFF},//42
{0xFF,0xFF,0xFF,0xFF},//43
{0xFF,0xFF,0xFF,0xFF},//44
{0xFF,0xFF,0xFF,0xFF},//45
{0xFF,0xFF,0xFF,0xFF},//46
};
void ledLattice::showHanzi(int fd, uint8_t* hz)
{
    unsigned char logoCmd[6]=
    {
        0x25,//长度37
        0x12,//设备号，在模块复位时上报。12C18ECB
        0xC1,
        0x8E,
        0xCB,
        0x07,//命令号
    //点阵数据
    };
    sendCmd(fd,logoCmd,6);
    sendCmd(fd,hz,32);
}

void ledLattice::sendCmd(int fd,uint8_t* cmd, uint8_t len)
{
    while(len--)
    {
        serialPutchar(fd,*cmd);
        cmd++;
    }
}

int ledLattice::show()
{
     int fd;
     int i;
     delay(10);
     /*打开串口设备ttyS0，波特率9600*/
     if((fd=serialOpen("/dev/ttyAMA0",9600))<0)
     {
          int time = millis();//计时函数，从wiringPi初始化开始至今
          printf("serial doesn't open,return -1,time is %d",time);
          return 1;
     }
     showHanzi(fd,(unsigned char*)ziji2);
//   for(i=0;i<46;i++)
//   {
//      showHanzi(fd,(unsigned char*)zqkl+((46-i)*4));
//      delay(100);
//      delay(100);
//   }
     serialClose(fd);//发送完之后关闭串口，不然会出现程序运行一段时间后串口自动关闭的情况（serialOpen()返回-1）。
           // int c = waitKey(10);
         //   if ((char)c == 27) { break; } // escape
     return 0;
}
