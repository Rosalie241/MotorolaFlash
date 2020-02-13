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

#ifndef FLASHER_H
#define FLASHER_H

#include "Fastboot.h"     // Fastboot
#include "FlashingStep.h" // FlashingStep

#include <QCryptographicHash> // QCryptographicHash
#include <QDir>               // QDir
#include <QFileInfo>          // QFileInfo
#include <QObject>            // QObject
#include <QString>            // QString
#include <cstring>            // std::strcmp
#include <list>               // std::list
#include <tinyxml2.h>         // tinyxml2

class Flasher : public QObject
{
    Q_OBJECT

  private:
    std::list<FlashingStep> flashingSteps;
    std::string directory = "";
    Fastboot *fastboot = nullptr;
    bool rebootAfterFlashing = true;
    bool dryRun = false;
    bool verifyFiles = false;
    bool verifyHash(QString fileName, QString hash, QCryptographicHash::Algorithm hashType);

  public:
    Flasher(Fastboot *);
    bool LoadFile(QString);
    void SetDryRun(bool);
    void SetRebootAfterFlashing(bool);
    void SetVerifyFiles(bool);

  public slots:
    void Flash();

  signals:
    void OnStatusUpdate(std::string);
    void OnProgressChanged(int);
    void OnFinished();
};

#endif // FLASHER_H