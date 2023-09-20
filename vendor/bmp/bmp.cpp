/*
 * @Description:
 * @FilePath: /BMP/src/bmp.cpp
 * @Author: Wei Zhou
 * @Github: https://github.com/zromyk/BMP
 * @Date: 2020-03-27 20:54:39
 * @LastEditors: Wei Zhou
 * @LastEditTime: 2021-01-25 21:54:42
 * @Copyright: Copyright © 2017 muyiro. All rights reserved.
 */

#include <iostream>
#include <cstdlib>
#include <cstring>
#include "bmp.h"

#define DEBUG_MESSAGE(str)  printf("%s, %s, %d: %s\n", __FILE__, __func__, __LINE__, str)
#define CHECK_ARRAY(array) \
    do { \
        if (array == NULL) { \
            DEBUG_MESSAGE("The BMP image lacks RGB or gary information."); \
            return false; \
        } \
    } while(0)
#define CHECK_BORDER(width, height) \
    do { \
        if (width >= head.info.biWidth || height >= head.info.biHeight) { \
            DEBUG_MESSAGE("The width or height input beyond the border."); \
            return false; \
        } \
    } while(0)

/**
 * @description: 构造函数
 * @param {type}    rgbInfo {RGBInfoNode *}: 储存rgb信息的数组首地址
 *                  width {uint32_t}: 图像的宽度
 *                  height {uint32_t}: 图像的高度
 */
BMP::BMP(){}

BMP::BMP(uint32_t width, uint32_t height)
{
    setBMPInfo(width, height);
    deleteArray(gary);
    applyArray(rgb);

    RGBInfoNode rgbInfo = {0, 0, 0};
    for (uint32_t h = 0; h < height; ++h)
    {
        if (h >= head.info.biHeight) { break; }
        for (uint32_t w = 0; w < width; ++w)
        {
            if (w >= head.info.biWidth) { break; }
            rgb[h][w] = rgbInfo;
        }
    }
}

BMP::BMP(RGBInfoNode *rgbInfo, uint32_t width, uint32_t height)
{
    setBMPInfo(width, height);
    deleteArray(gary);
    applyArray(rgb);
    fill(rgbInfo, 0, 0, width, height);
}

BMP::BMP(uint8_t *garyInfo, uint32_t width, uint32_t height)
{
    setBMPInfo(width, height);
    deleteArray(rgb);
    applyArray(gary);
    fill(garyInfo, 0, 0, width, height);
}

BMP::~BMP()
{
    deleteArray(rgb);
    deleteArray(gary);
}

/**
 * @description: 撤销/申请rgb动态数组内存
* @param {type}    rgb / gary
 */
template <typename T>
void BMP::deleteArray(T** &array) 
{
    if (array != NULL) {
        for (uint32_t h = 0; h < head.info.biHeight; ++h) {
            delete[] array[h];
            array[h] = NULL;
        }
        delete[] array;
        array = NULL;
    }
}

template <typename T>
void BMP::applyArray(T** &array)
{
    deleteArray(array);
    array = new T*[head.info.biHeight];
    if (array == NULL) {
        DEBUG_MESSAGE("Fatal: Malloc failed to allocate memory. Memory release failure.");
        exit(-1);
    }
    for (uint32_t h = 0; h < head.info.biHeight; ++h) {
        array[h] = new T[head.info.biWidth];
        if (array[h] == NULL) {
            DEBUG_MESSAGE("Fatal: Malloc failed to allocate memory. Memory release failure.");
            exit(-1);
        }
    }
}

/**
 * @description:
 * @param {type}    width {uint32_t}: 图像的宽度
 *                  height {uint32_t}: 图像的高度
 * @return:
 */
