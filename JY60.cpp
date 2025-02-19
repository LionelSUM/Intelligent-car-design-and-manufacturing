#include "JY60.h"

UnbufferedSerial device(A4,A5);  // tx, rx
//BufferedSerial pc(USBTX,USBRX);
unsigned char Re_buf[11],counter=0;
unsigned char sign=0;
float yaw1 = 0;
float yaw2 = 0;
int GetData1=0;
int GetData2=0;
float DeltaAngle=0;
//float TurnAngle;
float FinishFlag=0;

void StartdeviceEvent() {
    //Re_buf[counter]=(unsigned char)device.getc();
    (unsigned char)device.read(&Re_buf[counter],sizeof(Re_buf[counter]));
    //if(counter==0&&Re_buf[0]!=0x55) return;       //第0号数据不是帧头      
    if(counter!=0||Re_buf[0]==0x55){       
    counter++;     
    if(counter==11)             //接收到11个数据
    {    
       counter=0;               //重新赋值，准备下一帧数据的接收 
       sign=1;
    }
    //当接收到完整数据帧时开始进行姿态解算
    if(sign)
   {  
    sign=0;
    if(Re_buf[0]==0x55)      //检查帧头
    { 
    static float lastYaw = 0;
    static float turnNum = 0;
    if(Re_buf [1]==0x53)
    {
        yaw1=0;
        double yaw_;
        yaw_ = Re_buf[7] << 8 | Re_buf[6];
        yaw_ = yaw_ / 32768.0 * 180;
        if(lastYaw < 90 && yaw_ > 270){turnNum -= 1;}
        else if(lastYaw > 270 && yaw_ < 90){turnNum += 1;}
        lastYaw = yaw_;
        yaw1 = yaw_ + 360*turnNum;
        GetData1=1;
        //pc.printf(" Start angle: %8.2f\n", yaw_+360*turnNum);
    }
    else GetData1=0;
    }
  } 

}

}

void EnddeviceEvent() {
    //Re_buf[counter]=(unsigned char)device.getc();
    (unsigned char)device.read(&Re_buf[counter],sizeof(Re_buf[counter]));
    //if(counter==0&&Re_buf[0]!=0x55) return;       //第0号数据不是帧头      
    if(counter!=0||Re_buf[0]==0x55){       
    counter++;     
    if(counter==11)             //接收到11个数据
    {    
       counter=0;               //重新赋值，准备下一帧数据的接收 
       sign=1;
    }
    //当接收到完整数据帧时开始进行姿态解算
    if(sign)
   {  
    sign=0;
    if(Re_buf[0]==0x55)      //检查帧头
    { 
    static float lastYaw = 0;
    static float turnNum = 0;
    if(Re_buf [1]==0x53)
    {
        yaw2=0;
        double yaw_;
        yaw_ = Re_buf[7] << 8 | Re_buf[6];
        yaw_ = yaw_ / 32768.0 * 180;
        if(lastYaw < 90 && yaw_ > 270){turnNum -= 1;}
        else if(lastYaw > 270 && yaw_ < 90){turnNum += 1;}
        lastYaw = yaw_;
        yaw2 = yaw_ + 360*turnNum;
        DeltaAngle = abs(yaw2-yaw1);
        GetData2=1;
        //pc.printf("Turn Angle: %8.2f\n", yaw_+360*turnNum);
        // if(DeltaAngle>90)
        // {
        //     FinishFlag=1;
        //     DeltaAngle=0;
        //     //pc.printf("End Angle: %8.2f\n", yaw_+360*turnNum);
        // }
        // else FinishFlag=0;
    }
    else GetData2=0;
    }
  } 

}

}

