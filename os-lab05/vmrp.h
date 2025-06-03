
#ifndef VMRP_H
#define VMRP_H

#include <iostream>
#include <iomanip> 

using namespace std;

class Replace {
public:
Replace();
~Replace();
void Lru(void);
void Fifo(void);
void Clock(void);
void Eclock(void);
void Lfu(void); 
void Mfu(void);

private:
int PageNumber;  // 访问页数
int FrameNumber; // 内存实页数
int FaultNumber; // 缺页数
int* ReferencePage; // 引用页号数组
int* EliminatePage; // 淘汰页号数组
int* PageFrames;  // 实页号数组
void InitSpace(const char* MethodName); // 初始化空间
void Report(void); // 报告结果
};

#endif
