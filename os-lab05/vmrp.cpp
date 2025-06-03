#include "vmrp.h" 

Replace::Replace() {
  int i;
  cout << "Please input page numbers :";
  cin >> PageNumber;
  ReferencePage = new int[PageNumber]; 
  EliminatePage = new int[PageNumber]; 
  cout << "Please input reference page string :";
  for (i = 0; i < PageNumber; i++)
    cin >> ReferencePage[i]; 
  cout << "Please input page frames :";
  cin >> FrameNumber;
  PageFrames = new int[FrameNumber]; 
}

Replace::~Replace() {
  delete[] ReferencePage;
  delete[] EliminatePage;
  delete[] PageFrames;
}

void Replace::InitSpace(const char* MethodName) {
  int i;
  cout << endl << MethodName << endl;
  FaultNumber = 0; 
  for (i = 0; i < PageNumber; i++)
    EliminatePage[i] = -1;
  for (i = 0; i < FrameNumber; i++)
    PageFrames[i] = -1; 
}

void Replace::Report(void) {
  cout << endl << "Eliminate page:";
  for (int i = 0; EliminatePage[i] != -1; i++)
    cout << EliminatePage[i] << " ";
  cout << endl << "Number of page faults = " << FaultNumber << endl;
  cout << setw(6) << setprecision(3); 
  cout << "Rate of page faults = "
<< 100 * (float)FaultNumber / (float)PageNumber << "%" << endl; 
}

void Replace::Lru(void) {
  int i, j, k, l, next;
  InitSpace("LRU");
  for (k = 0, l = 0; k < PageNumber; k++) {
    next = ReferencePage[k];
    // 检测引用页当前是否已在实存
    for (i = 0; i < FrameNumber; i++) {
      if (next == PageFrames[i]) {
        // 引用页已在实存将其调整到页记录栈顶
        next = PageFrames[i];
        for (j = i; j > 0; j--)
          PageFrames[j] = PageFrames[j - 1];
        PageFrames[0] = next;
        break;
      }
    }
    if (PageFrames[0] == next) {
      // 如果引用页已放栈顶，则为不缺页，报告当前内存页号
      for (j = 0; j < FrameNumber; j++)
        if (PageFrames[j] >= 0)
          cout << PageFrames[j] << " ";
      cout << endl;
      continue;
    } else
      // 如果引用页还未放栈顶，则为缺页，缺页数加1
      FaultNumber++;
    // 栈底页号记入淘汰页数组中
    EliminatePage[l] = PageFrames[FrameNumber - 1];
    // 向下压栈
    for (j = FrameNumber - 1; j > 0; j--)
      PageFrames[j] = PageFrames[j - 1];
    PageFrames[0] = next; // 引用页放栈顶
    // 报告当前实存中页号
    for (j = 0; j < FrameNumber; j++)
      if (PageFrames[j] >= 0)
        cout << PageFrames[j] << " ";
    // 报告当前淘汰的页号
    if (EliminatePage[l] >= 0)
      cout << "->" << EliminatePage[l++] << endl;
    else
      cout << endl;
  }
  Report(); }

void Replace::Fifo(void) {
  int i, j, k, l, next;
  InitSpace("FIFO");
  // 循环装入引用页
  for (k = 0, j = l = 0; k < PageNumber; k++) {
    next = ReferencePage[k];
    // 如果引用页已在实存中，报告实存页号
    for (i = 0; i < FrameNumber; i++)
      if (next == PageFrames[i])
        break;
    if (i < FrameNumber) {
      for (i = 0; i < FrameNumber; i++)
        cout << PageFrames[i] << " ";
      cout << endl;
      continue;
    }
    // 引用页不在实存中，缺页数加1
    FaultNumber++;
    EliminatePage[l] = PageFrames[j];
    PageFrames[j] = next; // 引用页号放最先入页号处
    j = (j + 1) % FrameNumber; // 最先入页号循环下移
    // 报告当前实存页号和淘汰页号
    for (i = 0; i < FrameNumber; i++)
      if (PageFrames[i] >= 0)
        cout << PageFrames[i] << " ";
    if (EliminatePage[l] >= 0)
      cout << "->" << EliminatePage[l++] << endl;
    else
      cout << endl;
  }
  Report();
}

void Replace::Clock(void) {
  InitSpace("Clock");
  int pointer = 0;
  bool* used = new bool[FrameNumber];
  int eliminateIndex = 0;

  for (int i = 0; i < FrameNumber; i++) {
    used[i] = false;
  }

  for (int k = 0; k < PageNumber; k++) {
    int next = ReferencePage[k];
    bool found = false;

    for (int i = 0; i < FrameNumber; i++) {
      if (PageFrames[i] == next) {
        found = true;
        used[i] = true;
        break;
      }
    }

    if (!found) {
      FaultNumber++;
      while (used[pointer]) {
        used[pointer] = false;
        pointer = (pointer + 1) % FrameNumber;
      }

      if (PageFrames[pointer] != -1) {
        EliminatePage[eliminateIndex++] = PageFrames[pointer];
      }

      PageFrames[pointer] = next;
      used[pointer] = true;
      pointer = (pointer + 1) % FrameNumber;
    }

    for (int j = 0; j < FrameNumber; j++) {
      if (PageFrames[j] >= 0)
        cout << PageFrames[j] << " ";
    }
    if (!found && eliminateIndex > 0 &&
      EliminatePage[eliminateIndex - 1] > 0)
      cout << "->" << EliminatePage[eliminateIndex - 1] << endl;
    else
      cout << endl;
  }

  delete[] used;
  Report();
}

