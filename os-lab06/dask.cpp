#include "dask.h"
#include <cstdlib>
#include <ctime>
#include <bits/stdc++.h>
#define MAX_CYLINDER 200
using namespace std;
DiskArm::DiskArm() {
    int i;
    // 输入当前道号
    cout << "Please input Current cylinder :";
    cin >> CurrentCylinder;
    // 磁头方向，输入 0 表示向小道号移动，1 表示向大道号移动
    cout << "Please input Current Direction (0/1) :";
    cin >> SeekDirection;
    // 输入磁盘请求数，请求道号
    cout << "Please input Request Numbers :";
    cin >> RequestNumber;
    cout << "Please input Request cylinder string :";
    Request = new int[sizeof(int) * RequestNumber];
    Cylinder = new int[sizeof(int) * RequestNumber];
    srand(static_cast<unsigned>(time(0)));
    // for (i = 0; i < RequestNumber; i++)
    // cin >> Request[i];
    for (int i = 0; i < RequestNumber; i++) {
        Request[i] = rand() % 200;
    }
    for (int j = 0; j < RequestNumber; j++) {
        cout << Request[j] << " ";
    }
    cout << endl;
}
DiskArm::~DiskArm() {}
// 初始化道号，寻道记录
void DiskArm::InitSpace(char* MethodName) {
    int i;
    cout << endl << MethodName << endl;
    SeekNumber = 0;
    SeekChang = 0;
    for (i = 0; i < RequestNumber; i++)
        Cylinder[i] = Request[i];
}
// 统计报告算法执行情况
void DiskArm::Report(void) {
    cout << endl;
    cout << "Seek Number: " << SeekNumber << endl;
    cout << "Chang Direction: " << SeekChang << endl;
}
// 先来先服务算法
void DiskArm::Fcfs(void) {
    int Current = CurrentCylinder;
    int Direction = SeekDirection;
    InitSpace("FCFS");
    cout << Current;
    for (int i = 0; i < RequestNumber; i++) {
        if (((Cylinder[i] >= Current) && !Direction) ||
            ((Cylinder[i] < Current) && Direction)) {
            // 需要调头
            SeekChang++;             // 调头数加 1
            Direction = !Direction;  // 改变方向标志
            // 报告当前响应的道号
            cout << endl << Current << " -> " << Cylinder[i];
        } else  // 不需调头，报告当前响应的道号
            cout << " -> " << Cylinder[i];
        // 累计寻道数，响应过的道号变为当前道号
        SeekNumber += abs(Current - Cylinder[i]);
        Current = Cylinder[i];
    }
    // 报告磁盘移臂调度的情况
    Report();
}

void DiskArm::Sstf(void) {
    int Direction = SeekDirection;
    int Current = CurrentCylinder;
    int Shortest;
    int Distance = INT_MAX; 
    InitSpace("SSTF");
    cout << Current;
    for (int i = 0; i < RequestNumber; i++) {
        for (int j = 0; j < RequestNumber; j++) {
            if (Cylinder[j] == -1)
                continue; 
            if (Distance > abs(Current - Cylinder[j])) {
                Distance = abs(Current - Cylinder[j]);
                Shortest = j;
            }
        }
        if (((Cylinder[Shortest] >= Current) && !Direction) ||
            ((Cylinder[Shortest] < Current) && Direction)) {
            SeekChang++;  
            Direction = !Direction; 
       
            cout << endl << Current << " -> " << Cylinder[Shortest];
        } else  
            cout << " -> " << Cylinder[Shortest];
     
        SeekNumber += abs(Current - Cylinder[Shortest]);
        Current = Cylinder[Shortest];
      
        Distance = INT_MAX;
        Cylinder[Shortest] = -1;
    }
    Report();
}

void DiskArm::Scan(void) {
    int Current = CurrentCylinder;
    int Direction = SeekDirection;
    InitSpace("SCAN");
    
    cout << Current;
    
    sort(Cylinder, Cylinder + RequestNumber);
    
    int index = 0;
    while (index < RequestNumber && Cylinder[index] < Current) {
        index++;
    }
    int sum = 0;
    if (Direction == 1) {
        for (int i = index; i < RequestNumber; i++) {
            cout << "->" << Cylinder[i];
            SeekNumber += abs(Current - Cylinder[i]);
            Current = Cylinder[i];
            sum++;
        }
        if (sum != RequestNumber&& Current != MAX_CYLINDER) {
            cout << "->" << MAX_CYLINDER;
            SeekNumber += abs(Current - MAX_CYLINDER);
            Current = MAX_CYLINDER;
            cout << endl;
        }
        if(sum != RequestNumber) {
        SeekChang++;
        Direction = 0;
        }
        for (int i = index-1; i >= 0; i--) {
            cout << "->" << Cylinder[i];
            SeekNumber += abs(Current - Cylinder[i]);
            Current = Cylinder[i];
        }
    } else {
        for (int i = index-1; i >= 0; i--) {
            cout << "->" << Cylinder[i];
            SeekNumber += abs(Current - Cylinder[i]);
            Current = Cylinder[i];
            sum++;
        }
        if (sum != RequestNumber && Current != 0) {
            cout << "->0";
            SeekNumber += abs(Current - 0);
            Current = 0;
            cout << endl;
        }
        if(sum != RequestNumber) {
        SeekChang++;
        Direction = 1;
        }
        for (int i = index; i < RequestNumber; i++) {
            cout << "->" << Cylinder[i];
            SeekNumber += abs(Current - Cylinder[i]);
            Current = Cylinder[i];
        }
    }
    
    Report();
}

