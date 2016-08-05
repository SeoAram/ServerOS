#include "stdafx.h"
#include "ImageResource.h"


ImageResource::ImageResource()
{
	currentFrame = 0;
	currentImgCnt = 0;
}

ImageResource::ImageResource(const ImageResource& img){
	imgResource = img.imgResource;
	frameCnt = img.frameCnt;
	imgCnt = imgCnt;
	currentFrame = img.currentFrame;
	currentImgCnt = img.currentImgCnt;
	imgWidth = img.imgWidth;
	imgHeight = img.imgHeight;
	imgFrameWidth = img.imgFrameWidth;
	imgFrameHeight = img.imgFrameHeight;
	imgMode = img.imgMode;
	red = img.red;
	green = img.green;
	blue = img.blue;
	imgState = img.imgState;
}

ImageResource& ImageResource::operator=(const ImageResource& img){
	imgResource = img.imgResource;
	frameCnt = img.frameCnt;
	imgCnt = imgCnt;
	currentFrame = img.currentFrame;
	currentImgCnt = img.currentImgCnt;
	imgWidth = img.imgWidth;
	imgHeight = img.imgHeight;
	imgFrameWidth = img.imgFrameWidth;
	imgFrameHeight = img.imgFrameHeight;
	imgMode = img.imgMode;
	red = img.red;
	green = img.green;
	blue = img.blue;
	imgState = img.imgState;
	return *this;
}

ImageResource::~ImageResource()
{
}


int ImageResource::setImgResource(const int& resourceImg, const int& imgCount, const int& imgFrame, const LPARAM& lParam, const char& imgMode, const unsigned char& r, const unsigned char& g, const unsigned char& b){

	imgResource = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(resourceImg));
	if (imgResource == NULL)
		return IMAGELOAD_FAIL;

	imgCnt = imgCount;
	frameCnt = imgFrame;
	this->imgMode = imgMode;

	BITMAP bmp;
	GetObject(imgResource, sizeof(BITMAP), &bmp);
	imgWidth = bmp.bmWidth;
	imgHeight = bmp.bmHeight;

	imgFrameWidth = imgWidth / imgFrame;
	imgFrameHeight = imgHeight / imgCount;

	red = r;
	green = g;
	blue = b;

	return IMAGELOAD_SUCCESS;
}

void ImageResource::Animate(){
	if (IMAGE_SINGLE_FRAME == imgMode){
		currentFrame = 0;
	}
	else if (IMAGE_MULTIPLE_FRAME == imgMode){
		(++currentFrame) %= frameCnt;
	}
	else if (IMAGE_MULTI_ANIM == imgMode){
		(++currentFrame) %= frameCnt;
		if (imgState != CHARACTER_STOP && currentFrame == 0){
			currentImgCnt = 3;
			imgState = CHARACTER_STOP;
		}
	}
}

int ImageResource::Animate(const char CharacterView){
	if (IMAGE_SINGLE_FRAME == imgMode){
		currentFrame = 0;
	}
	else if (IMAGE_MULTIPLE_FRAME == imgMode){
		(++currentFrame) %= frameCnt;
	}
	else if (IMAGE_MULTI_ANIM == imgMode){
		(++currentFrame) %= frameCnt;
		if (currentFrame == 0){
			currentImgCnt = CharacterView + CHARACTER_STOP;
			imgState = CHARACTER_STOP;
		}
	}
	return currentFrame;
}

void ImageResource::Render(HWND& hWnd, HDC& hdc){
	static HDC mem1dc, mem2dc;
	static HBITMAP hBit1, oldBit1, oldBit2;
	
	hdc = GetDC(hWnd);

	if (hBit1 == NULL){
		hBit1 = CreateCompatibleBitmap(hdc, WINDOW_WIDTH, WINDOW_HEIGHT);
	}

	mem1dc = CreateCompatibleDC(hdc);
	mem2dc = CreateCompatibleDC(mem1dc);

	oldBit1 = (HBITMAP)SelectObject(mem1dc, hBit1);
	oldBit2 = (HBITMAP)SelectObject(mem1dc, imgResource);
	
	SelectObject(mem2dc, imgResource);
	
	TransparentBlt(mem1dc, currentFrame*imgFrameWidth, currentImgCnt*imgFrameHeight, 
		imgFrameWidth, imgFrameHeight, mem2dc, currentFrame*imgFrameWidth, currentImgCnt*imgFrameHeight, 
		imgFrameWidth, imgFrameHeight, RGB(red, green, blue));


	SetBkMode(mem1dc, TRANSPARENT);


	BitBlt(hdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, mem1dc, 0, 0, SRCCOPY);

	SelectObject(mem2dc, oldBit2); DeleteDC(mem2dc);
	SelectObject(mem1dc, oldBit1); DeleteDC(mem1dc);

	ReleaseDC(hWnd, hdc);
}

void ImageResource::Render(HDC& mem1dc, HDC& mem2dc, const int& x, const int& y){
	SelectObject(mem2dc, (HBITMAP)imgResource);
	if (IMAGE_MULTI_ANIM == imgMode || IMAGE_MULTIPLE_FRAME == imgMode){
		TransparentBlt(mem1dc, x - imgFrameWidth / 2, y - imgFrameHeight + BLOCK_SIZE / 2, imgFrameWidth, imgFrameHeight,
			mem2dc, currentFrame*imgFrameWidth, currentImgCnt*imgFrameHeight, imgFrameWidth, imgFrameHeight, RGB(red, green, blue));
	}
	else if (IMAGE_SINGLE_FRAME == imgMode){
		TransparentBlt(mem1dc, x, y, imgWidth, imgHeight,
			mem2dc, 0, 0, imgWidth, imgHeight, RGB(red, green, blue));
	}
}

void ImageResource::RenderX2(HDC& mem1dc, HDC& mem2dc, const int x, const int y){
	SelectObject(mem2dc, (HBITMAP)imgResource);
	if (IMAGE_MULTI_ANIM == imgMode || IMAGE_MULTIPLE_FRAME == imgMode){
		TransparentBlt(mem1dc, x - imgFrameWidth, y - imgFrameHeight, (imgFrameWidth * 2), (imgFrameHeight * 2),
			mem2dc, currentFrame*imgFrameWidth, currentImgCnt*imgFrameHeight, imgFrameWidth, imgFrameHeight, RGB(red, green, blue));
	}
	else if (IMAGE_SINGLE_FRAME == imgMode){
		TransparentBlt(mem1dc, x, y, imgWidth * 2, imgHeight * 2,
			mem2dc, 0, 0, imgWidth, imgHeight, RGB(red, green, blue));
	}
}

void ImageResource::setAnimate(unsigned int animNum, char state){
	if (0<= animNum && animNum < imgCnt){
		imgState = state;
		currentImgCnt = animNum;
		currentFrame = 0;
	}
}