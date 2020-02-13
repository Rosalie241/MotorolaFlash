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

    // clang-format off

    // init bools
    this->deviceReady       = false;
    this->flashReady        = false;
    this->flashing          = false;
    this->dryRun            = false;

    // init workers
    this->fastboot          = new Fastboot();
    this->fastbootThread    = new QThread();
    this->flasher           = new Flasher(this->fastboot);
    this->flasherThread     = new QThread();

    // move the worker classes to threads
    this->fastboot->moveToThread(this->fastbootThread);
    this->flasher->moveToThread(this->flasherThread);

    // required before connecting
    qRegisterMetaType<std::string>("std::string");

    // bind starting function(s)
    connect(this->fastbootThread, SIGNAL(started()), this->fastboot, SLOT(WatchDeviceState()));
    connect(this->flasherThread , SIGNAL(started()), this->flasher , SLOT(Flash()));

    // bind Fastboot events
    connect(this->fastboot, &Fastboot::OnDeviceConnectedChanged, this, &MotorolaFlash::handleDeviceConnectedChange);
    connect(this->fastboot, &Fastboot::OnCallbackReceived      , this, &MotorolaFlash::handleCallbackReceived);
    connect(this->fastboot, &Fastboot::OnStatusCallbackReceived, this, &MotorolaFlash::handleStatusCallbackReceived);

    // bind Flasher events
    connect(this->flasher , &Flasher::OnProgressChanged        , this, &MotorolaFlash::handleProgressChanged);
    connect(this->flasher , &Flasher::OnFinished               , this, &MotorolaFlash::handleFlashingFinished);
    connect(this->flasher , &Flasher::OnStatusUpdate           , this, &MotorolaFlash::handleStatusUpdate);

    // clang-format on

    // init bools in Flasher
    this->flasher->SetDryRun(this->dryRunCheckBox->isEnabled());
    this->flasher->SetRebootAfterFlashing(this->rebootCheckBox->isEnabled());
    this->flasher->SetVerifyFiles(this->verifyCheckBox->isEnabled());

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
        this->logText->append("[libfastboot] OKAY!");
    else
        this->logText->append("[libfastboot] FAIL!");
}

void MotorolaFlash::handleCallbackReceived(const std::string info)
{
    // surpress useless 'is-logical:partition not found'
    if (info.starts_with("is-logical:") && info.ends_with("not found"))
        return;

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

void MotorolaFlash::handleProgressChanged(int percentProgress)
{
    this->progressBar->setValue(percentProgress);
}

void MotorolaFlash::handleStatusUpdate(std::string info)
{
    this->logText->append("[Flasher] " + QString::fromStdString(info));
}

void MotorolaFlash::handleFlashingFinished()
{
    this->flashing = false;
    this->setFlashButton();
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

void MotorolaFlash::on_flashButton_clicked()
{
    this->flashing = true;
    this->setFlashButton();

    // TODO
    if (this->flasherThread->isRunning())
        this->flasherThread->terminate();

    this->flasherThread->start();
}

void MotorolaFlash::on_openButton_clicked()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Open flashfile.xml/servicefile.xml"), "",
                                                tr("flashfile.xml(flashfile.xml);;servicefile.xml(servicefile.xml)"));

    // when the user pressed 'cancel', return
    if (file.isEmpty())
        return;

    this->flashReady = this->flasher->LoadFile(file);

    if (!flashReady)
        this->logText->append("[Flasher] Failed to load \"" + file + "\"");
    else
        this->logText->append("[Flasher] \"" + file + "\"");

    this->setFlashButton();
}

void MotorolaFlash::on_dryRunCheckBox_toggled(bool value)
{
    this->dryRun = value;
    this->flasher->SetDryRun(value);

    this->setFlashButton();
}

void MotorolaFlash::on_rebootCheckBox_toggled(bool value)
{
    this->flasher->SetRebootAfterFlashing(value);
}

void MotorolaFlash::on_verifyCheckBox_toggled(bool value)
{
    this->flasher->SetVerifyFiles(value);
}

void MotorolaFlash::setFlashButton()
{
    // enable flash button when we're ready
    this->flashButton->setEnabled(flashReady && (deviceReady || dryRun) && !flashing);
}