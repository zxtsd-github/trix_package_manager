#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <complex.h>

#define BUFFER_SIZE 10000

char communicate[1000];

void install(char *package) {
    char buffer[BUFFER_SIZE];
    if (getcwd(buffer, sizeof(buffer)) == NULL) {
        perror("获取运行目录失败");
        exit(EXIT_FAILURE);
    }
    printf("当前运行目录：%s\n", buffer);

    printf("解压包体文件，这可能会花费一部分时间（以root权限）\n");
    char sfs_job[BUFFER_SIZE];
    snprintf(sfs_job, BUFFER_SIZE, "sudo unsquashfs %s %s/temp", package, buffer);
    system(sfs_job);

    char file_cd_job[BUFFER_SIZE];
    snprintf(file_cd_job, BUFFER_SIZE, "cd %s/temp/squashfs-root", buffer);
    system(file_cd_job);

    FILE *package_head_file_read;
    char package_head_read_job[100];
    package_head_file_read = fopen("package_head", "r");
    if (package_head_file_read == NULL) {
        printf("包头未找到，结束\n");
        return;
    }
    fgets(package_head_read_job, 100, package_head_file_read);
    fclose(package_head_file_read);

    char file_cd_2rd_part_job[BUFFER_SIZE];
    snprintf(file_cd_2rd_part_job, BUFFER_SIZE, "cd %s", buffer);
    system(file_cd_2rd_part_job);

    printf("即将开始执行包安装步骤(以root权限)\n");
    char install_job[BUFFER_SIZE];
    snprintf(install_job, BUFFER_SIZE, "sudo bash %s/temp/squashfs-root/install.sh", buffer);
    system(install_job);

    char package_tail_file_path[BUFFER_SIZE];
    snprintf(package_tail_file_path, BUFFER_SIZE, "%s/temp/squashfs-root/package-tail", buffer);
    FILE *package_tail_file_read;
    char package_tail_read_job[100];
    package_tail_file_read = fopen(package_tail_file_path, "r");
    if (package_tail_file_read == NULL) {
        printf("包尾未找到，结束");
        return;
    }
    fgets(package_tail_read_job, 100, package_tail_file_read);
    fclose(package_tail_file_read);

    char cp_uninstall_script_job[BUFFER_SIZE];
    snprintf(cp_uninstall_script_job, BUFFER_SIZE, "bash %s/temp/squashfs-root/uninstall.sh", buffer);
    system(cp_uninstall_script_job);

    char list_file_dir[BUFFER_SIZE];
    snprintf(list_file_dir, BUFFER_SIZE, "%s/installed-list", buffer);
    FILE *LIST_ADD = fopen(list_file_dir, "a+");
    if (LIST_ADD == NULL) {
        printf("找不到包文件集，结束\n");
        return;
    }
    fprintf(LIST_ADD, "\n%s", package_head_read_job);
    fclose(LIST_ADD);

    char add_pkg[BUFFER_SIZE];
    snprintf(add_pkg, BUFFER_SIZE, "%s/pkg_dir/%s", buffer, package_head_read_job);
    FILE *FILE_ADD = fopen(add_pkg, "a+");
    fprintf(FILE_ADD, "%s", package_tail_read_job);
    fclose(FILE_ADD);

    printf("安装完成，进行清理操作（以root权限运行）\n");
    char rm_file_job[BUFFER_SIZE];
    snprintf(rm_file_job, BUFFER_SIZE, "sudo rm -rf %s/temp/*", buffer);
    system(rm_file_job);
}

