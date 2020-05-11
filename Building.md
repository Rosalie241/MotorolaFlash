# Linux
1) Install qt5 & openssl
2) 
```
./scripts/prepare.sh
./scripts/build.sh
```

# Windows
1) Install [MSYS2](https://www.msys2.org/)
2) Open `mingw32.exe`
3)
```
sudo pacman -S --needed base-devel mingw-w64-i686-clang mingw-w64-i686-openssl mingw-w64-i686-qt5 mingw-w64-i686-cmake
./scripts/prepare.sh
./scripts/publish_windows.sh
```