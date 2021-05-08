/********************************************************************
file base:	DSGraphic
file ext:	h
author:		�ۻ�(shaohua@njtech.edu.cn)
purpose:	ͼ�ο�ӿ�
version:    1.4
*********************************************************************/

#pragma once
#include <Windows.h>
#include <string>
#include <gdiplus.h>
#include <locale>

using namespace std;
class GraphicObject;

/**	����
	@param  x     ���x����
	@param  y     ���y����
	@param  size  ��İ뾶
	@param  color �����ɫ
	@return ���볡���ж����ָ��
*/
GraphicObject* drawPoint(float x, float y, float size = 3, Gdiplus::Color color = Gdiplus::Color::Red);

/**	����
	@param  x1,y1  �߶���������
	@param  x2,y2  �߶��յ������
	@param  width  �߶εĿ��
	@param  color  �߶ε���ɫ
	@return ���볡���ж����ָ��
*/
GraphicObject* drawLine(float x1, float y1, float x2, float y2, float width = 1.0f, Gdiplus::Color color = Gdiplus::Color::Green);

/**	�����ַ���
	@param  s     �ַ�������
	@param  x     �ַ���λ�õ�x����
	@param  y     �ַ���λ�õ�y����
	@param  size  �ֺŴ�С
	@param  color �ַ�������ɫ
	@return ���볡���ж����ָ��
*/
GraphicObject* drawText(string s, float x, float y, float size = 20, Gdiplus::Color color = Gdiplus::Color::Blue);

/**	�����ַ���
@param  i     �ַ�������
@param  x     �ַ���λ�õ�x����
@param  y     �ַ���λ�õ�y����
@param  size  �ֺŴ�С
@param  color �ַ�������ɫ
@return ���볡���ж����ָ��
*/
GraphicObject* drawText(int i, float x, float y, float size = 20, Gdiplus::Color color = Gdiplus::Color::Blue);

/**	��ʾ����
	@param  left   �������Ͻǵ�x����
	@param  top    �������Ͻǵ�y����
	@param  width  ���ڿ��
	@param  Height ���ڸ߶�
*/
int showWindow(int left = 100, int top = 100, int width = 800, int height = 600);

/**	��մ���
*/
void clearWindow();

/**	���ö������¼��
@param  speed   ���³�������ʱ���õļ��ʱ�䣨�Ժ���Ϊ��λ��
*/
void setPlayingSpeed(unsigned speed);

/**	�ڳ����в���һ������
@param  p   ָ�򱻲�������
*/
void clearObject(void* p);


//////////***��������Ϊ�ڲ�ͼ��ϵͳ�����࣬�ⲿ����ʹ��***//////////////////////////
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

