# Linux
1) Install qt5 & openssl
2) 
```
./prepare.sh
./build.sh
```

# Windows
1) Install [MSYS2](https://www.msys2.org/)
2) Open `mingw32.exe`
3)
```
sudo pacman -S --needed base-devel mingw-w64-i686-clang mingw-w64-i686-openssl mingw-w64-i686-qt5 mingw-w64-i686-cmake
./prepare.sh
./build_msys2.sh
```