/*
**
** Copyright (C) 2020 Ashcon Mohseninia
** Author: Ashcon Mohseninia <ashcon50@gmail.com>
**
** This library is free software; you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published
** by the Free Software Foundation, either version 3 of the License, or (at
** your option) any later version.
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public
** License along with this library; if not, <http://www.gnu.org/licenses/>.
**
*/

#include "pch.h"
#include "Logger.h"
#include "nyanko-passthru.h"

bool startup()
{
    LOGGER.writeToFile("\n\n##RESTART##\n");
    return true;
}

void close()
{}

BOOL APIENTRY DllMain(
    HMODULE hModule,
    DWORD ul_reason_for_call,
    LPVOID lpReserved
)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        LOGGER.logInfo("DLLMAIN", "NYANKO DLL LOADED");
    }

    return TRUE;
}