void BMP::setBMPInfo(uint32_t width, uint32_t height)
{
#ifdef _WIN32
    bfType                    = 19778;      // 2byte，位图文件的类型，标识，就是“BM”二字
#elif TARGET_OS_MAC
    head.file.bfType          = 19778;      // 2byte，位图文件的类型，标识，就是“BM”二字
#endif
    head.file.bfReserved      = 0x00;       // 4byte，位图文件保留字，必须为0
    head.file.bfOffBits       = 138;        // 4byte，偏移数，即位图文件头+位图信息头+调色板的大小
    head.info.biSize          = 124;        // 4byte，位图信息头的大小
    head.info.biWidth         = width;      // 4byte，位图的宽度，以像素为单位
    head.info.biHeight        = height;     // 4byte，位图的高度，以像素为单位
    head.info.biPlanes        = 0x01;       // 2byte，目标设备的级别，固定值1
    head.info.biBitCount      = 24;         // 2byte，每个像素所需的位数，1-黑白图，4-16色，8-256色，24-真彩色
    head.info.biCompression   = 0x00;       // 4byte，压缩类型，0(不压缩)，1(BI_RLE8压缩类型)，2(BI_RLE4压缩类型)
    head.info.biXPelsPerMeter = 2834;       // 4byte，水平分辨率(像素/米)
    head.info.biYPelsPerMeter = 2834;       // 4byte，垂直分辨率(像素/米)
    head.info.biClrUsed       = 0x00;       // 4byte，位图使用的颜色数，如果为0，则颜色数为2的biBitCount次方
    head.info.biClrImportant  = 0x00;       // 4byte，重要的颜色数，0代表所有颜色都重要

    lineLegnth    = (head.info.biWidth*head.info.biBitCount+31)/32*4; // 扫描的单行数据长度
    width         = head.info.biWidth;    // BMP位图的长度
    height        = head.info.biHeight;   // BMP位图的高度
    offset        = head.file.bfOffBits;  // 偏移数，即位图文件头+位图信息头+调色板的大小
    redundant     = head.info.biWidth % 4;// 补齐4字节需要的字节数

    head.info.biSizeImage = head.info.biWidth * head.info.biHeight * 3
                            + head.info.biHeight * redundant;           // 4byte，全部像素占用的字节数，BI_RGB时可设为0
    head.file.bfSize = 138 + head.info.biSizeImage;                     // 4byte，位图文件的大小
    sizeImage   = head.file.bfSize;     // 全部像素占用的字节数
}

/**
 * @description: 显示bmp位图的头结构信息
 * @param {type}
 * @return:
 */
void BMP::bitmapFileShow()
{
    printf("\n--------------------------------------\n");
    printf("bitmap-file header\n\n");
#ifdef _WIN32
    printf("bfType: %7c%c (%d)\n", bfType/256, bfType%256, bfType);
#elif TARGET_OS_MAC
    printf("bfType: %7c%c (%d)\n", head.file.bfType/256, head.file.bfType%256, head.file.bfType);
#endif
    if (head.file.bfSize/1024/1024 > 1024) {
        printf("bfSize: %8.1f GB (%d B)\n", head.file.bfSize / 1024.0 / 1024.0 / 1024.0, head.file.bfSize);
    }
    else if (head.file.bfSize/1024 > 1024) {
        printf("bfSize: %8.1f MB (%d B)\n", head.file.bfSize / 1024.0 / 1024.0, head.file.bfSize);
    }
    else if (head.file.bfSize > 1024) {
        printf("bfSize: %8.1f kB (%d B)\n", head.file.bfSize / 1024.0, head.file.bfSize);
    }
    else {
        printf("bfSize: %8d B\n", head.file.bfSize);
    }
    printf("bfReserved: %4d\n", head.file.bfReserved);
    printf("bfOffBits: %5d\n", head.file.bfOffBits);
}

/**
 * @description: 显示bmp位图的图像数据头结构信息
 * @param {type}
 * @return:
 */
