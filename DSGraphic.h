/********************************************************************
file base:	DSGraphic
file ext:	h
author:		邵华(shaohua@njtech.edu.cn)
purpose:	图形库接口
version:    1.4
*********************************************************************/

#pragma once
#include <Windows.h>
#include <string>
#include <gdiplus.h>
#include <locale>

using namespace std;
class GraphicObject;

/**	画点
	@param  x     点的x坐标
	@param  y     点的y坐标
	@param  size  点的半径
	@param  color 点的颜色
	@return 加入场景中对象的指针
*/
GraphicObject* drawPoint(float x, float y, float size = 3, Gdiplus::Color color = Gdiplus::Color::Red);

/**	画线
	@param  x1,y1  线段起点的坐标
	@param  x2,y2  线段终点的坐标
	@param  width  线段的宽度
	@param  color  线段的颜色
	@return 加入场景中对象的指针
*/
GraphicObject* drawLine(float x1, float y1, float x2, float y2, float width = 1.0f, Gdiplus::Color color = Gdiplus::Color::Green);

/**	绘制字符串
	@param  s     字符串内容
	@param  x     字符串位置的x坐标
	@param  y     字符串位置的y坐标
	@param  size  字号大小
	@param  color 字符串的颜色
	@return 加入场景中对象的指针
*/
GraphicObject* drawText(string s, float x, float y, float size = 20, Gdiplus::Color color = Gdiplus::Color::Blue);

/**	绘制字符串
@param  i     字符串内容
@param  x     字符串位置的x坐标
@param  y     字符串位置的y坐标
@param  size  字号大小
@param  color 字符串的颜色
@return 加入场景中对象的指针
*/
GraphicObject* drawText(int i, float x, float y, float size = 20, Gdiplus::Color color = Gdiplus::Color::Blue);

/**	显示窗口
	@param  left   窗口左上角的x坐标
	@param  top    窗口左上角的y坐标
	@param  width  窗口宽度
	@param  Height 窗口高度
*/
int showWindow(int left = 100, int top = 100, int width = 800, int height = 600);

/**	清空窗口
*/
void clearWindow();

/**	设置动画更新间隔
@param  speed   更新场景数据时设置的间隔时间（以毫秒为单位）
*/
void setPlayingSpeed(unsigned speed);

/**	在场景中擦除一个对象
@param  p   指向被擦除对象
*/
void clearObject(void* p);


//////////***以下内容为内部图形系统所需类，外部无需使用***//////////////////////////
enum GraphicType { gtPixel, gtPoint, gtLine, gtText };

class GraphicObject
{
public:
	Gdiplus::Color mColor;
	virtual GraphicType getGraphicType() = 0;
};

class DSPoint : public GraphicObject
{
public:
	DSPoint(float x, float y, float size, Gdiplus::Color color):mx(x),my(y),mSize(size)
	{
		mColor = color;
	}
	float mx;
	float my;
	float mSize;
	GraphicType getGraphicType()
	{
		return gtPoint;
	}
};

class DSLine : public GraphicObject
{
public:
	DSLine(float x1, float y1, float x2, float y2, float width, Gdiplus::Color color)
		:mx1(x1), mx2(x2), my1(y1), my2(y2), mWidth(width)
	{
		mColor = color;
	}
	GraphicType getGraphicType()
	{
		return gtLine;
	}
	float mx1,mx2,my1,my2;
	float mWidth;
};

class DSText :public GraphicObject
{
public:
	DSText(string s, float x, float y, float size, Gdiplus::Color color):mx(x),my(y),mSize(size)
	{
		mColor = color;

		const char* source = s.c_str();
		std::string curLocale = setlocale(LC_ALL, NULL); // curLocale = "C";  
		setlocale(LC_ALL, "chs");
		size_t length = s.size() + 1;
		wchar_t* dest = new wchar_t[length];
		wmemset(dest, 0, length);
		//mbstowcs(dest, source, length);
		size_t converted = 0;
		mbstowcs_s(&converted, dest, length, source, _TRUNCATE);
		mText = dest;
		setlocale(LC_ALL, curLocale.c_str());
		delete[] dest;
	}

	GraphicType getGraphicType()
	{
		return gtText;
	}
	
	wstring mText;
	float mx, my;
	float mSize;
};

