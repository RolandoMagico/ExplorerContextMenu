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
#include <algorithm>
#pragma unmanaged

/***********************************************************************************************************************
 DEFINES
***********************************************************************************************************************/

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
  ExplorerContextMenu::ExplorerContextMenu(const vector<wstring>& paths, bool createDefaultMenu, const set<wstring>& extendedMenuWhitelist, const set<wstring>& extendedMenuBlacklist)
  {
    HRESULT result = S_OK;
    IShellFolder* desktop = nullptr;
    ITEMIDLIST** itemIdList = nullptr;
    uint32_t itemIdListLength = 0;
    bool pathsContainsFiles = false;
    bool pathsContainsDirectories = false;

    if (paths.empty())
    {
      // Nothing to do if an empty list ist passed
    }
    else if (paths.size() > UINT32_MAX)
    {
      // Too many items, cannot handle that :-(
    }
    else if (FAILED(result = CoInitialize(nullptr)))
    {
      // Nothing to do here, CoInitialize failed :-(
    }
    else if (FAILED(result = SHGetDesktopFolder(&desktop)))
    {
      // Unable to retrive desktop opbject :-(
    }
    else
    {
      this->shellExtensions.push_back(desktop);
      itemIdListLength = (uint32_t)paths.size();
      itemIdList = new ITEMIDLIST * [paths.size()];
      for (size_t pathIndex = 0; (pathIndex < paths.size()) && SUCCEEDED(result); pathIndex++)
      {
        uint32_t fileAttributes = GetFileAttributes(paths[pathIndex].c_str());
        if (fileAttributes != INVALID_FILE_ATTRIBUTES)
        {
          if ((fileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
          {
            pathsContainsDirectories = true;
          }
          else
          {
            pathsContainsFiles = true;
          }
        }

        result = desktop->ParseDisplayName(NULL, NULL, (LPWSTR)paths[pathIndex].c_str(), nullptr, &(itemIdList[pathIndex]), NULL);
      }
    }

    if (SUCCEEDED(result) && (itemIdListLength != 0))
    {
      LPCITEMIDLIST* itemIdListArg = (LPCITEMIDLIST*)itemIdList;
      if (createDefaultMenu == true)
      {
        if (FAILED(result = this->GetDefaultContextMenu(desktop, itemIdListArg, itemIdListLength)))
        {
          // Something went wrong when creating the default menu
        }
      }
      
      if (SUCCEEDED(result))
      {
        vector<wstring> extensions;

        if ((pathsContainsFiles == false) && (pathsContainsDirectories == true))
        {
          this->GetAvailableMenuExtensions(L"Directory\\shellex\\ContextMenuHandlers", extensions);
          this->GetAvailableMenuExtensions(L"Directory\\Background\\shellex\\ContextMenuHandlers", extensions);
        }

        if ((pathsContainsFiles == true) && (pathsContainsDirectories == false))
        {
          this->GetAvailableMenuExtensions(L"*\\shellex\\ContextMenuHandlers", extensions);
        }

        this->GetAvailableMenuExtensions(L"AllFilesystemObjects\\shellex\\ContextMenuHandlers", extensions);

        // Ensure that every CLSID is only listed once
        extensions.erase(unique(extensions.begin(), extensions.end()), extensions.end());

        // Create extended context menu only if default was either skipped or successful
        if (FAILED(result = this->GetExtendedContextMenu(extensions, desktop, itemIdListArg, itemIdListLength, extendedMenuWhitelist, extendedMenuBlacklist)))
        {
          // Something went wrong when creating the exteneded menu
        }
      }

      this->RemoveDuplicateSeparators();
    }

    if (itemIdList != nullptr)
    {
      delete[] itemIdList;
    }
  }

  ExplorerContextMenu::~ExplorerContextMenu()
  {
    ULONG releaseStatus;
    for (size_t index = 0; index < this->shellExtensions.size(); index++)
    {
      releaseStatus = this->shellExtensions[index]->Release();
    }
  }

  HRESULT ExplorerContextMenu::GetDefaultContextMenu(IShellFolder* desktop, LPCITEMIDLIST* itemIdList, UINT itemIdListLength)
  {
    HRESULT result = S_OK;
    DEFCONTEXTMENU defaultContextMenu = { 0 };
    defaultContextMenu.hwnd = NULL;
    defaultContextMenu.psf = desktop;
    defaultContextMenu.cidl = itemIdListLength;
    defaultContextMenu.apidl = itemIdList;

    IContextMenu* contextMenu;

    if (SUCCEEDED(result = SHCreateDefaultContextMenu(&defaultContextMenu, IID_PPV_ARGS(&contextMenu))))
    {
      this->shellExtensions.push_back(contextMenu);
      this->GetMenuData(contextMenu, CMF_DEFAULTONLY);
    }

    return result;
  }

  HRESULT ExplorerContextMenu::GetExtendedContextMenu(const vector<wstring>& extensions, IShellFolder* desktop, LPCITEMIDLIST* itemIdList, UINT itemIdListLength, const set<wstring>& extendedMenuWhitelist, const set<wstring>& extendedMenuBlacklist)
  {
    HRESULT result = S_OK;
    IDataObject* dataObject = nullptr;

    if (extensions.size() == 0)
    {
      // No extensions availabe, nothing more to do
    }
    else if (FAILED(result = desktop->GetUIObjectOf(NULL, itemIdListLength, itemIdList, IID_IDataObject, NULL, (void**)&dataObject)))
    {
      // Unable to retrieve data object :-(
    }
    else
    {
      // Change all entreis to lower case
      set<wstring> whitelistLowerCase;
      for (wstring entry : extendedMenuWhitelist)
      {
        std::transform(entry.begin(), entry.end(), entry.begin(), ::towlower);
        whitelistLowerCase.insert(entry);
      }

      set<wstring> blacklistLowerCase;
      for (wstring entry : extendedMenuBlacklist)
      {
        std::transform(entry.begin(), entry.end(), entry.begin(), ::towlower);
        blacklistLowerCase.insert(entry);
      }

      for (wstring extension : extensions)
      {
        if ((whitelistLowerCase.empty() || (whitelistLowerCase.find(extension) != whitelistLowerCase.end())))
        {
          if (blacklistLowerCase.find(extension) == blacklistLowerCase.end())
          {
            // Convert string to CLSID. 
            // Because coversion to GUID is already tested in GetAvailableMenuExtensions, no return value check is required here
            GUID CLSID_NewMenu;
            (void)CLSIDFromString(extension.c_str(), &CLSID_NewMenu);

            IShellExtInit* shellExtInit;
            if (FAILED(result = CoCreateInstance(CLSID_NewMenu, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&shellExtInit))))
            {
              // Failed to create an IShellExtInit instance :-(
            }
            else if (FAILED(result = shellExtInit->Initialize(NULL, dataObject, NULL)))
            {
              // Failed to initialied the shell extension
              shellExtInit->Release();
            }
            else
            {
              this->shellExtensions.push_back(shellExtInit);
              IContextMenu* contextMenu;
              if (SUCCEEDED(result = shellExtInit->QueryInterface(IID_PPV_ARGS(&contextMenu))))
              {
                this->GetMenuData(contextMenu, CMF_NORMAL);
              }
            }
          }
        }
      }
    }
 
    if (dataObject != nullptr)
    {
      dataObject->Release();
    }

    return result;
  }

  bool ExplorerContextMenu::GetAvailableMenuExtensions(const wstring& registryPath, vector<wstring>& target)
  {
    HRESULT result = S_OK;
    LSTATUS registryResult;
    HKEY contextMenuHandlers = nullptr;
    DWORD numberOfSubKey = 0;
    DWORD subKeyMaxLength = 0;
    bool returnValue = false;

    if ((registryResult = RegOpenKeyEx(HKEY_CLASSES_ROOT, registryPath.c_str(), 0, KEY_READ, &contextMenuHandlers)) != ERROR_SUCCESS)
    {
      // Unable to open registry :-(
    }
    else if ((registryResult = RegQueryInfoKey(contextMenuHandlers, NULL, NULL, NULL, &numberOfSubKey, &subKeyMaxLength, NULL, NULL, NULL, NULL, NULL, NULL)) != ERROR_SUCCESS)
    {
      // Unable to read registry keys :-(
    }
    else
    {
      for (uint32_t keyIndex = 0; keyIndex < numberOfSubKey; keyIndex++)
      {
        wchar_t registryValue[255] = { 0 };
        DWORD bufferLength = sizeof(registryValue);

        DWORD numberOfReadBytes = subKeyMaxLength + 1;
        wchar_t* registryKeyName = new wchar_t[numberOfReadBytes];

        if ((registryResult = RegEnumKeyEx(contextMenuHandlers, keyIndex, registryKeyName, &numberOfReadBytes, NULL, NULL, NULL, NULL)) != ERROR_SUCCESS)
        {
          // Unable to enumerate registry keys :-(
        }
        else if ((registryResult = RegGetValue(contextMenuHandlers, registryKeyName, NULL, RRF_RT_REG_SZ, NULL, registryValue, &bufferLength)) != ERROR_SUCCESS)
        {
          // Unable to get registry value :-(
        }
        else
        {
          returnValue = true;
          // Convert string to CLSID. As some plugins use the registry key name for the ID and some use the registry value, try both
          GUID CLSID_NewMenu;
          wchar_t* clsidString = nullptr;
          if (SUCCEEDED(result = CLSIDFromString(registryValue, &CLSID_NewMenu)))
          {
            clsidString = registryValue;
          }
          else if (SUCCEEDED(result = CLSIDFromString(registryKeyName, &CLSID_NewMenu)))
          {
            clsidString = registryKeyName;
          }

          if (SUCCEEDED(result))
          {
            wstring clsidStringLowerCase(clsidString);
            std::transform(clsidStringLowerCase.begin(), clsidStringLowerCase.end(), clsidStringLowerCase.begin(), ::towlower);
            target.push_back(clsidStringLowerCase);
          }
        }

        delete[] registryKeyName;
      }
    }

    return returnValue;
  }
}

/***********************************************************************************************************************
 EOF
***********************************************************************************************************************/