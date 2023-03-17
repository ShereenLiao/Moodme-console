
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
### Linux
```shell
apt-get install build-essential;
apt-get install libboost-all-dev;
apt-get install -y nlohmann-json-dev;
```
## Compile
```shell
cmake .
make install
```

## Run
```shell
./console
```
Valid Usages:
* search \<criteria>
* next
* rank \<rating>
* cancel
* exit