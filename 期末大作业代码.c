#include<stdio.h>
#include<string.h>
#include<math.h>
int arlen(int s[])//为了得到数组s[]的实际数据个数，仿照字符串中strlen的函数原型，编写alren()函数
{
    int len = 0;
    while (s[len] != -1)
        len++;
    return len;
}
char* getForm(int n) //定义一个返回值为字符串指针的函数，用于返回字符串，实现递归
{
    char res[300] = "";//定义一个足够大的字符串，用于容纳结果
    int curr = n, i = 0;//定义变量curr来替代形参n，并承接factor，i用于计数
    int s[300];//定义一个足够大的数组，用于储存指数
    for (i = 0; i < 300; i++)
        s[i] = -1;//指数可能为0，故为s[300]中的每个元素赋初值-1，
    i = 0;//下面的while循环需要用i来计数，i恢复为0
    while (curr != 0) //利用while循环从大到小逐个找到factor能被拆解成的2的完全平方数的指数，直至变量curr=0
    {
        int factor = (int)pow(2, 15), exponent = 15;/*定义倍数factor为2^15,指数exponent=15（题目已限定带入形参n的值小于20000，2的整数次幂中只有2^15刚好大于20000),
        pow的函数原型是double类型，需强制转换为int型*/
        while (factor > curr)//利用while循环从大到小找到刚好比curr（即传入实参）小的2的完全平方数
        {
            factor /= 2;
            exponent--;//从15递减，获取其指数
        }
        s[i++] = exponent;//利用s[i]数组来储存指数
        curr -= factor;//curr减去获取的2的完全平方数，继续循环
    }

    for (i = 0; i < arlen(s); i++)//逐个调取储存于s[]中的指数
    {
        int exponent = s[i];
        if (exponent == 0)//如果指数为0，接入2(0)
            strcat(res, "2(0)");
        else if (exponent == 1)//如果指数为1，接入2
            strcat(res, "2");
        else//如果指数大于1,则可以继续进行拆解
        {
            strcat(res, "2(");//指数大于1，则下一步必定要拆指数，那一定是2的指数，故在前面接上2(
            strcat(res, getForm(exponent));/*该处即是本函数的核心所在，将较大的指数传入函数getForm(),再来一步步求指数，拆解成2()……的形式，实现递归*/
            strcat(res, ")");//有头有尾，与2(相对应
        }
        if (i < arlen(s) - 1)//在顺序执行和递归的返回过程中每一组被拆好的2的指数次方之间都需要用+来连接，然而最后一组是不需要的
            strcat(res, "+");
    }
    return res;
}
int main(void)
{
    int x = 0; char* a;
    printf("请输入一个正整数n (n<=20000):\n");
    scanf("%d", &x);//输入一个小于20000的正整数
    a=getForm(x);//进入返回被要求字符串的函数
    printf("%s\n", a);
    return 0;
}
