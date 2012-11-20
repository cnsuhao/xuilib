#pragma once

/// ATL
#include <atlbase.h>
#include <atlwin.h>
#include <atltypes.h>
#include <atlstr.h>

/// WTL
#include "WTL80\atlapp.h"
#include "WTL80\atlctrls.h"

#include "GlobalConstants.h"

/// GDIPLUS
#pragma comment(lib, "gdiplus.lib")

/// XUI Initializing. 
#pragma comment(linker, "/INCLUDE:?g_xui_life@@3VCXUILIFE@@A")