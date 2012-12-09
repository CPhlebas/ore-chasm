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

static Time* s_instance = 0;

Time::Time()
{
    m_hour = 7;
    m_minute = 0;
    std::cout << "Current Time, hour: " << m_hour << " : " << m_minute << " pm? " << m_pm << "\n";
}

Time::~Time()
{
}

void Time::setHour(unsigned char hour)
{

}

void Time::setMinute(unsigned char minute)
{

}

void Time::tick()
{
    std::cout << "Current Time, hour: " << m_hour << " : " << m_minute << " pm? " << m_pm << "\n";
    // 1 minute, presumably
    int interval = 1;

    m_minute += interval;

    if (m_minute >= 60) {
        ++m_hour;
        m_minute = 0;
    }

    if (m_hour > 12) {
        m_pm = !m_pm;
        m_hour = 1;
    }


}

Time* Time::instance()
{
    if (!s_instance) {
        s_instance = new Time();
    }

    return s_instance;
}
