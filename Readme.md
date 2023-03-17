
## Dependency
Json Parser: nlohmann-json, https://github.com/nlohmann/json#cmake \
Boost Library: boost, https://www.boost.org/

## Install Dependency
### Mac
```shell
brew install build-essential
brew install nlohmann-json-dev 
brew install boost
```
### Ubuntu
```shell
sudo yum -y install git
sudo yum -y install cmake
sudo yum groupinstall 'Development Tools'
sudo yum install boost-devel
sudo apt-get install nlohmann-json3-dev
```
## Compile
```shell
cmake .
make
```

## Run
```shell
./moodeMeConsole
```
## Valid Usages:
* search \<criteria>
* next
* rank \<rating>
* cancel
* exit