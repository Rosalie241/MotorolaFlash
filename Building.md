# Linux
1) Install qt5 & openssl
2) 
```
./scripts/prepare.sh
./scripts/build.sh
```

# Windows
1) Install [MSYS2](https://www.msys2.org/)

## 32bit
2) Open `mingw32.exe`
3)
```
pacman -S --needed base-devel mingw-w64-i686-clang mingw-w64-i686-openssl mingw-w64-i686-qt5 mingw-w64-i686-cmake
./scripts/prepare.sh
./scripts/publish_windows.sh x86
```

## 64bit
2) Open `mingw64.exe`
3)
```
pacman -S --needed base-devel mingw-w64-x86_64-clang mingw-w64-x86_64-openssl mingw-w64-x86_64-qt5 mingw-w64-x86_64-cmake
./scripts/prepare.sh
./scripts/publish_windows.sh 
```
