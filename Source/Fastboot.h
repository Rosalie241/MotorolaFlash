/*
 MotorolaFlash - https://github.com/tim241/MotorolaFlash
 Copyright (C) 2020 Tim Wanders <tim241@mailbox.org>
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef FASTBOOT_H
#define FASTBOOT_H

#include <fastboot/fastboot_driver.h> // fastboot::FastBootDriver
#include <fastboot/usb.h>             // usb_open

#include <fcntl.h>    // open
#include <filesystem> // std::filesystem
#include <thread>     // std::this_thread

#include <QObject> // QObject

class Fastboot : public QObject
{
    Q_OBJECT

    // clang-format off
  private:
    Transport *transport                  = nullptr;
    fastboot::FastBootDriver driver       = NULL;
    int64_t downloadLimit                 = -1;
    int slotCount                         = -1;
    std::string currentSlot               = "";
    fastboot::DriverCallbacks *callbacks  = nullptr;

    static int UsbOpenCallback(usb_ifc_info *);
    void WaitForDeviceConnect();
    void WaitForDeviceDisConnect();

    // clang-format on
  public:
    Fastboot();
    ~Fastboot();
    bool Flash(std::string fileName, std::string partition);
    bool Erase(std::string partition);
    bool Oem(std::string command);
    bool GetVar(std::string key, std::string *val);
    bool Reboot();

  public slots:
    void WatchDeviceState();

  signals:
    void OnCallbackReceived(const std::string);
    void OnStatusCallbackReceived(int);
    void OnDeviceConnectedChanged(bool);
};

#endif // FASTBOOT_H