void BMP::bitmapInfoShow()
{
    printf("\n--------------------------------------\n");
    printf("bitmap-information header\n\n");
    printf("biSize: %15d B\n", head.info.biSize);
    printf("biWidth: %14d pixel\n", head.info.biWidth);
    printf("biHeight: %13d pixel\n", head.info.biHeight);
    printf("biPlanes: %13d\n", head.info.biPlanes);
    printf("biBitCount: %11d b deep\n", head.info.biBitCount);
    switch(head.info.biCompression)
    {
        case 0: printf("biCompression: %8d (no compression)\n", head.info.biCompression); break;
        case 1: printf("biCompression: %8d (BI_RLE8 compression type)\n", head.info.biCompression); break;
        case 2: printf("biCompression: %8d (BI_RLE4 compression type)\n", head.info.biCompression); break;
        default: printf("biCompression: %8d (error)\n", head.info.biCompression); break;
    }
    if (head.info.biSizeImage/1024/1024 > 1024) {
        printf("biSizeImage: %10.1f GB (%d B)\n", head.info.biSizeImage / 1024.0 / 1024.0 / 1024.0, head.info.biSizeImage);
    }
    else if (head.info.biSizeImage/1024 > 1024) {
        printf("biSizeImage: %10.1f MB (%d B)\n", head.info.biSizeImage / 1024.0 / 1024.0, head.info.biSizeImage);
    }
    else if (head.info.biSizeImage > 1024) {
        printf("biSizeImage: %10.1f kB (%d B)\n", head.info.biSizeImage / 1024.0, head.info.biSizeImage);
    }
    else {
        printf("biSizeImage: %10d B\n", head.info.biSizeImage);
    }
    printf("biXPelsPerMeter: %6u Pixel/m\n", head.info.biXPelsPerMeter);
    printf("biYPelsPerMeter: %6u Pixel/m\n", head.info.biYPelsPerMeter);
    printf("biClrUsed: %12u\n", head.info.biClrUsed);
    printf("biClrImportant:  %6u\n", head.info.biClrImportant);
}

/**
 * @description: 得到BMP图像的RGB信息
 * @param {type} rgbInfo {RGBInfoNode *}: 储存RGB信息的首地址
 * @return:
 */

bool BMP::setPoint(RGBInfoNode rgbInfo, uint32_t width, uint32_t height)
{
    CHECK_BORDER(width, height);
    if (is_gary()) { gary2rgb(); }
    CHECK_ARRAY(rgb);
    rgb[height][width] = rgbInfo;
    return true;
}

bool BMP::setPoint(uint8_t garyInfo, uint32_t width, uint32_t height)
{
    CHECK_BORDER(width, height);
    if (is_rgb()) { rgb2gary(); }
    CHECK_ARRAY(gary);
    gary[height][width] = garyInfo;
    return true;
}

/**
 * @description: 得到BMP图像的RGB信息
 * @param {type} rgbInfo {RGBInfoNode *}: 储存RGB信息的首地址
 * @return:
 */
bool BMP::getPoint(RGBInfoNode* rgbInfo, uint32_t width, uint32_t height)
{
    CHECK_BORDER(width, height);
     if (is_gary()) { gary2rgb(); }
    CHECK_ARRAY(rgb);
    *rgbInfo = rgb[height][width];
    return true;
}

bool BMP::getPoint(uint8_t* garyInfo, uint32_t width, uint32_t height)
{
    CHECK_BORDER(width, height);
    if (is_rgb()) { rgb2gary(); }
    CHECK_ARRAY(gary);
    *garyInfo = gary[height][width];
    return true;
}

/**
 * @description: 得到BMP图像的颜色信息
 * @param {type} garyInfo {uint8_t *}: 储存颜色信息的首地址
 * @return: {bool} true: succeed, false: fail
 */
bool BMP::screenShot(RGBInfoNode* rgbInfo) 
{
    return screenShot(rgbInfo, 0, 0, head.info.biWidth, head.info.biHeight);
}

bool BMP::screenShot(RGBInfoNode* rgbInfo, uint32_t w_sta, uint32_t h_sta, uint32_t width, uint32_t height)
{
    if (is_gary()) { gary2rgb(); }
    CHECK_ARRAY(rgb);
    for (uint32_t h = h_sta; h < h_sta + height; ++h)
    {
        if (h >= head.info.biHeight) { break; }
        for (uint32_t w = w_sta; w < w_sta + width; ++w)
        {
            if (w >= head.info.biWidth) { break; }
            *rgbInfo = rgb[h][w];
            rgbInfo++;
        }
    }
    return true;
}

bool BMP::screenShot(uint8_t *garyInfo) 
{
    return screenShot(garyInfo, 0, 0, head.info.biWidth, head.info.biHeight);
}

bool BMP::screenShot(uint8_t *garyInfo, uint32_t w_sta, uint32_t h_sta, uint32_t width, uint32_t height)
{
    if (is_rgb()) { rgb2gary(); }
    CHECK_ARRAY(gary);
    for (uint32_t h = h_sta; h < h_sta + height; ++h)
    {
        if (h >= head.info.biHeight) { break; }
        for (uint32_t w = w_sta; w < w_sta + width; ++w)
        {
            if (w >= head.info.biWidth) { break; }
            *garyInfo = gary[h][w];
            garyInfo++;
        }
    }
    return true;
}