void Replace::Eclock(void) {
  InitSpace("EClock");
  int pointer = 0;
  bool* used = new bool[FrameNumber];
  bool* modified = new bool[FrameNumber];
  int eliminateIndex = 0;

  for (int i = 0; i < FrameNumber; i++) {
    used[i] = false;
    modified[i] = false;
  }

  for (int k = 0; k < PageNumber; k++) {
    int next = ReferencePage[k];
    bool found = false;

    for (int i = 0; i < FrameNumber; i++) {
      if (PageFrames[i] == next) {
        found = true;
        used[i] = true;
        modified[i] = true;
        break;
      }
    }

    if (!found) {
      FaultNumber++;
      while (used[pointer] || modified[pointer]) {
        if (used[pointer]) {
          used[pointer] = false;
        } 
        else if (modified[pointer]) {
          modified[pointer] = false;
          used[pointer] = true;
        }
        pointer = (pointer + 1) % FrameNumber;
      }


      if (PageFrames[pointer] != -1) {
        EliminatePage[eliminateIndex++] = PageFrames[pointer];
      }

      PageFrames[pointer] = next;
      used[pointer] = true;
      modified[pointer] = false;
      pointer = (pointer + 1) % FrameNumber;
    }

    for (int j = 0; j < FrameNumber; j++) {
      if (PageFrames[j] >= 0)
        cout << PageFrames[j] << " ";
    }
    if (!found && eliminateIndex > 0 &&
      EliminatePage[eliminateIndex - 1] > 0)
      cout << "->" << EliminatePage[eliminateIndex - 1] << endl;
    else
      cout << endl;
  }

  delete[] used;
  delete[] modified;
  Report();
}


void Replace::Lfu(void) {
  InitSpace("LFU");
  int* frequency = new int[FrameNumber];
  int eliminateIndex = 0;

  for (int i = 0; i < FrameNumber; i++) {
    frequency[i] = 0;
  }

  for (int k = 0; k < PageNumber; k++) {
    int next = ReferencePage[k];
    bool found = false;

    for (int i = 0; i < FrameNumber; i++) {
      if (PageFrames[i] == next) {
        found = true;
        frequency[i]++;
        break;
      }
    }

    if (!found) {
      FaultNumber++;
      int minFreqIndex = 0;
      for (int i = 1; i < FrameNumber; i++) {
        if (frequency[i] < frequency[minFreqIndex]) {
          minFreqIndex = i;
        }
      }

      if (PageFrames[minFreqIndex] != -1) {
        EliminatePage[eliminateIndex++] = PageFrames[minFreqIndex];
      }

      PageFrames[minFreqIndex] = next;
      frequency[minFreqIndex] = 1;
    }

    for (int j = 0; j < FrameNumber; j++) {
      if (PageFrames[j] >= 0)
        cout << PageFrames[j] << " ";
    }
    if (!found && eliminateIndex > 0 &&
      EliminatePage[eliminateIndex - 1] > 0)
      cout << "->" << EliminatePage[eliminateIndex - 1] << endl;
    else
      cout << endl;
  }

  delete[] frequency;
  Report();
}

void Replace::Mfu(void) {
  InitSpace("MFU");
  int* frequency = new int[FrameNumber];
  int* lastUsed = new int[FrameNumber]; 
  int eliminateIndex = 0;

  // 初始化频率和使用时间
  for (int i = 0; i < FrameNumber; i++) {
    frequency[i] = 0;
    lastUsed[i] = -1; // -1 表示页帧空闲
  }

  for (int k = 0; k < PageNumber; k++) {
    int next = ReferencePage[k];
    bool found = false;
    int freeFrame = -1; // 跟踪空闲页帧

 
    for (int i = 0; i < FrameNumber; i++) {
      if (PageFrames[i] == next) {
        found = true;
        frequency[i]++;
        lastUsed[i] = k;
        break;
      }

      if (PageFrames[i] == -1 && freeFrame == -1) {
        freeFrame = i;
      }
    }

    if (!found) {
      FaultNumber++;
      

      if (freeFrame != -1) {
        PageFrames[freeFrame] = next;
        frequency[freeFrame] = 1;
        lastUsed[freeFrame] = k;
      }

      else {
        int maxFreqIndex = 0;
        // 找到频率最高的页帧（频率相同时选最久未使用的）
        for (int i = 1; i < FrameNumber; i++) {
          if (frequency[i] > frequency[maxFreqIndex] || 
             (frequency[i] == frequency[maxFreqIndex] && 
              lastUsed[i] < lastUsed[maxFreqIndex])) {
            maxFreqIndex = i;
          }
        }

 
        EliminatePage[eliminateIndex] = PageFrames[maxFreqIndex];

        PageFrames[maxFreqIndex] = next;
        frequency[maxFreqIndex] = 1;
        lastUsed[maxFreqIndex] = k;
      }
    }

    for (int j = 0; j < FrameNumber; j++) {
      if (PageFrames[j] >= 0)
        cout << PageFrames[j] << " ";
    }
    

    if (!found && EliminatePage[eliminateIndex] >= 0) {
      cout << "->" << EliminatePage[eliminateIndex];
      eliminateIndex++;
    }
    cout << endl;
  }

  delete[] frequency;
  delete[] lastUsed;
  Report();
}


int main(int argc, char* argv[]) {
  Replace* vmpr = new Replace();
  vmpr->Fifo();
  vmpr->Lru();
  vmpr->Clock();
  vmpr->Eclock();
  vmpr->Lfu();
  vmpr->Mfu();

  delete vmpr;
  return 0;
}
