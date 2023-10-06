#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define MAX_PATH_LENGTH 100

void install(const char* package_path, const char* install_path) {
    // 检查参数是否为空
    if (package_path == NULL || install_path == NULL) {
        printf("Invalid package path or install path.\n");
        return;
    }

    // 检查安装路径是否存在，若不存在则创建目录
    struct stat st;
    if (stat(install_path, &st) == -1) {
        if (mkdir(install_path, 0755) == -1) {
            printf("Failed to create install directory.\n");
            return;
        }
    }

    // 使用系统命令复制文件
    char copy_command[MAX_PATH_LENGTH * 2 + 20];
    sprintf(copy_command, "cp %s %s", package_path, install_path);
    system(copy_command);

    // 将包名和安装路径写入包列表文件
    FILE* fp;
    char package_list_path[MAX_PATH_LENGTH + 20];
    sprintf(package_list_path, "/opt/tpm/package.list");

    fp = fopen(package_list_path, "a");
    if (fp == NULL) {
        printf("Failed to open package list file.\n");
        return;
    }

    fprintf(fp, "%s,%s\n", package_path, install_path);

    fclose(fp);

    printf("Installation succeeded.\n");
}

void uninstall(const char* package_name, const char* install_path) {
    // 检查参数是否为空
    if (package_name == NULL || install_path == NULL) {
        printf("Invalid package name or install path.\n");
        return;
    }

    // 删除安装目录中的文件
    char remove_command[MAX_PATH_LENGTH + 20];
    sprintf(remove_command, "rm -rf %s/%s", install_path, package_name);
    system(remove_command);

    // 从包列表中移除该包名和安装路径
    FILE* fp;
    char package_list_path[MAX_PATH_LENGTH + 20];
    sprintf(package_list_path, "/opt/tpm/package.list");

    fp = fopen(package_list_path, "r");
    if (fp == NULL) {
        printf("Failed to open package list file.\n");
        return;
    }

    // 创建临时文件
    FILE* temp_fp;
    char temp_package_list_path[MAX_PATH_LENGTH + 20];
    sprintf(temp_package_list_path, "/opt/tpm/temp_package.list");

    temp_fp = fopen(temp_package_list_path, "w");
    if (temp_fp == NULL) {
        printf("Failed to create temporary package list file.\n");
        fclose(fp);
        return;
    }

    char line[MAX_PATH_LENGTH + 1];

    // 逐行读取包列表文件并写入临时文件
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = '\0'; // 移除换行符

        char stored_package_name[MAX_PATH_LENGTH + 1];
        char stored_install_path[MAX_PATH_LENGTH + 1];

        sscanf(line, "%[^,],%s", stored_package_name, stored_install_path);

        if (strcmp(stored_package_name, package_name) != 0 || strcmp(stored_install_path, install_path) != 0) {
            fprintf(temp_fp, "%s\n", line);
        }
    }

    fclose(fp);
    fclose(temp_fp);

    // 删除旧的包列表文件
    remove(package_list_path);

    // 将临时文件重命名为包列表文件
    rename(temp_package_list_path, package_list_path);

    printf("Uninstallation succeeded.\n");
}

void upgrade(const char* package_path, const char* install_path) {
    // 首先运行卸载函数
    uninstall(package_path, install_path);

    // 然后运行安装函数
    install(package_path, install_path);
}

void list() {
    FILE* fp;
    char package_list_path[MAX_PATH_LENGTH + 20];
    sprintf(package_list_path, "/opt/tpm/package.list");

    fp = fopen(package_list_path, "r");
    if (fp == NULL) {
        printf("Failed to open package list file.\n");
        return;
    }

    char line[MAX_PATH_LENGTH + MAX_PATH_LENGTH + 3];

    // 逐行读取包列表文件并解析打印
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = '\0'; // 移除换行符

        char package_path[MAX_PATH_LENGTH + 1];
        char install_path[MAX_PATH_LENGTH + 1];

        sscanf(line, "%[^,],%s", package_path, install_path);

        printf("Package: %s\nInstall Path: %s\n\n", package_path, install_path);
    }

    fclose(fp);
}
    void ver(){
		printf("ver 1.0\n  Code and build by zxtsd \n License MIT\n");
        return 0;
    }


int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("You should provide the package path and install path as arguments.\n");
        return 124;
    }

    if (strcmp(argv[1], "-i") == 0) {
        const char* package_path = argv[2];
        const char* install_path = argv[3];
        install(package_path, install_path);
    }
    else if (strcmp(argv[1], "-u") == 0) {
        const char* package_name = argv[2];
        const char* install_path = argv[3];
        uninstall(package_name, install_path);
    }
    else if (strcmp(argv[1], "-ug") == 0) {
        const char* package_path = argv[2];
        const char* install_path = argv[3];
        upgrade(package_path, install_path);
    }
    else if (strcmp(argv[1], "--list") == 0) {
        list();
    }
    else if (strcmp(argv[1],"-v") == 0) {
        ver();
    }
    else {
        printf("Invalid command.\n");
        return 126;
    }

    return 0;
}