/**
 * @description: 填充函数
 * @param {type}    rgbInfo {RGBInfoNode *}: 储存rgb信息的数组首地址
 *                  garyInfo {uint8_t *}: 储存gary信息的数组首地址
 *                  width {uint32_t}: 图像的宽度
 *                  height {uint32_t}: 图像的高度
 */
bool BMP::fill(RGBInfoNode *rgbInfo) 
{
    return fill(rgbInfo, 0, 0, head.info.biWidth, head.info.biHeight);
}

bool BMP::fill(RGBInfoNode *rgbInfo, uint32_t w_sta, uint32_t h_sta, uint32_t width, uint32_t height)
{
    if (is_gary()) { gary2rgb(); }
    CHECK_ARRAY(rgb);
    for (uint32_t h = h_sta; h < h_sta + height; ++h)
    {
        if (h >= head.info.biHeight) { break; }
        for (uint32_t w = w_sta; w < w_sta + width; ++w)
        {
            if (w >= head.info.biWidth) { break; }
            rgb[h][w] = *rgbInfo;
            rgbInfo++;
        }
    }
    return true;
}

bool BMP::fill(uint8_t *garyInfo) 
{
    return fill(garyInfo, 0, 0, head.info.biWidth, head.info.biHeight);
}

bool BMP::fill(uint8_t *garyInfo, uint32_t w_sta, uint32_t h_sta, uint32_t width, uint32_t height)
{
    if (is_rgb()) { rgb2gary(); }
    CHECK_ARRAY(gary);
    for (uint32_t h = h_sta; h < h_sta + height; ++h)
    {
        if (h >= head.info.biHeight) { break; }
        for (uint32_t w = w_sta; w < w_sta + width; ++w)
        {
            if (w >= head.info.biWidth) { break; }
            gary[h][w] = *garyInfo;
            garyInfo++;
        }
    }
    return true;
}

/**
 * @description: 清空图像rgb信息
 * @return: {bool} true: succeed, false: fail
 */
bool BMP::clear()
{
    if (rgb != NULL)
    {
        for (uint32_t h = 0; h < head.info.biHeight; ++h)
        {
            memset(rgb[h], 0, sizeof(RGBInfoNode) * head.info.biWidth);
        }
        return true;
    }
    if (gary != NULL)
    {
        for (uint32_t h = 0; h < head.info.biHeight; ++h)
        {
            memset(gary[h], 0, sizeof(uint8_t) * head.info.biWidth);
        }
        return true;
    }
    return false;
}

/**
 * @description: RGB 格式转化成 GARY 格式
 * @return: {bool} true: succeed, false: fail
 */
bool BMP::gary2rgb()
{
    CHECK_ARRAY(gary);
    applyArray(rgb);
    for (uint32_t h = 0; h < head.info.biHeight; ++h)
    {
        for (uint32_t w = 0; w < head.info.biWidth; ++w)
        {
            rgb[h][w].red   = gary[h][w];
            rgb[h][w].green = gary[h][w];
            rgb[h][w].blue  = gary[h][w];
        }
    }
    deleteArray(gary);
    return true;
}

bool BMP::rgb2gary()
{
    CHECK_ARRAY(rgb);
    applyArray(gary);
    for (uint32_t h = 0; h < head.info.biHeight; ++h)
    {
        for (uint32_t w = 0; w < head.info.biWidth; ++w)
        {
            gary[h][w] = (rgb[h][w].red + rgb[h][w].green + rgb[h][w].blue) / 3;
        }
    }
    deleteArray(rgb);
    return true;
}

/**
 * @description: 读取一张mbp图像的信息
 * @param {type} filePath {const char *}: 需要读取的MBP文件的路径
 * @return: {bool} true: succeed, false: fail
 */
