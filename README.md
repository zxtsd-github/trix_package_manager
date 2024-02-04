# Trix Package Manager (TPM)

## Description
TPM is a package manager designed for Trix OS and other distributions. It is made by a middle school student, so it may be unstable.

## Package Format
TPM uses squashfs to build packages, and the package extension is ".tpm". To build a package, you need to write an `install.sh` script and create a `package-head` file to save the package name. Additionally, you should write a `package-tail` file to save the package path.

## Installation
1. Build `main.c` to an executable binary file:
    ```
    gcc -o tpm main.c
    ```
2. Put `tpm` somewhere that you can remember.
3. Create a symbolic link to `/usr/bin`:
    ```
    ln -sf /path/to/tpm /usr/bin
    ```

## Usage
Type `tpm --help` to see the available commands.
