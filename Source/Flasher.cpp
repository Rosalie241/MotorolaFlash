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

#include "Flasher.h"

Flasher::Flasher(Fastboot *fastboot)
{
    this->fastboot = fastboot;
}

bool Flasher::LoadFile(QString file)
{
    // clear vars
    this->flashingSteps.clear();
    this->directory = "";

    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError ret = doc.LoadFile(file.toStdString().c_str());

    if (ret != tinyxml2::XMLError::XML_SUCCESS)
        return false;

    tinyxml2::XMLElement *steps = doc.FirstChildElement("flashing")->FirstChildElement("steps");

    if (!steps)
        return false;

    tinyxml2::XMLElement *step = steps->FirstChildElement("step");

    if (!step)
        return false;

    // add all the steps to the 'flashingSteps' list
    while (step)
    {
        FlashingStep fstep;

        if (step->Attribute("filename") != NULL)
            fstep.filename = std::string(step->Attribute("filename"));
        if (step->Attribute("operation") != NULL)
            fstep.operation = std::string(step->Attribute("operation"));
        if (step->Attribute("partition") != NULL)
            fstep.partition = std::string(step->Attribute("partition"));
        if (step->Attribute("var") != NULL)
            fstep.var = std::string(step->Attribute("var"));
        if (step->Attribute("MD5") != NULL)
            fstep.md5 = std::string(step->Attribute("MD5"));
        if (step->Attribute("SHA1") != NULL)
            fstep.sha1 = std::string(step->Attribute("SHA1"));

        // add step to list
        flashingSteps.push_back(fstep);

        // go to next element
        step = step->NextSiblingElement();
    }

    this->directory = QFileInfo(file).absolutePath().toStdString();

    return true;
}

void Flasher::Flash()
{
    // make sure we have everything
    if (!this->fastboot || this->flashingSteps.empty() || this->directory.empty())
        return;

    float totalSteps = flashingSteps.size();
    float currentStep = 0.0;
    bool failed = false;
    for (FlashingStep step : this->flashingSteps)
    {
        currentStep++;

        if (step.operation == "flash")
        { // fastboot flash

            QString fileName = QDir::cleanPath(QString::fromStdString(this->directory) + QDir::separator() +
                                               QString::fromStdString(step.filename));

            if (!this->fastboot->Flash(fileName.toStdString(), step.partition))
                break;
        }
        else if (step.operation == "erase")
        { // fastboot erase

            failed = !this->fastboot->Erase(step.partition);
        }
        else if (step.operation == "oem")
        { // fastboot oem

            failed = !this->fastboot->Oem(step.var);
        }
        else if (step.operation == "getvar")
        { // fastboot getvar

            failed = !this->fastboot->GetVar(step.var, nullptr);
        }

        emit this->OnProgressChanged((int)(currentStep / totalSteps * 100));

        // break when failed
        if (failed)
            break;
    }

    if (this->RebootAfterFlashing && !failed)
        this->fastboot->Reboot();

    emit this->OnFinished();
}