bool BMP::read(const char *filePath, bool showInfo)
{
    FILE *fp;
    if ((fp = fopen(filePath, "rb")) == NULL) {
        DEBUG_MESSAGE((std::string("Can't open ") + std::string(filePath)).c_str());
        return false;
    }
#ifdef _WIN32
    fread(&bfType, sizeof(uint16_t), 1, fp);
#endif
    fread(&head, sizeof(BmpHeadNode), 1, fp);

    if (showInfo == true) {
        bitmapFileShow(); // 显示文件头结构体信息
        bitmapInfoShow(); // 显示图像数据头结构体信息
    }
    lineLegnth  = (head.info.biWidth*head.info.biBitCount+31)/32*4; // 扫描的单行数据长度
    width       = head.info.biWidth;    // BMP位图的长度
    height      = head.info.biHeight;   // BMP位图的高度
    sizeImage   = head.file.bfSize;     // 全部像素占用的字节数
    offset      = head.file.bfOffBits;  // 偏移数，即位图文件头+位图信息头+调色板的大小
    redundant   = head.info.biWidth % 4; // 补齐4字节需要的字节数

    // 限制读取的图像长宽
    if (head.info.biHeight > 32768 || head.info.biWidth > 32768) {
        DEBUG_MESSAGE("The image is too large. Please check if the image is damaged.");
    }
    else {
        deleteArray(gary);
        applyArray(rgb); // 指向RGB数据的二位数组头指针
        for (uint32_t iy = 0; iy < head.info.biHeight; ++iy) {
            //因为bmp文件的原点是左下角，所以bmp图片需要顺着y轴的反方向来读取
            fseek(fp, head.file.bfOffBits + (head.info.biHeight-1 - iy) * lineLegnth, SEEK_SET); //最后一行开始读取
            fread(rgb[iy], sizeof(RGBInfoNode), head.info.biWidth, fp);
        }
    }
    fclose(fp);
    return true;
}

/**
 * @description: 生成一张BMP图片
 * @param {type} filePath {const char *}: 需要读取的MBP文件的路径
 * @return: {bool} true: succeed, false: fail
 */
bool BMP::write(const char *filePath, bool flip_y, bool showInfo)
{
    FILE *fp;

    if ((fp = fopen(filePath, "wb")) == NULL) {
        DEBUG_MESSAGE((std::string("Can't build ") + std::string(filePath)).c_str());
        return false;
    }
    if (rgb == NULL && gary == NULL) { // 如果没有rgb图像信息，发送错误消息
        DEBUG_MESSAGE("Unable to find image information.");
        return false;
    }

    if (showInfo == true) {
        bitmapFileShow(); // 显示文件头结构体信息
        bitmapInfoShow(); // 显示图像数据头结构体信息
    }

    uint32_t ix = 0;
#ifdef _WIN32
    fwrite(&bfType, sizeof(uint16_t), 1, fp);
    ix += sizeof(uint16_t);
#endif
    fwrite(&head, sizeof(BmpHeadNode), 1, fp);
    ix += sizeof(BmpHeadNode);
    for (; ix < head.file.bfOffBits; ++ix) {
        fputc(0x00, fp);
    }

    if (rgb != NULL)
    {
        for (uint32_t iy = 0; iy < head.info.biHeight; ++iy) {
            uint32_t y = flip_y ? iy : head.info.biHeight-1 - iy;
            fwrite(rgb[y], sizeof(RGBInfoNode), head.info.biWidth, fp);
            for (uint32_t ix = 0; ix < redundant; ++ix) {
                fputc(0x00, fp);
            }
        }
    }
    else
    {
        RGBInfoNode **rgb_temp = NULL;
        applyArray(rgb_temp);
        for (uint32_t h = 0; h < head.info.biHeight; ++h)
        {
            for (uint32_t w = 0; w < head.info.biWidth; ++w)
            {
                rgb_temp[h][w].red   = gary[h][w];
                rgb_temp[h][w].green = gary[h][w];
                rgb_temp[h][w].blue  = gary[h][w];
            }
        }
        for (uint32_t iy = 0; iy < head.info.biHeight; ++iy) {
            uint32_t y = flip_y ? iy : head.info.biHeight-1 - iy;
            fwrite(rgb_temp[y], sizeof(RGBInfoNode), head.info.biWidth, fp);
            for (uint32_t ix = 0; ix < redundant; ++ix) {
                fputc(0x00, fp);
            }
        }
        deleteArray(rgb_temp);
    }
    fclose(fp);
    return true;
}
