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
#pragma once
#include <set>
#include <string>
#include <vector>

#include "ExplorerContextMenuEntry.h"

#include "shobjidl_core.h"

using namespace std;
/***********************************************************************************************************************
 CLASS
***********************************************************************************************************************/
namespace ContextQuickie
{
  /// <summary>
  /// This class represents the root element of a context menu.
  /// </summary>
  class ExplorerContextMenu : public ExplorerContextMenuEntry
  {
  private:
    /// <summary>
    /// A list with loaded shell extensions during menu creation.
    /// </summary>
    vector<IUnknown*> shellExtensions;

    /// <summary>
    /// Loads the default explorer context menu.
    /// </summary>
    /// <param name="desktop">A reference to the desktop folder which is used as the parent element for all files/folders.</param>
    /// <param name="itemIdList">A list of file system items for which the menu will be created.</param>
    /// <param name="itemIdListLength">The number of items in <see paramref="itemIdList"/></param>
    /// <returns>A value indicating whether loading the default context menu was successful or not.</returns>
    HRESULT GetDefaultContextMenu(IShellFolder* desktop, LPCITEMIDLIST* itemIdList, UINT itemIdListLength);

    /// <summary>
    /// Loads the extended explorer context menu.
    /// The extended menu contains the entris which are specified by shell extensions in the registry.
    /// </summary>
    /// <param name="desktop">A reference to the desktop folder which is used as the parent element for all files/folders.</param>
    /// <param name="itemIdList">A list of file system items for which the menu will be created.</param>
    /// <param name="itemIdListLength">The number of items in <see paramref="itemIdList"/></param>
    /// <param name="extendedMenuWhitelist">A list of context menu extension CLSIDs which are loaded. If the list is empty, all available extensions are loaded.</param>
    /// <param name="extendedMenuBlacklist">A list of context menu extension CLSIDs which are not loaded.</param>
    /// <returns>A value indicating whether loading the default context menu was successful or not.</returns>
    HRESULT GetExtendedContextMenu(IShellFolder* desktop, LPCITEMIDLIST* itemIdList, UINT itemIdListLength, const set<wstring>& extendedMenuWhitelist, const set<wstring>& extendedMenuBlacklist);

    /// <summary>
    /// Gets the CLSIDs for all installed context menu extensions from the registry.
    /// </summary>
    /// <param name="target">A list in which the CLSIDs are stored.</param>
    /// <returns><c>true</c> if retrieving the CLSIDs from the registry was successful; otherwise <c>false</c>.</returns>
    bool GetAvailableMenuExtensions(vector<wstring>& target);

  public:
    /// <summary>
    /// Constructor which creates the context menu.
    /// </summary>
    /// <param name="paths">A list of paths for which the context menu will be created.</param>
    /// <param name="createDefaultMenu">A value indicating whether the default menu will be loaded or not.</param>
    /// <param name="extendedMenuWhitelist">A list of context menu extension CLSIDs which are loaded. If the list is empty, all available extensions are loaded.</param>
    /// <param name="extendedMenuBlacklist">A list of context menu extension CLSIDs which are not loaded.</param>
    ExplorerContextMenu(const vector<wstring>& paths, bool createDefaultMenu = true, const set<wstring>& extendedMenuWhitelist = set<wstring>(), const set<wstring>& extendedMenuBlacklist = set<wstring>());

    /// <summary>
    /// Destructor.
    /// </summary>
    ~ExplorerContextMenu();
  };
}
/***********************************************************************************************************************
 EOF
***********************************************************************************************************************/

