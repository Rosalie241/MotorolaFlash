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

#include "Fastboot.h"

Fastboot::Fastboot()
{

    callbacks = new fastboot::DriverCallbacks();

    std::function<void(const std::string a)> callBackFunc(
        std::bind(&Fastboot::OnCallbackReceived, this, std::placeholders::_1));

    std::function<void(int)> callBackFunc2(std::bind(&Fastboot::OnStatusCallbackReceived, this, std::placeholders::_1));

    callbacks->info = callBackFunc;
    callbacks->prolog = callBackFunc;
    callbacks->epilog = callBackFunc2;
}

Fastboot::~Fastboot()
{
    if (transport)
        transport->Close();
}

void Fastboot::WaitForDeviceConnect()
{
    while (!this->transport)
    {
        this->transport = usb_open(UsbOpenCallback);

        // we should only sleep when transport isn't defined
        if (!this->transport)
            std::this_thread::sleep_for(std::chrono::microseconds(50));
    }

    this->driver = fastboot::FastBootDriver(transport, *callbacks);

    // extract max download size
    std::string downloadLimitString;
    fastboot::RetCode ret = this->driver.GetVar("max-download-size", &downloadLimitString);

    if (ret == fastboot::RetCode::SUCCESS)
        this->downloadLimit = std::stol(downloadLimitString);
    else
        this->downloadLimit = -1;

    // extract slot-count
    std::string slotCountString;
    ret = this->driver.GetVar("slot-count", &slotCountString);

    if (ret == fastboot::RetCode::SUCCESS)
        this->slotCount = std::stol(slotCountString);
    else
        this->slotCount = -1;

    // extract current-slot
    if (this->driver.GetVar("current-slot", &this->currentSlot) != fastboot::RetCode::SUCCESS)
        this->currentSlot = "";

    emit this->OnDeviceConnectedChanged(true);
}

void Fastboot::WaitForDeviceDisConnect()
{
    // TODO: test on windows
    while (this->transport->WaitForDisconnect() == -1)
        std::this_thread::sleep_for(std::chrono::microseconds(50));

    // reset transport
    this->transport = nullptr;

    emit this->OnDeviceConnectedChanged(false);
}

void Fastboot::WatchDeviceState()
{
    while (true)
    {
        WaitForDeviceConnect();
        WaitForDeviceDisConnect();
    }
}

int Fastboot::UsbOpenCallback(usb_ifc_info *info)
{
    // stolen from android_system_core/fastboot/fastboot.cpp
    return (info->ifc_class != 0xff || info->ifc_subclass != 0x42 || info->ifc_protocol != 0x03);
}

bool Fastboot::Flash(std::string fileName, std::string partition)
{
    bool failed = true;
    bool isSparse = false;
    int64_t imageSize = -1;

    // failed to extract download limit
    if (this->downloadLimit == -1)
        return false;

#if defined(_WIN32)
    int fd = open(fileName.c_str(), O_RDONLY | O_BINARY);
#else
    int fd = open(fileName.c_str(), O_RDONLY);
#endif

    if (fd == -1)
        return false;

    imageSize = std::filesystem::file_size(fileName);

    // only check for slot when it's an AB device
    if (slotCount >= 2)
    {
        // check if partition has ab slot
        std::string hasSlot;

        fastboot::RetCode ret = this->driver.GetVar("has-slot:" + partition, &hasSlot);

        if (ret != fastboot::RetCode::SUCCESS)
        {
            close(fd);
            return false;
        }

        // if it has, append current slot to partition name
        if (hasSlot == "yes")
            partition += "_" + this->currentSlot;
    }

    // if the partition is logical, resize on device
    std::string isLogicalString;
    bool isLogical = this->driver.GetVar("is-logical:" + partition, &isLogicalString) == fastboot::RetCode::SUCCESS &&
                     isLogicalString == "yes";
    if (isLogical)
        this->driver.ResizePartition(partition, std::to_string(imageSize));

    bool ret = this->driver.FlashPartition(partition, fd, imageSize) == fastboot::RetCode::SUCCESS;

    // show error when not successful
    if (!ret)
        emit this->OnCallbackReceived(this->driver.Error());

    close(fd);
    return ret;
}

bool Fastboot::Reboot()
{
    return this->driver.Reboot() == fastboot::RetCode::SUCCESS;
}
