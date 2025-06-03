import matplotlib.pyplot as plt

# 页面访问序列，用于模拟内存请求
reference_sequence = [1,2,3,4,1,2,5,1,2,3,4,5]
# 模拟的内存帧数量范围
frame_counts = [ 2, 3,4]

# 存储不同帧数下的总缺页次数
total_page_faults_per_frame = []

# 遍历每种帧数配置
for num_frames_current in frame_counts:
    current_pages_in_memory = []  # 模拟当前内存中的页面
    fault_count = 0  # 当前帧数下的缺页计数

    print(f"\n--- 模拟帧数: {num_frames_current} ---")
    # 遍历每个页面请求
    for page_request in reference_sequence:
        print(f"请求页面: {page_request}, 当前内存: {current_pages_in_memory}", end=" -> ")

        # 检查页面是否已在内存中（命中或缺页）
        if page_request not in current_pages_in_memory:
            fault_count += 1  # 缺页，增加缺页计数

            # 如果内存未满，直接加入新页面
            if len(current_pages_in_memory) < num_frames_current:
                current_pages_in_memory.append(page_request)
                print(f"缺页，页面 {page_request} 加入，内存: {current_pages_in_memory}")
            # 如果内存已满，根据FIFO规则替换页面
            else:
                current_pages_in_memory.pop(0)  # 移除最老的页面（列表第一个元素）
                current_pages_in_memory.append(page_request)  # 加入新页面
                print(f"缺页，页面 {page_request} 加入，内存: {current_pages_in_memory}")
        else:
            print(f"命中，内存: {current_pages_in_memory}")  # 页面命中

    total_page_faults_per_frame.append(fault_count)  # 记录当前帧数下的总缺页数
    print(f"帧数 {num_frames_current} 下的缺页总数: {fault_count}")

# 绘图
plt.figure(figsize=(10, 6))
plt.plot(frame_counts, total_page_faults_per_frame, marker='o', linestyle='-', color='b', label='FIFO Page Faults')

plt.title('FIFO Page Replacement Algorithm: Belady Anomaly Demonstration')
plt.xlabel('Number of Memory Frames')
plt.ylabel('Total Page Faults')
plt.grid(True, linestyle='--', alpha=0.7)
plt.xticks(frame_counts)
plt.legend()
plt.show()

print("\n--- 结果汇总 ---")
for i in range(len(frame_counts)):
    print(f"内存帧数: {frame_counts[i]}, 缺页次数: {total_page_faults_per_frame[i]}")
