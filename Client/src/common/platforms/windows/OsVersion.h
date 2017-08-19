// TortoiseDarcs - a Windows shell extension for easy version control

// Copyright (C) 2000 - Francis Irving
// <francis@flourish.org> - May 2000

// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#ifndef OS_VERSION_H
#define OS_VERSION_H

#include <windows.h>

extern OSVERSIONINFO g_OsVersionInfo;

// Return true if the version of Windows is *not* 95, 98 or ME.
inline bool WindowsPlatformIsNT()
{
   return (g_OsVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT);
}


// Return true if the version of Windows is 95
inline bool WindowsVersionIs95()
{
   return (g_OsVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS && 
           g_OsVersionInfo.dwMajorVersion == 4 && 
           g_OsVersionInfo.dwMinorVersion == 0);
}


// Return true if the version of Windows is NT 4
inline bool WindowsVersionIsNT4()
{
   return (g_OsVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT && 
           g_OsVersionInfo.dwMajorVersion == 4 && 
           g_OsVersionInfo.dwMinorVersion == 0);
}


// Return true if the version of Windows is Win2000
inline bool WindowsVersionIs2K()
{
   return (g_OsVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT && 
           g_OsVersionInfo.dwMajorVersion == 5 && 
           g_OsVersionInfo.dwMinorVersion == 0);
}


// Return true if the version of Windows is 2000 or worse.
inline bool WindowsVersionIs2KOrHigher()
{
   return (g_OsVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT && 
           g_OsVersionInfo.dwMajorVersion >= 5);
}


// Return true if the version of Windows is XP or worse.
inline bool WindowsVersionIsXPOrHigher()
{
   return ((g_OsVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT) &&
          ((g_OsVersionInfo.dwMajorVersion == 5) &&
           (g_OsVersionInfo.dwMinorVersion >= 1)) ||
          (g_OsVersionInfo.dwMajorVersion >= 6));
}

#endif
