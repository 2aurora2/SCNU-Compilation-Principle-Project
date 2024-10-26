/**
*****************************************************************************
*  Copyright (C), 2024, 林江荣
*  All right reserved. See COPYRIGHT for detailed Information.
*
*  @file    utils.h
*  @brief   常用操作的函数封装为类的静态成员：包括但不限于文件操作
*
*  @author  林江荣
*  @date    2024-10-26
*  @version V1.0.0
*----------------------------------------------------------------------------
*  @note 历史版本  修改人员    修改内容
*  @note V1.0.0        林江荣        创建文件
*****************************************************************************
*/

#ifndef UTILS_H
#define UTILS_H

#include <QString>

/*!
    @ClassName      Util
    @Description    常用操作的函数封装于Util类作为其静态成员函数
*/
class Util {
public:
    static QString ReadFile();
};

#endif  // UTILS_H
