#!/bin/bash

#编译
gcc -o tpm main.c

# 1. 创建目录
sudo mkdir -p /usr/local/tpm

# 2. 安装可执行文件
sudo cp tpm /usr/local/tpm/

# 3. 安装其他依赖文件
sudo mkdir -p /usr/local/tpm/pkg_dir
sudo touch /usr/local/tpm/installed-list

# 4. 创建符号链接
sudo ln -s /usr/local/tpm/tpm /usr/bin/tpm

# 5. 提供使用说明
echo "Trix Package Manager 安装成功！"
echo "使用方法："
echo "tpm -i 包路径：安装软件包"
echo "tpm -u 包名：卸载软件包"
echo "tpm --list：查看已安装的软件列表"
echo "tpm --help：查看帮助信息"
