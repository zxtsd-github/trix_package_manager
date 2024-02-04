# Trix Package Manager(TPM)

#### A Package manager for trix os or other distrobution
### It's made by a student who in the middle school，so it maybe very unstable 

## Package format
#### Trix Package Manager use squashfs to build package ,and the package extension is named "tpm"
#### To bulid package you need to write a <code> install.sh </code> to and write a <code> package-head </code> file to save package name,also you need to write a <code> package-tail </code> to save package path

## Insatll 
#### 1.build main.c to excutable binary file 
####   <code> gcc -o tpm main.c </code>
#### 2.put tpm to somewhere that you can remember
#### 3.<code> ln -sf /path/to/tpm /usr/bin </code>

## Usage
#### just type tpm --help
