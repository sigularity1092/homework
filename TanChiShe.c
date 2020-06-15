#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <conio.h>
#include <stdlib.h>
int i,j;
//方向键的ASCLL值:上72，左75，右77，下80
//背景颜色的代码： 0=黑色  1蓝色 2 绿色 3湖蓝色 4红色 5紫色 6黄色 7白色 8灰色 9淡蓝色 
//**改变当前光标方块的背景颜色和字体颜色**//
void BackGround(unsigned int ForeColor , unsigned int BackGroundColor ) {
	
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ForeColor + BackGroundColor * 0x10);//改变当前光标的背景和字体颜色
}

//**改变光标的位置**//
void gotoxy(int x, int y) {
	
	COORD coord;   //获取坐标轴结构体
	coord.X = x;
	coord.Y = y;
	 //获取控制台句柄，值为-11
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);   //移动光标到x，y处
}

//**初始化地图数据**//
void restart(int bk[20][20], int look[4], int move[20][20]) {
	//bk为总的地图数据记录整个地图，为1时表示墙体，为2时表示果实，为3时表示蛇
	//look记录数据，为0时表示朝向，为1时表示长度，为3时表示胜负情况,为4表示分数
	//move记录蛇走过的路程，用来打印蛇时判断用 
	int pp, qq;  //用来记录获取的随机坐标
	//接下来要初始化整个地图//
	for( i=0;i<=16;i++)
		for ( j = 0; j <= 16; j++) {
			if (i == 0 || i == 16 || j == 0 || j == 16)  bk[i][j] = 1;//1表示墙体
			else bk[i][j] = 0; //0表示什么都没有
			move[i][j] = 0;    //该数组用来记录蛇移动的轨迹
		}
	//将蛇初始化在8,8坐标上
	bk[8][8] = 3;
	move[8][8] = 1;//则此时8,8,上的轨迹就应该记录为1
	move[0][0] = 1;//用此来记录步数
	pp = rand() % 15 + 1;//范围是1-15
	qq = rand() % 15 + 1;
	bk[pp][qq] = 2;//表示这个位置有果实了
	look[0] = 1;//表示朝向,向上
	look[1] = 1;//表示长度
	look[2] = 0;//当为1是表示失败
	look[3] = 0;//记录得分
	//接下来要绘制地图//
	for( i=0;i<=16;i++)
		for ( j = 0; j <= 16; j++) {
			gotoxy(i * 2, j);//光标移动,每个光标都是矩形
			switch (bk[i][j]) {
			case 0:
				BackGround(0, 0);
				break;//如果没有东西打印黑色
			case 1:
				BackGround(0, 1);
				break;//墙打印蓝色
			case 2:
				BackGround(0, 2);
				break;//果实打印绿色
			case 3:
				BackGround(0, 3);
				break;//蛇打印湖蓝色
			default:
				break;
			}
			printf("  ");//地图中直接就是涂空格符
		}
	//接下来要显示积分//
	gotoxy(35, 0);
	BackGround(7, 0);//用白字黑底打印
	printf("现在得分是:%d,请再接再厉!^_^", look[2]);
}


//**运动主体**//
void map(int bk[20][20], int look[4], int xy[2], int move[20][20]) {
	//bk是地图信息，look作数据记录，xy记录坐标，move记录蛇的运动轨迹
	int b[10], qq=0, pp=0;//b用来吸收输入，qq和pp用来随机初始化果实坐标
	if (kbhit()) {//记录按下的是哪个方向键
		b[0] = getch();//用b来记录
		if (b[0] == 224)  b[0] = getch();//如果为224表示为方向键，但是要再一次获取才行
		if (b[0] == 72 && look[0] != 2)
			//如果输入的为上并且朝向不为下
			look[0] = 1;
		if (b[0] == 80 && look[0] != 1)
			look[0] = 2;
		if (b[0] == 75 && look[0] != 4)
			look[0] = 3;
		if (b[0] == 77 && look[0] != 3)
			look[0] = 4;
	}
	switch (look[0]) {
	case 1:
		//往上走
		xy[1]--;
		break;
	case 2:
		//往下走
		xy[1]++;
		break;
	case 3:
		//往左走
		xy[0]--;
		break;
	case 4:
		//往右走
		xy[0]++;
		break;
	}
	//接下来蛇就开始走动了//
	move[0][0]++;//蛇的步数加一
	move[xy[0]][xy[1]] = move[0][0];//记录当前格子中蛇的轨迹
	gotoxy(35, 2);
	BackGround(7, 0);
	printf("横坐标:%d,纵坐标:%d", xy[0],xy[1]);
	gotoxy(xy[0] * 2, xy[1]);//这里蛇头就往前移动了
	BackGround(0, 3);//与蛇体一个颜色
	printf("  ");

	//如果吃了果实//
	if (bk[xy[0]][xy[1]] == 2) {
		look[2]++;//分数加一
		look[1]++;//长度加一
		//更新分数
		gotoxy(35, 0);
		BackGround(7, 0);
		printf("现在得分是:%d,请再接再厉!^_^", look[2]);
		while (bk[pp][qq] != 0) {
			pp = rand() % 15 + 1;
			qq = rand() % 15 + 1;
		}
		bk[pp][qq] = 2;//将这个地方变为果实
		gotoxy(pp * 2, qq);
		BackGround(0, 2);
		printf("  ");
	}

	//如果撞了墙或者自己//
	if (bk[xy[0]][xy[1]] == 1 || bk[xy[0]][xy[1]] == 3) {
		look[3] = 1;//表示已经输了
		gotoxy(6, 6);
		BackGround(7, 0);
		printf("你输了，最后得分:%d", look[2]);
	}

	bk[xy[0]][xy[1]] = 3;//使这个位置变成蛇
	//接下来要检测蛇然后刷新蛇的位置//
	for( i=0;i<=16;i++)
		for (j = 0; j <= 16; j++) {
			if (move[i][j] == move[xy[0]][xy[1]] - look[1]){
				//如果符合这个条件,则表示蛇已经移动出这个位置了
				//要删除这个位置的蛇尾巴
				//一次只有一个方块会符合要求吧？
				bk[i][j] = 0;
				gotoxy(i * 2, j);
				BackGround(0, 0);
				printf("  ");
				break;//一次只找一个
			}
		}
	end:;
}
int main() {
	int bk[20][20], xy[2], move[20][20], look[4];
	xy[1] = xy[0] = 8;
	srand((unsigned) time(NULL));//初始化随机种子
	system("pause");
	restart(bk, look, move);
	while (look[3] == 0) {
		Sleep(250);//休眠400ms一次
		map(bk, look, xy, move);
	}
	system("pause");
	printf("游戏结束，谢谢游玩!^_^");
	return 0;
}
