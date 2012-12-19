/******************************************************************************
 *   Copyright (C) 2012 by Shaun Reich <sreich@kde.org>                       *
 *                                                                            *
 *   This program is free software; you can redistribute it and/or            *
 *   modify it under the terms of the GNU General Public License as           *
 *   published by the Free Software Foundation; either version 2 of           *
 *   the License, or (at your option) any later version.                      *
 *                                                                            *
 *   This program is distributed in the hope that it will be useful,          *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *   GNU General Public License for more details.                             *
 *                                                                            *
 *   You should have received a copy of the GNU General Public License        *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.    *
 *****************************************************************************/

#include "debug.h"

#include <assert.h>
#include <iostream>
#include <sstream>

#ifndef NDEBUG
#endif

LogStream Debug::log(Debug::Area area)
{
    return LogStream(area);
}

void Debug::assertf(bool value, std::string message)
{
    if (!value) {
        std::cout << message << "\n";
        assert(0);
    }
}

void Debug::fatal(bool value, Debug::Area area, std::string message)
{
    if (!value) {
        Debug::log(area) << "FATAL: " << message;
        assert(0);
    }
}

////////////////////////////////////////////////////////////////// LogStream class ///////////////////////////////////////////////////////////////////////////

LogStream::LogStream(Debug::Area area) : m_area(area)
{
}

LogStream::~LogStream()
{
    std::string areaString;

    switch (m_area) {
    case Debug::Area::General:
        areaString.append("[General]");
        break;
    case Debug::Area::Graphics:
        areaString.append("[Graphics]");
        break;
    case Debug::Area::Physics:
        areaString.append("[Physics]");
        break;
    case Debug::Area::Sound:
        areaString.append("[Sound]");
        break;
    case Debug::Area::System:
        areaString.append("[System]");
        break;
    }
    std::cout << "----------- " << areaString << " | " << str() << " ---------------------------------------------------" << "\n";
}

LogStream::LogStream(const LogStream& stream): std::stringstream(stream.str())
{
    m_area = stream.m_area;
}
