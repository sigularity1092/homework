#include"DSGraphic.h"
#include <iostream>
#include<cmath>

using namespace std;

const double PI = 3.1415926;

struct JNode
{
	int number;
	double x;
	double y;
	JNode* next;
	void* pObject;
};//结点

struct JQueue
{
	JNode* pFront;
	JNode* pRear;
};//队列

void InitQueue(JQueue& q, int size, double centerX, double centerY, double r);//创建队列

void EnQueue(JQueue& q, int number, double x, double y);//进队

void DeQueue(JQueue& q, int& number, double& x, double& y);//出队


int main()
{
	showWindow();
	setPlayingSpeed(300);//设置动画转换时间
	drawText("遥感1901 金家纬", 20, 20);

	int n = 24, m = 3;
	double r = 200, centerX = 300, centerY = 300;//规定圆心和半径

	cout << "请输入总人数："<<endl;
	cin >> n;
	cout << "请输入间隔："<<endl;
	cin >> m;
	if (m <= 0 || n <= 0)
		return 1;

	JQueue lq;//创建队列

	InitQueue(lq, n, centerX, centerY, r);//结点组成链队，画圆

	int i = 0, k = n;//用i记录每次的数字
	while (k > 1)//队列不空
	{

		double x = 0, y = 0;
		int	number = 0;//用x，y，number三个变量来记录
		DeQueue(lq, number, x, y); i++;//出队
		if (i % m != 0)//如果是m的倍数则出队
			EnQueue(lq, number, x, y);
		else
		{
			drawText("x", x, y);//在窗口中擦除出队元素
			k--;
		}
	}

	drawText("最后的幸存者是：", 520, 400);
	drawText(lq.pFront->number, 520, 450);
	//取出队列中仅有的元素，作为幸存者的数字
	
	system("pause");
	return 0;
}


void InitQueue(JQueue& q, int size, double centerX, double centerY, double r)//创建队列
{
	q.pFront = q.pRear = NULL;//初始时队头和队尾指针均为空
	int count = 1; 	//设置一个计数器count，表示每个人的序号
	double radian = 2 * PI / size;
	while (count <= size)
	{
		int x=0, y=0;//用x，y记录每个点坐标
		x = centerX + cos(radian * (count - 1)) * r;
		y = centerY + sin(radian * (count - 1)) * r;
		EnQueue(q, count, x, y);
		count++;
	}
}

void EnQueue(JQueue& q, int number, double x, double y)//进队,q为引用型参数
{
	JNode* s;

	s = new JNode;//创建新结点s，插入到链队的末尾
	s->number = number;
	s->x = x;
	s->y = y;
	s->next = NULL;
	s->pObject = drawText(number, x, y);//画出未被淘汰的结点

	if (q.pFront == NULL)//原队为空队的情况
		q.pRear = q.pFront = s;//front和rear均指向s结点
	else // 原队不为空队的情况
	{
		q.pRear->next = s;//将结点s链到队尾
		q.pRear = s;//rear指向它
	}
}

void DeQueue(JQueue& q, int& number, double& x, double& y)//出队
{
	clearObject(q.pFront->pObject);//先清除该结点图像
	
	JNode* p;
	if (q.pFront != NULL) //链队不为空
	{
		p = q.pFront;//p指向队头结点
		number = p->number;
		x = p->x;
		y = p->y;
		if (q.pRear == q.pFront)//原队只有一个结点,删除后队变空
			q.pRear = q.pFront = NULL;
		else//原队有两个或以上结点的情况
			q.pFront = q.pFront->next;//将对头指针指向下一个结点

		delete p;
	}
}
