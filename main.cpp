#include "mbed.h"
#include "HCSR04.h"

HCSR04 sensor1(D8, D9);//front supersonic sensor, trig, echo
HCSR04 sensor2(D10, D11);//left supersonic sensor1
HCSR04 sensor3(D12, D13);//left supersonic sensor2
DigitalOut in1(D2);
DigitalOut in2(D3);
DigitalOut in3(D4);
DigitalOut in4(D5);
PwmOut left(D6);
PwmOut right(D7);

float distance1,distance2,distance3;
float MiniEdge=10;
float MaxEdge=20;
float TurnDistanse1=20;//正前方超声波转弯距离
float TurnDistanse2=50;//侧面超声波转弯距离
float diff;//超声波的测量差距

// main() runs in its own thread in the OS
void forward()//向前直行
{
    in1=1;
    in2=0;
    in3=0;
    in4=1;
    left.write(0.45);
    right.write(0.4);
}
void DiffNegative()//车头向右微调
{
    in1=1;
    in2=0;
    in3=1;
    in4=0;
    left.write(0.25);
    right.write(0.25); 
    thread_sleep_for(100);
}
void DiffPositive()//车头向左微调
{
    in1=0;
    in2=1;
    in3=0;
    in4=1;
    left.write(0.25);
    right.write(0.25); 
    thread_sleep_for(100);
}
void Rightward()//向右偏
{
    in1=1;
    in2=0;
    in3=0;
    in4=1;
    left.write(0.6);
    right.write(0.4);
    thread_sleep_for(600);
}
void Leftward()//向左偏
{
    in1=1;
    in2=0;
    in3=0;
    in4=1;
    left.write(0.45);
    right.write(0.6);
    thread_sleep_for(600);
}
void AnticlockQuarterTurn()//逆时针直角转弯
{
    in1=1;
    in2=0;
    in3=1;
    in4=0;
    left.write(0.3);
    right.write(0.3);
    thread_sleep_for(1000);
}
void ClockQuarterTurn()//顺时针直角转弯
{
    in1=0;
    in2=1;
    in3=0;
    in4=1;
    left.write(0.3);
    right.write(0.3);
    thread_sleep_for(1000);
}
void Stop()
{
    in1=0;
    in2=0;
    in3=0;
    in4=0;
}
void AlongHandrail()//沿着栏杆走，并修正方向
{
    if(sensor2.getCm()>MaxEdge||sensor3.getCm()>MaxEdge)//离栏杆大于20cm，偏向栏杆
    {
        Leftward();
    }
    else if(sensor2.getCm()<MiniEdge||sensor3.getCm()>MiniEdge)//离栏杆小于10cm，偏离栏杆
    {
        Rightward();
    }
    else 
    {
        diff=sensor2.getCm()-sensor3.getCm();
        if(abs(diff)<0.5)
        {
            forward();
            thread_sleep_for(500);
            //认为小车水平，保持直行
        }
        else if(diff<-0.5)
        {
            DiffNegative();//车头向右微调
        }
        else if(diff>0.5)
        {
            DiffPositive();//车头向左微调
        }

    }
}
int main()
{
    // while (true) {
    //         printf("%.3f %.3f %.3f",sensor1.getCm(),sensor2.getCm(),sensor3.getCm());
    // }

    while(true)
    {
        if(sensor1.getCm()<20)//前方超声波未识别到栏杆，直行
        {
            in1=1;
            in2=0;
            in3=0;
            in4=1;
            left.write(0.45);
            right.write(0.4);
        }
        else {//识别到栏杆，逆时针直角转弯
            Stop();
            thread_sleep_for(500);
            AnticlockQuarterTurn();
            break;//转弯结束，跳出循环
        }
    }

    //开始通过超声波和栏杆进行巡线，第一个直线
    while(true)
    {
        if(sensor2.getCm()>TurnDistanse2&&sensor3.getCm()>TurnDistanse2)//沿栏杆的第一个直角转弯
        {
            forward();
            thread_sleep_for(500);
            Stop();
            ClockQuarterTurn();
            break;//跳出循环
        }
        else//没有到转弯条件，沿着栏杆前进
        {
            AlongHandrail();
        }

    }

    //第二个直线，和第一个直线一样除了转弯条件
    while(true)
    {
        if(sensor2.getCm()>TurnDistanse1)//沿栏杆的第二个直角转弯
        {
            forward();
            thread_sleep_for(500);
            Stop();
            AnticlockQuarterTurn();
            break;//跳出循环
        }
        else//没有到转弯条件，沿着栏杆前进
        {
            AlongHandrail();
        }

    }

    //第三个直线，和第一个直线完全一样
    while(true)
    {
        if(sensor2.getCm()>TurnDistanse2&&sensor3.getCm()>TurnDistanse2)//沿栏杆的第三个直角转弯
        {
            forward();
            thread_sleep_for(500);
            Stop();
            ClockQuarterTurn();
            break;//跳出循环
        }
        else
        {
            AlongHandrail();
        }
    }

    //第四个直线，和第二个直线完全一样
    while(true)
    {
        if(sensor2.getCm()>TurnDistanse1)//沿栏杆的第四个直角转弯
        {
            forward();
            thread_sleep_for(500);
            Stop();
            AnticlockQuarterTurn();
            break;//跳出循环
        }
        else//没有到转弯条件，沿着栏杆前进
        {
            AlongHandrail();
        }

    }

    //第五个直线，将要识别垃圾桶并投放乒乓球
    while(true)
    {
        if(sensor2.getCm()>TurnDistanse1)//识别到垃圾桶，投球并转弯
        {
            Stop();
            thread_sleep_for(500);

            /*
            添加投球代码
            */

            AnticlockQuarterTurn();
            break;//跳出循环
        }
        else//没有到转弯条件，沿着栏杆前进
        {
            AlongHandrail();
        }

    }

    /*
    接下来开始前往花坛，发送消息
    */


}

