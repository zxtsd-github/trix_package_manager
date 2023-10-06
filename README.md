# trix_package_manager
The package manager for TrixOS 

### Trix package manager (TPM) is a package manager for TrixOS

## In the testing,TPM has a lot of problem , and it almost haven't any package ,please use it be careful
## It probably very unstable！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！

# How to build TPM
#### You just need enter <code> make main.c </code> to build it or use <code> gcc -o tpm main.c <code> to build TPM

# How  to install 
### You need been root
#### 1.<code> mkdir /opt/tpm </code>
#### 2.<code> cp /path/to/tpm /opt/tpm </code>
#### 3.<code> mkdir /opt/tpm/cache && touch package.list && touch temp_package.list </code>
#### 4.<code> ln -sf /opt/tpm/tpm /usr/share/bin </code>

# How to use it ?
## tpm [-i/-u/-ug/--list] [package_path] [install_path]
