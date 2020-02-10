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

#ifndef WINDOWFORM_H
#define WINDOWFORM_H

#include "Fastboot.h"         // Fastboot
#include "Flasher.h"          // Flasher
#include "ui_MotorolaFlash.h" // Ui::MotorolaFlash

#include <QCloseEvent> // QCloseEvent
#include <QFileDialog> // QFileDialog
#include <QMessageBox> // QMessageBox
#include <QThread>     // QThread

class MotorolaFlash : public QMainWindow, private Ui::MotorolaFlash
{
    Q_OBJECT

  private:
    // vars
    Flasher *flasher;
    Fastboot *fastboot;
    QThread *fastbootThread;
    QThread *flasherThread;
    bool flashing;
    bool flashReady;
    bool deviceReady;
    bool dryRun;

    // functions
    void setFlashButton();

  public:
    MotorolaFlash(QMainWindow *);
    ~MotorolaFlash();

  private slots:
    // UI event handlers
    void on_flashButton_clicked();
    void on_openButton_clicked();
    void on_dryRunCheckBox_toggled(bool);
    void on_rebootCheckBox_toggled(bool);
    void closeEvent(QCloseEvent *);

    // Fastboot event handlers
    void handleDeviceConnectedChange(bool);
    void handleCallbackReceived(const std::string);
    void handleStatusCallbackReceived(int);

    // Flasher event handlers
    void handleProgressChanged(int);
    void handleFlashingFinished();
};

#endif // WINDOWFORM_H