void uninstall(char *uninstall_package){
    char buffer[BUFFER_SIZE];
    if (getcwd(buffer, sizeof(buffer)) == NULL) {
        perror("获取运行目录失败");
        exit(EXIT_FAILURE);
    }
    printf("当前运行目录：%s\n", buffer);

    char list_file_dir[BUFFER_SIZE];
    snprintf(list_file_dir, BUFFER_SIZE, "%s/pkg_dir/%s", buffer, uninstall_package);
    FILE *list_path = fopen(list_file_dir, "r");
    if (list_path == NULL){
        printf("软件未安装，结束");
        return;
    }
    fclose(list_path);

    char *pkg_path = malloc(sizeof(char) * BUFFER_SIZE);
    if (pkg_path == NULL) {
        printf("内存分配失败\n");
        exit(EXIT_FAILURE);
    }

    FILE *list_file = fopen(list_file_dir, "r");
    if (list_file == NULL) {
        printf("找不到软件包路径，结束\n");
        free(pkg_path);
        return;
    }
    fgets(pkg_path, BUFFER_SIZE, list_file);
    fclose(list_file);

    printf("删除软件操作不可逆，您确定要删除该软件包吗？（y/N）");
    char yN[10];
    scanf("%s", yN);
    if (strcmp(yN, "y") == 0){
        char uninstall_command[BUFFER_SIZE];
        snprintf(uninstall_command, BUFFER_SIZE, "rm -rf %s", pkg_path);
        system(uninstall_command);
    } else if (strcmp(yN, "n") == 0) {
        printf("操作已取消，结束");
    } else {
        printf("输入无效，操作已取消，结束");
    }

    free(pkg_path);
}

void update(char *update_package){
    char buffer[BUFFER_SIZE];
    if (getcwd(buffer, sizeof(buffer)) == NULL) {
        perror("获取运行目录失败");
        exit(EXIT_FAILURE);
    }
    printf("当前运行目录：%s\n", buffer);

    printf("解压包体文件，这可能会花费一部分时间（以root权限）\n");
    char update_sfs_job[BUFFER_SIZE];
    snprintf(update_sfs_job, BUFFER_SIZE, "sudo unsquashfs %s %s/temp", update_package, buffer);
    system(update_sfs_job);

    char update_script_dir[BUFFER_SIZE];
    snprintf(update_script_dir, BUFFER_SIZE, "%s/temp/squashfs-root/update.sh", buffer);
    FILE *update = fopen(update_script_dir, "r");
    if (update == NULL) {
        printf("该软件没有升级脚本，请联系开发者或重新安装\n");
        return;
    }
    fclose(update);

    char update_command[BUFFER_SIZE];
    snprintf(update_command, BUFFER_SIZE, "bash %s/temp/squashfs-root/update.sh", buffer);
    system(update_command);
}

void list(){
    char buffer[BUFFER_SIZE];
    if (getcwd(buffer, sizeof(buffer)) == NULL) {
        perror("获取运行目录失败");
        exit(EXIT_FAILURE);
    }
    printf("当前运行目录：%s\n", buffer);

    char list_file_dir[BUFFER_SIZE];
    snprintf(list_file_dir, BUFFER_SIZE, "%s/installed-list", buffer);
    char list_command[BUFFER_SIZE];
    snprintf(list_command, BUFFER_SIZE, "cat %s", list_file_dir);
    system(list_command);
}

void help(){
    printf("Trix Package Manager\n");
    printf("by zxtsd\n");
    printf("Usage tpm [-i/-u/--list/--help] [包路径/包名/无需参数/无需参数]\n");
    printf("-i  安装软件包\n");
    printf("-u  卸载软件包\n");
    printf("--list  已安装软件列表\n");
    printf("--help 展示该帮助\n");
}

int main(int argc, char *argv[]) {
    if (argc > 2) {
        if (strcmp(argv[1], "-i") == 0) {
            install(argv[2]);
            return 0;
        }else {
            printf("参数错误，结束\n");
            return 127;
        }

        if(strcmp(argv[1], "-u") == 0){
            uninstall(argv[2]);
            return 0;
        }else {
            printf("参数错误，结束\n");
            return 127;
        }

        if (strcmp(argv[1], "--list") == 0) {
            list();
            return 0;    
        }else {
            printf("参数错误，结束\n");
            return 127;
        }
        
        if(strcmp(argv[1], "--help") == 0){
            help();
            return 0;
        }else {
            printf("参数错误，结束\n");
            return 127;
        }
    }
    help();
    return 0;
}