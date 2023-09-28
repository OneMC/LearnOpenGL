/*
 * @Description:
 * @FilePath: /src/Users/zhouwei/Desktop/project/cpp/BMP/include/bmp.h
 * @Author: Wei Zhou
 * @Github: https://github.com/zromyk/BMP
 * @Date: 2020-03-27 09:09:45
 * @LastEditors: Wei Zhou
 * @LastEditTime: 2020-05-05 10:23:45
 * @Copyright: Copyright © 2017 muyiro. All rights reserved.
 */

#pragma once

#include <iostream>
#include <cstdlib>
#include <cstring>

#ifdef __GNUC__
#define PACK( __Declaration__ ) __Declaration__ __attribute__((__packed__))
#endif

#ifdef _MSC_VER
#define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop))
#endif

// BRG
PACK(typedef struct {
    uint8_t blue;     // 蓝色强度
    uint8_t green;    // 绿色强度
    uint8_t red;      // 红色强度
})RGBInfoNode;

/* BMP位图数据 24，32位深适用*/
class BMP
{
public:
    /**
     * @description: 构造函数
     * @param {type} rgbInfo {RGBInfoNode *}: 储存rgb信息的数组首地址
     *               width {uint32_t}: 图像的宽度
     *               height {uint32_t}: 图像的高度
     */
    BMP();
    BMP(uint32_t width, uint32_t height);
    BMP(RGBInfoNode *rgbInfo, uint32_t width, uint32_t height);
    BMP(uint8_t *garyInfo, uint32_t width, uint32_t height);
    ~BMP();

    /**
     * @description: 判断是否有数据
     * @return: {bool} true 有, false 无
     */
    inline bool is_rgb()    {return (rgb != NULL && gary == NULL);}
    inline bool is_gary()   {return (rgb == NULL && gary != NULL);}
    inline bool is_exists() {return (rgb != NULL || gary != NULL);}

    /**
     * @description: 得到图像的高度 / 宽度
     * @return: {uint32_t} 图像的高度 / 宽度
     */
    inline uint32_t getHeight() {return head.info.biHeight;}
    inline uint32_t getWidth() {return head.info.biWidth;}

    /**
     * @description: 读取一张mbp图像的信息 / 生成一张BMP图片
     * @param {type} filePath {const char *}: 需要读取的MBP文件的路径
     * @return: {bool} true: succeed, false: fail
     */
    bool read(const char *filePath, bool showInfo = false);
    bool write(const char *filePath, bool flip_y=false, bool showInfo = false);

    /**
     * @description:
     * @param {type} rgbInfo {RGBInfoNode *}: 储存RGB信息的首地址
     * @return: {bool} true: succeed, false: fail
     */
    bool getPoint(RGBInfoNode *rgbInfo, uint32_t width, uint32_t height);
    bool getPoint(uint8_t *garyInfo, uint32_t width, uint32_t height);
    bool setPoint(RGBInfoNode rgbInfo, uint32_t width, uint32_t height);
    bool setPoint(uint8_t garyInfo, uint32_t width, uint32_t height);

    /**
     * @description: 得到图像的颜色
     * @param {type} garyInfo {uint8_t *}: 储存颜色信息的首地址
     * @return: {bool} true: succeed, false: fail
     */
    bool screenShot(RGBInfoNode* rgbInfo);
    bool screenShot(RGBInfoNode* rgbInfo, uint32_t w_sta, uint32_t h_sta, uint32_t width, uint32_t height);
    bool screenShot(uint8_t *garyInfo);
    bool screenShot(uint8_t *garyInfo, uint32_t w_sta, uint32_t h_sta, uint32_t width, uint32_t height);
    bool fill(RGBInfoNode *rgbInfo);
    bool fill(RGBInfoNode *rgbInfo, uint32_t w_sta, uint32_t h_sta, uint32_t width, uint32_t height);
    bool fill(uint8_t *garyInfo);
    bool fill(uint8_t *garyInfo, uint32_t w_sta, uint32_t h_sta, uint32_t width, uint32_t height);

