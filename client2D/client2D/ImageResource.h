#pragma once
#include "stdafx.h"
#include "Constant.h"

class ImageResource
{
private:
	HBITMAP			imgResource;			//�׷��� ���ҽ�
	unsigned int	frameCnt;				//���ҽ��� ������ ��
	unsigned int	imgCnt;					//�̹����� ����(���� �̹����� ��)
	unsigned int	currentFrame;			//���� ������(����)
	unsigned int	currentImgCnt;			//���� ǥ�õǴ� �̹��� ��ȣ(����)
	unsigned int	imgWidth, imgHeight;	//�̹����� �ʺ�, ����
	unsigned short	imgFrameWidth;			//�̹��� ������ �ʺ�
	unsigned short	imgFrameHeight;			//�̹��� ������ ����
	char			imgMode;				//�̹��� ��� (���� �̹��� / �ִϸ��̼� / ���� �ִϸ��̼�)
	unsigned char	red, green, blue;		//�̹��� ��� ��(����ȭ)
	char			imgState;				//�� �� �ݺ��ϴ� �ִϸ��̼��ΰ�
public:
	ImageResource();
	ImageResource(const ImageResource& img);
	ImageResource& operator=(const ImageResource& img);
	~ImageResource();



	//---------------------�⺻ set / get �Լ�---------------------
	void setFrameCnt(const unsigned int& i){ frameCnt = i; }
	void setImgCnt(const unsigned int& i){ imgCnt = i; }
	void setCurrentFrame(const unsigned int& i){ currentFrame = i; }
	void setCurrentImgCnt(const unsigned int& i){ currentImgCnt = i; }
	void setImgWidth(const unsigned int& i){ imgWidth = i; }
	void setImgheight(const unsigned int& i){ imgHeight = i; }
	void setImgFrameWidth(const unsigned int& i){ imgFrameWidth = i; }
	void setImgFrameHeight(const unsigned int& i){ imgFrameHeight = i; }
	void setImgFrameSize(const unsigned int& w, const unsigned int& h){
		imgFrameWidth = w;
		imgFrameHeight = h;
	}
	void setImgState(const char& s){ imgState = s; }

	unsigned int getFrameCnt() const { return frameCnt; }
	unsigned int getImgCnt() const { return imgCnt; }
	unsigned int getCurrentFrame() const { return currentFrame; }
	unsigned int getCurrentImgCnt() const { return currentImgCnt; }
	unsigned int getImgWidth() const { return imgWidth; }
	unsigned int getImgHeight() const { return imgHeight; }
	unsigned short getImgFrameWidth() const { return imgFrameWidth; }
	unsigned short getImgFrameHeight() const { return imgFrameHeight; }
	HBITMAP& getImage() { return imgResource; }
	HBITMAP& getImgResource() { return imgResource; }
	char getImgState(){ return imgState; }
	//---------------------�⺻ set / get �Լ�---------------------



	void setImgResource(const int& resourceImg, const LPARAM& lParam){
		imgResource = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(resourceImg));
	}
	int setImgResource(const int& resourceImg, const int& imgCount, const int& imgFrame, const LPARAM& lParam, const char& imgMode, const unsigned char& r = 0, const unsigned char& g = 0, const unsigned char& b = 0);

	void Animate();
	int Animate(const char CharacterView);
	void Render(HWND& hWnd, HDC& hdc);
	void Render(HDC& mem1dc, HDC& mem2dc, const int& x = 0, const int& y = 0);
	void RenderX2(HDC& mem1dc, HDC& mem2dc, const int x = 0, const int y = 0);

	void setAnimate(unsigned int animNum = 0, char state = CHARACTER_STOP);
};

