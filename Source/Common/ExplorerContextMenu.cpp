/***********************************************************************************************************************
 MIT License

 Copyright(c) 2020 Roland Reinl

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files(the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and /or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions :

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
***********************************************************************************************************************/

/***********************************************************************************************************************
 INCLUDES
***********************************************************************************************************************/
#include "ExplorerContextMenu.h"
#include "objbase.h"
#include "shlobj_core.h"
#pragma unmanaged

/***********************************************************************************************************************
 DEFINES
***********************************************************************************************************************/
#define GET_KEY_STATE_KEY_PRESSED   (0x8000u)
/***********************************************************************************************************************
 TYPES
***********************************************************************************************************************/

/***********************************************************************************************************************
 LOCAL CONSTANTS
***********************************************************************************************************************/

/***********************************************************************************************************************
 LOCAL VARIABLES
***********************************************************************************************************************/

/***********************************************************************************************************************
 LOCAL FUNCTION DECLARATIONS
***********************************************************************************************************************/

/***********************************************************************************************************************
 IMPLEMENTATION
***********************************************************************************************************************/
namespace ContextQuickie
{
  ExplorerContextMenu::ExplorerContextMenu(vector<wstring> paths)
  {
    HRESULT result = S_OK;

    if (paths.empty())
    {
      /* Nothing to do if an empty list ist passed */
    }
    else if (paths.size() > UINT_MAX)
    {
      /* Too many items, cannot handle that */
    }
    else if (SUCCEEDED(result = CoInitialize(nullptr)))
    {
      ITEMIDLIST** itemIdList = new ITEMIDLIST * [paths.size()];
      for (size_t pathIndex = 0; (pathIndex < paths.size()) && SUCCEEDED(result); pathIndex++)
      {
        result = SHParseDisplayName(paths[pathIndex].c_str(), nullptr, &(itemIdList[pathIndex]), 0, NULL);
      }

      if (SUCCEEDED(result))
      {
        IShellFolder* desktop = nullptr;
        if (SUCCEEDED(result = SHGetDesktopFolder(&desktop)))
        {
          HWND windowHandle = GetCurrentWindowHandle();
          if (windowHandle != nullptr)
          {
            DEFCONTEXTMENU defaultContextMenu = { 0 };
            defaultContextMenu.hwnd = windowHandle;
            defaultContextMenu.psf = desktop;
            defaultContextMenu.cidl = (UINT)paths.size();
            defaultContextMenu.apidl = (LPCITEMIDLIST*)itemIdList;

            HMENU menu = CreatePopupMenu();
            UINT flags = CMF_NORMAL;
            if ((GetKeyState(VK_SHIFT) & GET_KEY_STATE_KEY_PRESSED) == GET_KEY_STATE_KEY_PRESSED)
            {
              flags |= CMF_EXTENDEDVERBS;
            }
            IContextMenu* contextMenu;
            if (SUCCEEDED(result = SHCreateDefaultContextMenu(&defaultContextMenu, IID_IContextMenu3, (void**)&(contextMenu))))
            {
              result = contextMenu->QueryContextMenu(menu, 0, 0, UINT_MAX, flags);
            }
            else if (SUCCEEDED(result = SHCreateDefaultContextMenu(&defaultContextMenu, IID_IContextMenu2, (void**)&(contextMenu))))
            {
              result = contextMenu->QueryContextMenu(menu, 0, 0, UINT_MAX, flags);
            }
            else if (SUCCEEDED(result = SHCreateDefaultContextMenu(&defaultContextMenu, IID_IContextMenu, (void**)&(contextMenu))))
            {
              result = contextMenu->QueryContextMenu(menu, 0, 0, UINT_MAX, flags);
            }

            if (SUCCEEDED(result))
            {
              this->GetMenuData(menu);
            }

            DestroyMenu(menu);
          }

          desktop->Release();
        }
      }
    }
  }

  HWND ExplorerContextMenu::GetCurrentWindowHandle()
  {
    /* Try to get the current window handle, try to get the desktop handle first, if it doesn't work, try to get the console window handle. */
    HWND windowHandle = nullptr;
    windowHandle = GetDesktopWindow();
    if (windowHandle == nullptr)
    {
      windowHandle = GetConsoleWindow();
    }

    return windowHandle;
  }

  uint32_t ExplorerContextMenu::GetNumberOfEntries(HMENU menu)
  {
    uint32_t count = 0;
    for (int menuIndex = 0; menuIndex < GetMenuItemCount(menu); menuIndex++)
    {
      MENUITEMINFO menuInfo;

      menuInfo.fMask = MIIM_SUBMENU;
      menuInfo.cbSize = sizeof(MENUITEMINFO);
      if (GetMenuItemInfo(menu, menuIndex, true, &menuInfo) == false)
      {
        /* TODO: Error handling if the text cannot be retrieved */
      }
      else if (menuInfo.hSubMenu != nullptr)
      {
        count += GetNumberOfEntries(menuInfo.hSubMenu);
      }

      count++;
    }

    return count;
  }
}
/***********************************************************************************************************************
 EOF
***********************************************************************************************************************/