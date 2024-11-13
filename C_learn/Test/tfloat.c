#include<stdio.h>
#include <STRING.H>

void main()
{
	//单精度浮点数计算机中二进制的表示方法（1位表示符号+8位表示指数(正则表达式)+23位表示尾部(EXCESS系统法)）
	float data; 
	unsigned long buff;
	int i,t;
	char s[34], q[34];
	// 将 0.75 以单精度浮点数的形式存储在变量 date 中
	data=(float)1.2;
    t = data;
	// 把数据复制到 4 字节长度的整数变量 buff 中以逐个提取出每一位
	memcpy(&buff,&data,4);
	// 逐一提取出每一位
	for (i = 33; i >= 0; i--) 
	{
		// 加入破折号来区分符号部分、指数部分和尾数部分
		if (i==1||i==10)
		{
			s[i] = '-';
            q[i] = '-';
		}
		else
		{
			// 为各个字节赋值 '0' 或者 '1'
			if (buff % 2 == 1)
			{
				s[i]='1';
			}
			else
			{
				s[i]='0';
			}
			buff/=2;

            if (t % 2 == 1)
			{
				q[i]='1';
			}
			else
			{
				q[i]='0';
			}
			t/=2;
		}
	}

	s[34]='\0';
    q[34]='\0';
	printf("%s\n",s);
    printf("%s\n",q);
}
