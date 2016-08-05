#pragma once
#include "stdafx.h"
#include "Constant.h"

class ImageResource
{
private:
	HBITMAP			imgResource;			//그래픽 리소스
	unsigned int	frameCnt;				//리소스의 프레임 수
	unsigned int	imgCnt;					//이미지의 개수(동작 이미지의 수)
	unsigned int	currentFrame;			//현재 프레임(가로)
	unsigned int	currentImgCnt;			//현재 표시되는 이미지 번호(세로)
	unsigned int	imgWidth, imgHeight;	//이미지의 너비, 높이
	unsigned short	imgFrameWidth;			//이미지 프레임 너비
	unsigned short	imgFrameHeight;			//이미지 프레임 높이
	char			imgMode;				//이미지 모드 (단일 이미지 / 애니메이션 / 다중 애니메이션)
	unsigned char	red, green, blue;		//이미지 배경 색(투명화)
	char			imgState;				//한 번 반복하는 애니메이션인가
public:
	ImageResource();
	ImageResource(const ImageResource& img);
	ImageResource& operator=(const ImageResource& img);
	~ImageResource();



	//---------------------기본 set / get 함수---------------------
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
	//---------------------기본 set / get 함수---------------------



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

