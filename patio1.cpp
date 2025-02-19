/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */
#include "mbed.h"
#include "platform/mbed_thread.h"
#include "HCSR04.h"
#include "JY60.h"
 
BufferedSerial HC(D1,D0,115200); //Serial communication with OpenMV
 
char buf[32]={0};


DigitalOut in1(D2);
DigitalOut in2(D3);
DigitalOut in3(D4);
DigitalOut in4(D5);
PwmOut left(D6);
PwmOut right(D7);

PwmOut Claw(A0);

HCSR04 sensor1(D8, D9);//front supersonic sensor, trig, echo
HCSR04 sensor2(D10, D11);//left supersonic sensor1
HCSR04 sensor3(D12, D13);//left supersonic sensor2

float distance1,distance2,distance3;
float TurnDistance=30;
char left_speed,right_speed;
char left_label,right_label;
int num;
 
void init()
{
 
    left.write(0.0);
    right.write(0.0);
    in1=0;
    in2=0;
    in3=0;
    in4=0;
    left_speed=0;
    right_speed=0;
    left_label=2;
    right_label=2;
}
void run()
{
    left_speed=int(left_speed);
    right_speed=int(right_speed);
    left_label=int(left_label);
    right_label=int(right_label);
  if((left_label==0 || left_label==1) &&(right_label==0 ||right_label==1)){
    if (left_label==0){
        in3=0;
        in4=1;
        left_speed=left_speed-256;
        }else{
            in3=1;
            in4=0;
            }
    float left_data=float(abs(left_speed)/100.0);
    left.write(left_data);
    if (right_label==0){
        in1=1;
        in2=0;
        right_speed=right_speed-256;
        }else{
            in1=0;
            in2=1;
            }
    float right_data=float(abs(right_speed)/100.0);
    right.write(right_data);
  }else{
      left.write(0.0);
      right.write(0.0);
  }
    }
void getdata_pack()
{
    num=HC.read(buf,sizeof(buf));//read the data sent by OpenMV and store them in buf
    //assign the data to different parameters
    left_speed=buf[0];
    right_speed=buf[1];
    left_label=buf[2];
    right_label=buf[3];
}

int main()
{
    init();
    while(true){
        if(sensor2.getCm()>10&&sensor3.getCm()>10)
        {

        if(sensor1.getCm()>TurnDistance)//如果距离大于30cm,接收来自OpenMV的数据，小车巡线
        {
            // in1=1;
            // in2=0;
            // in3=0;
            // in4=1;
            // left.write(0.42);
            // right.write(0.4);
            getdata_pack();
            run();
        }
        else//距离小于30cm，开始转弯
        {
        in1=0;
        in2=0;
        in3=0;
        in4=0;
            //转直角
            //记录转弯开始时角度
        while(true) {
            StartdeviceEvent();
            if(GetData1)
            {
                break;
            }
            }
        in2=1;
        in4=1;
        left.write(0.3);
        right.write(0.3); 
            //开始转弯
        while(true){
            if(FinishFlag==1) break;//转弯达到90度停止转弯，跳出循环，否则继续转弯
            else{
            while (true) {
            EnddeviceEvent();
            if(GetData2)//收到数据后跳出循环
            {
                break;
            }

           }
          }
         }
            in1=1;
            in2=0;
            in3=0;
            in4=1;
            left.write(0.42);
            right.write(0.4);
            thread_sleep_for(10000);
            in1=1;
            in2=0;
            in3=1;
            in4=0;
            left.write(0.3);
            right.write(0.3);
            thread_sleep_for(1000);
        }

        }
        else
        {
            in1=0;
            in2=0;
            in3=0;
            in4=0;
            break;
        }

        
    }
            // in1=1;
            // in2=0;
            // in3=1;
            // in4=0;
            // while(true)
            // {
            // left.write(0.30);
            // right.write(0.40);
            // }
 
}

