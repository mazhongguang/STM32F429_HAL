#ifndef __HC_SR04_H
#define __HC_SR04_H

#include "main.h"
#define HC_SR PAout(4)			//超声波控制引脚

#define HC_SR_Start() {HC_SR=1;delay_us(20);HC_SR=0;}

void HC_SR_Init(void);				//超声波功能初始化

#endif
