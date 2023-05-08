#!/bin/sh
# Add your startup script
# echo $FLAG > /home/ctf/flag && export FLAG='' # DO NOT DELETE
# 获取FLAG环境变量
# 计算FLAG变量的长度
flag_len=$(expr length "$FLAG")

# 计算中间位置
mid=$(($flag_len / 2))

# 将FLAG变量分成两半并分别赋值给flag1和flag2
flag1=$(echo "$FLAG" | cut -c 1-$mid)
flag2=$(echo "$FLAG" | cut -c $(($mid+1))-$flag_len)

export FLAG=''
# 打印分别存储在flag1与flag2中的值
# echo $flag1
# echo $flag2
echo $flag1 > flag1
echo $flag2 > flag2

/etc/init.d/xinetd start;
sleep infinity;