void DiskArm::CScan(void) {
    int Current = CurrentCylinder;
    int Direction = SeekDirection;
    InitSpace("C-SCAN");
    
    cout << Current;
    
    sort(Cylinder, Cylinder + RequestNumber);
    
    int index = 0;
    while (index < RequestNumber && Cylinder[index] < Current) {
        index++;
    }
    int sum = 0;
    if (Direction == 1) {
        for (int i = index; i < RequestNumber; i++) {
            cout << "->" << Cylinder[i];
            SeekNumber += abs(Current - Cylinder[i]);
            Current = Cylinder[i];
            sum++;
        }
        if (sum != RequestNumber && Current != MAX_CYLINDER) {
            cout << "->" << MAX_CYLINDER;
            SeekNumber += abs(Current - MAX_CYLINDER);
            Current = MAX_CYLINDER;
            cout << endl;
        }
        if(sum != RequestNumber) {
        cout << "->0";
        SeekNumber += MAX_CYLINDER;
        Current = 0;
        SeekChang++;
        cout << endl;
        }
        for (int i = 0; i < index; i++) {
            cout << "->" << Cylinder[i];
            SeekNumber += abs(Current - Cylinder[i]);
            Current = Cylinder[i];
        }
    } else {
        for (int i = index-1; i >= 0; i--) {
            cout << "->" << Cylinder[i];
            SeekNumber += abs(Current - Cylinder[i]);
            Current = Cylinder[i];
            sum++;
        }
        if (sum != RequestNumber && Current != 0) {
            cout << "->0";
            SeekChang++;
            SeekNumber += abs(Current - 0);
            Current = 0;
            cout << endl;
        }
        if(sum != RequestNumber) {
        cout << "->" << MAX_CYLINDER;
        SeekNumber += MAX_CYLINDER;
        Current = MAX_CYLINDER;
        SeekChang++;
        cout << endl;
        }
        for (int i = RequestNumber-1; i >= index; i--) {
            cout << "->" << Cylinder[i];
            SeekNumber += abs(Current - Cylinder[i]);
            Current = Cylinder[i];
        }
    }
    
    Report();
}

void DiskArm::Look(void) {
    int Current = CurrentCylinder;
    int Direction = SeekDirection;
    InitSpace("LOOK");
    
    cout << Current;
    
    sort(Cylinder, Cylinder + RequestNumber);
    
    int index = 0;
    while (index < RequestNumber && Cylinder[index] < Current) {
        index++;
    }
    int sum = 0;
    if (Direction == 1) {
        for (int i = index; i < RequestNumber; i++) {
            cout << "->" << Cylinder[i];
            SeekNumber += abs(Current - Cylinder[i]);
            Current = Cylinder[i];
            sum++;
        }
        if(sum != RequestNumber) {
            SeekChang++;
            Direction = 0;
            cout << endl;
        }
        
        for (int i = index-1; i >= 0; i--) {
            cout << "->" << Cylinder[i];
            SeekNumber += abs(Current - Cylinder[i]);
            Current = Cylinder[i];
        }
    } else {
        for (int i = index-1; i >= 0; i--) {
            cout << "->" << Cylinder[i];
            SeekNumber += abs(Current - Cylinder[i]);
            Current = Cylinder[i];
            sum++;
        }
        if(sum != RequestNumber) {
            SeekChang++;
            Direction = 1;
            cout << endl;
        }
        
        for (int i = index; i < RequestNumber; i++) {
            cout << "->" << Cylinder[i];
            SeekNumber += abs(Current - Cylinder[i]);
            Current = Cylinder[i];
        }
    }
    
    Report();
}

int main(int argc, char* argv[]) {
    
    DiskArm* dask = new DiskArm();

    dask->Fcfs();
    dask->Sstf();
    dask->Scan();
    dask->CScan();
    dask->Look();
}

