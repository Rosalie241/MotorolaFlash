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

#include "MotorolaFlash.h"

MotorolaFlash::MotorolaFlash(QMainWindow *parent) : QMainWindow(parent)
{
    this->setupUi(this);

    // disable resizing (for now?)
    this->setMaximumSize(this->size());
    this->setMinimumSize(this->size());

    // init bools
    this->deviceReady = false;
    this->flashReady = false;
    this->flashing = false;
    this->dryRun = false;

    fastboot = new Fastboot();
    fastbootThread = new QThread();

    // start the work
    fastboot->moveToThread(fastbootThread);

    // bind starting function
    connect(fastbootThread, SIGNAL(started()), fastboot, SLOT(WatchDeviceState()));

    // bind Fastboot events
    connect(fastboot, &Fastboot::OnDeviceConnectedChanged, this, &MotorolaFlash::handleDeviceConnectedChange);
    connect(fastboot, &Fastboot::OnCallbackReceived, this, &MotorolaFlash::handleCallbackReceived);
    connect(fastboot, &Fastboot::OnStatusCallbackReceived, this, &MotorolaFlash::handleStatusCallbackReceived);

    // connect(wf, &WaitForFastboot::OnDeviceAvailable, this, &MotorolaFlash::deviceReady);
    // Take care of cleaning up when finished too
    // connect(fastboot, SIGNAL(onReady()), wait_for_device_thread, SLOT(quit()));
    // connect(fastboot, SIGNAL(onReady()), fastboot, SLOT(deleteLater()));
    // connect(wait_for_device_thread, SIGNAL(finished()), this, SLOT(deviceReady()));

    // start Fastboot thread
    fastbootThread->start();
}

MotorolaFlash::~MotorolaFlash()
{
    if (fastbootThread->isRunning())
        fastbootThread->terminate();
}

void MotorolaFlash::handleStatusCallbackReceived(int status)
{
    if (status == 0)
        this->logText->append("[libfastboot] OK!");
    else
        this->logText->append("[libfastboot] FAIL!");
}

void MotorolaFlash::handleCallbackReceived(const std::string &info)
{
    this->logText->append("[libfastboot] " + QString::fromStdString(info));
}

void MotorolaFlash::handleDeviceConnectedChange(bool connected)
{
    deviceReady = connected;

    this->setFlashButton();

    if (connected)
        this->logText->append("[Fastboot] Device connected!");
    else
        this->logText->append("[Fastboot] Device disconnected!");
}

void MotorolaFlash::closeEvent(QCloseEvent *event)
{
    // when we're flashing,
    // show an error and ignore the event
    if (this->flashing)
    {
        event->ignore();

        QMessageBox errBox;
        errBox.setWindowTitle("Error!");
        errBox.setText("Flashing in progress!");
        errBox.setIcon(QMessageBox::Icon::Critical);
        errBox.setDefaultButton(QMessageBox::StandardButton::Ok);
        errBox.exec();
    }
    else
    {
        event->accept();

        // actually quit the application
        QGuiApplication::quit();
    }
}

#include <iostream>

void MotorolaFlash::on_flashButton_clicked()
{
    if (dryRun)
        return;

    this->fastboot->Flash("boot.img", "boot");
}

void MotorolaFlash::on_openButton_clicked()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Open flashfile.xml"), "", tr("flashfile.xml(flashfile.xml)"));

    // when the user pressed 'cancel', return
    if (file.isEmpty())
        return;

    this->flashReady = this->flasher.LoadFile(file);

    if (!flashReady)
        this->logText->append("[Flasher] Failed to load \"" + file + "\"");
    else
        this->logText->append("[Flasher] \"" + file + "\"");

    this->setFlashButton();
}

void MotorolaFlash::on_dryRunCheckBox_toggled(bool value)
{
    this->dryRun = value;

    this->setFlashButton();
}

void MotorolaFlash::setFlashButton()
{
    // enable flash button when we're ready
    this->flashButton->setEnabled(flashReady && (deviceReady || dryRun));
}