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

bool Flasher::LoadFile(QString file)
{
    // clear fsteps
    flashingSteps.clear();

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
        fstep.filename = step->Attribute("filename");
        fstep.operation = step->Attribute("operation");
        fstep.partition = step->Attribute("partition");
        fstep.var = step->Attribute("var");
        fstep.md5 = step->Attribute("MD5");
        fstep.sha1 = step->Attribute("SHA1");

        // add step to list
        flashingSteps.push_back(fstep);

        // go to next element
        step = step->NextSiblingElement();
    }

    return true;
}