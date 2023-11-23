///////////////////////////////////////////////////////////////////////////////
// File:	gvDirectSound.h
// SDK:		GameSpy Voice 2 SDK
//
// Copyright Notice: This file is part of the GameSpy SDK designed and 
// developed by GameSpy Industries. Copyright (c) 2004-2009 GameSpy Industries, Inc.

#ifndef _GV_DIRECT_SOUND_H_
#define _GV_DIRECT_SOUND_H_

#include "gvMain.h"

GVBool gviHardwareStartup(HWND hWnd);
void gviHardwareCleanup(void);
void gviHardwareThink(void);

int gviHardwareListDevices(GVDeviceInfo devices[], int maxDevices, GVDeviceType types);

GVDevice gviHardwareNewDevice(GVDeviceID deviceID, GVDeviceType type);

#endif
