/*
 * synergy -- mouse and keyboard sharing utility
 * Copyright (C) 2016 Symless.
 *
 * This package is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * found in the file COPYING that should have accompanied this file.
 *
 * This package is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#import <CoreFoundation/CoreFoundation.h>
#import <Carbon/Carbon.h>

#include "synergy/key_types.h"

#if defined(__cplusplus)
extern "C" {
#endif
bool				fakeNativeMediaKey(KeyID id);
bool				isMediaKeyEvent(CGEventRef event);
bool				getMediaKeyEventInfo(CGEventRef event, KeyID* keyId, bool* down, bool* isRepeat);
#if defined(__cplusplus)
}
#endif
