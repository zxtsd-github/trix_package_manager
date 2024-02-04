#!/bin/bash

# 检查参数数量
if [ "$#" -ne 2 ]; then
    echo "用法: $0 <程序目录> <软件包名>"
    exit 1
fi

SOFTWARE_DIR=$1
PACKAGE_NAME=$2
TEMP_DIR="temp_package"
PACKAGE_HEAD="package_head"
PACKAGE_TAIL="package-tail"

# 确保程序目录存在
if [ ! -d "$SOFTWARE_DIR" ]; then
    echo "指定的程序目录不存在：$SOFTWARE_DIR"
    exit 1
fi

# 创建临时目录来构建软件包
mkdir -p "$TEMP_DIR/squashfs-root"

# 复制程序目录到临时构建目录
cp -r "$SOFTWARE_DIR/"* "$TEMP_DIR/squashfs-root/"

# 输入包头信息
echo "请输入包头信息："
read package_head_info
echo "$package_head_info" > "$TEMP_DIR/squashfs-root/$PACKAGE_HEAD"

# 输入包尾信息
echo "请输入包尾信息："
read package_tail_info
echo "$package_tail_info" > "$TEMP_DIR/squashfs-root/$PACKAGE_TAIL"

# 检查 install.sh 脚本是否存在
INSTALL_SCRIPT="$TEMP_DIR/squashfs-root/install.sh"
if [ ! -f "$INSTALL_SCRIPT" ]; then
    echo "找不到 install.sh 安装脚本，将为您创建一个空的 install.sh 文件并打开 vim 编辑器，请完成脚本编写。"
    touch "$INSTALL_SCRIPT"
    vim "$INSTALL_SCRIPT"
fi

# 检查 update.sh 脚本是否存在
UPDATE_SCRIPT="$TEMP_DIR/squashfs-root/update.sh"
if [ ! -f "$UPDATE_SCRIPT" ]; then
    echo "找不到 update.sh 更新脚本，将为您创建一个空的 update.sh 文件并打开 vim 编辑器，请完成脚本编写。"
    touch "$UPDATE_SCRIPT"
    vim "$UPDATE_SCRIPT"
fi

# 使用 mksquashfs 创建软件包
mksquashfs "$TEMP_DIR/squashfs-root" "$PACKAGE_NAME.sfs" -comp xz

# 清理临时目录
rm -rf "$TEMP_DIR"

echo "软件包 $PACKAGE_NAME.sfs 已成功创建。"
