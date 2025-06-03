#!/bin/bash

n=12

random_pages=()
for (( i=0; i<n; i++ ))
do
    random_pages+=($((RANDOM % 25 + 1)))
done

echo "生成随机内存引用串..."
echo "总访问页数 n: $n"
echo "生成的随机页面引用串: ${random_pages[@]}"

echo "执行 vmrp 程序..."
./vmrp <<EOF
$n
${random_pages[@]}
4
EOF

echo "脚本执行完毕。"