    /**
     * @description: 清空图像rgb信息
     * @return: {bool} true: succeed, false: fail
     */
    bool clear();

    /**
     * @description: RGB 与 GARY 格式相互转化
     * @return: {bool} true: succeed, false: fail
     */
    bool gary2rgb();
    bool rgb2gary();

private:

#ifdef _WIN32
PACK(typedef struct  // BMP文件头结构，14byte
{
    //uint16_t bfType;      // 2byte，位图文件的类型，标识，就是“BM”二字
    uint32_t bfSize;        // 4byte，位图文件的大小
    uint32_t bfReserved;    // 4byte，位图文件保留字，必须为0
    uint32_t bfOffBits;     // 4byte，偏移数，即位图文件头+位图信息头+调色板的大小
})BitmapFileNode;
#elif __APPLE__
    #include "TargetConditionals.h"
    #if TARGET_OS_MAC
PACK(typedef struct  // BMP文件头结构，14byte
{
    uint16_t bfType;        // 2byte，位图文件的类型，标识，就是“BM”二字
    uint32_t bfSize;        // 4byte，位图文件的大小
    uint32_t bfReserved;    // 4byte，位图文件保留字，必须为0
    uint32_t bfOffBits;     // 4byte，偏移数，即位图文件头+位图信息头+调色板的大小
})BitmapFileNode;
    #else
        #error "Unknown Apple platform"
    #endif
#else
    #error "Unknown compiler"
#endif

PACK(typedef struct // 图像信息区，40byte
{
    uint32_t biSize;            // 4byte，位图信息头的大小
    uint32_t biWidth;           // 4byte，位图的宽度，以像素为单位
    uint32_t biHeight;          // 4byte，位图的高度，以像素为单位
    uint16_t biPlanes;          // 2byte，目标设备的级别，固定值1
    uint16_t biBitCount;        // 2byte，每个像素所需的位数，1-黑白图，4-16色，8-256色，24-真彩色
    uint32_t biCompression;     // 4byte，压缩类型，0(不压缩)，1(BI_RLE8压缩类型)，2(BI_RLE4压缩类型)
    uint32_t biSizeImage;       // 4byte，全部像素占用的字节数，BI_RGB时可设为0
    uint32_t biXPelsPerMeter;   // 4byte，水平分辨率(像素/米)
    uint32_t biYPelsPerMeter;   // 4byte，垂直分辨率(像素/米)
    uint32_t biClrUsed;         // 4byte，位图使用的颜色数，如果为0，则颜色数为2的biBitCount次方
    uint32_t biClrImportant;    // 4byte，重要的颜色数，0代表所有颜色都重要
})BitmapInfoNode;

/* BMP位图头结构体 */
PACK(typedef struct
{
    BitmapFileNode file;
    BitmapInfoNode info;
})BmpHeadNode;

#ifdef _WIN32
    uint16_t bfType;
#endif
    BmpHeadNode head;           // 头结构体
    RGBInfoNode **rgb       = NULL;  // 指向RGB数据的头指针
    uint8_t     **gary      = NULL;  // 指向gary的头指针
    
    uint32_t lineLegnth;    // 扫描的单行数据长度
    uint32_t width;         // BMP位图的长度
    uint32_t height;        // BMP位图的高度
    uint32_t sizeImage;     // 全部像素占用的字节数
    uint32_t offset;        // 偏移数，即位图文件头+位图信息头+调色板的大小
    uint32_t redundant;     // 补齐4字节需要的字节数

    template <typename T>
    void applyArray(T** &point);
    template <typename T>
    void deleteArray(T** &point);
    void bitmapFileShow();
    void bitmapInfoShow();
    void setBMPInfo(uint32_t width, uint32_t height);
};
