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

#include "time.h"

#include <iostream>
#include <sstream>

static Time* s_instance = 0;

Time::Time()
{
}

Time::~Time()
{
}

void Time::tick()
{
    // 1 minute, presumably
    int interval = 1;

    m_minute += interval;

    if (m_minute >= 60) {
        ++m_hour;
        m_minute = 0;
    }

    if (m_hour > 24) {
        m_hour = 0;
    }
}

std::string Time::toString()
{
    std::stringstream ss;
    ss << short(m_hour) << ":";

    //format it so it's native looking wrt single digit minutes
    if (m_minute < 10) {
        ss << "0";
    }

    ss << short(m_minute);
    return ss.str();
}

Time* Time::instance()
{
    if (!s_instance) {
        s_instance = new Time();
    }

    return s_instance;
}
