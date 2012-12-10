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

#ifndef TIME_H
#define TIME_H

#include <string>
#include <assert.h>

class Time
{
public:
    Time();
    static Time* instance();

    //FIXME: pass elapsed time into this
    /**
     * Called once per update() of the World, increments
     * the world time according to a constant value, compensated
     * by elapsed frame time.
     */
    void tick();

    void setTime(unsigned char hour, unsigned char minute) { setHour(hour); setMinute(minute); }

    void setHour(unsigned char hour) { assert(hour <= 12 && hour > 0); m_hour = hour; }

    void setMinute(unsigned char minute) { assert(minute <= 59 && minute >= 0); m_minute = minute; }

    unsigned char currentHour() { return m_hour; }
    unsigned char currentMinute() { return m_minute; }

    /**
     * Returns the current time as a formatted string.
     * e.g. 12:45, 23:59, 00:00. 24-hour clock.
     */
    std::string toString();

private:
    ~Time();

    //sunrise should be ~07:00, sunset 19:00
    unsigned char m_hour = 0;
    unsigned char m_minute = 0;
};
#endif