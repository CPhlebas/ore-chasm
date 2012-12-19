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

#ifndef DEBUG_H
#define DEBUG_H

#include <stdlib.h>
#include <string>
#include <iostream>
#include <sstream>

class LogStream;

class Debug
{
public:
    enum class Type
    {
        DebugType,
        Warning,
        Error,
    };

    enum class Area
    {
        General,
        Graphics,
        System,
        Physics,
        Sound
    };

    static LogStream log(Area area = Area::General);

    static void assertf(bool value, std::string message);
    static void fatal(bool value, Area area, std::string message);
};

class LogStream : public std::stringstream
{
public:
    LogStream(Debug::Area area);
    ~LogStream();

    LogStream(const LogStream& stream);

private:
    Debug::Area m_area;
};

#endif
