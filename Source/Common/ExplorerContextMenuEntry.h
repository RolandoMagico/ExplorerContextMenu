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
#include <string>
#include <vector>
#include "stdint.h"
#include "shlobj_core.h"
using namespace std;
/***********************************************************************************************************************
 CLASS
***********************************************************************************************************************/
namespace ContextQuickie
{
  /// <summary>
  /// This class represents one context menu entry.
  /// </summary>
  class ExplorerContextMenuEntry
  {
  private:
    /// <summary>
    /// The handle to the bitmap for this enty.
    /// </summary>
    HBITMAP BitmapHandle = nullptr;

    /// <summary>
    /// The context menu which contains this entry.
    /// </summary>
    IContextMenu* ContextMenu = nullptr;

    /// <summary>
    /// The next position at which new items are inserted during menu creation.
    /// </summary>
    uint32_t NextInsertPosition = 0;

    /// <summary>
    /// Add child entries to this entry.
    /// </summary>
    /// <param name="contextMenu">The context menu instance containing the child entries.</param>
    /// <param name="menu">The menu containing the data of the child entries.</param>
    /// <param name="isDefaultMenu">A value indicating whether the menu is the default menu or not.</param>
    void AddEntries(IContextMenu* contextMenu, HMENU menu, bool isDefaultMenu);

  public:
    /// <summary>
    /// The text of the entry.
    /// </summary>
    wstring* Text = nullptr;

    /// <summary>
    /// The help text of the entry.
    /// </summary>
    wstring* HelpText = nullptr;

    /// <summary>
    /// The command ID of the entry.
    /// </summary>
    uint32_t CommandId = 0;

    /// <summary>
    /// The command string of the entry
    /// </summary>
    string* CommandString = nullptr;

    /// <summary>
    /// Indicates wheter the entry is a separator or not.
    /// </summary>
    bool IsSeparator = false;

    /// <summary>
    /// The child entries of this entry.
    /// </summary>
    vector<ExplorerContextMenuEntry*> menuEntries;

    /// <summary>
    /// The default constructor.
    /// </summary>
    ExplorerContextMenuEntry();

    /// <summary>
    /// The constructor for creating an entry for a specific context menu.
    /// </summary>
    /// <param name="contextMenu">The context menu interface which is used for this entry.</param>
    /// <param name="menu">The menu which provides the data for this entry.</param>
    /// <param name="index">The index of this entry in the menu.</param>
    ExplorerContextMenuEntry(IContextMenu* contextMenu, HMENU menu, int32_t index);

    /// <summary>
    /// The default destructor.
    /// </summary>
    ~ExplorerContextMenuEntry();

    /// <summary>
    /// Gets the number of all entries including submenu entries.
    /// </summary>
    /// <returns></returns>
    size_t GetAllEntriesCount();

    /// <summary>
    /// Loads the child entries of this instance from the specific context menu interface.
    /// </summary>
    /// <param name="contextMenu">The context menu instance representing the child entries.</param>
    /// <param name="flags">The flags which are used to query the conext menu.</param>
    void GetMenuData(IContextMenu* contextMenu, uint32_t flags);

    /// <summary>
    /// Gets the dimensions of the bitmap image for this entry.
    /// </summary>
    /// <param name="width">Reference to store the image width.</param>
    /// <param name="height">Reference to store the image height.</param>
    /// <param name="planes">Reference to store the number of planes of the image.</param>
    /// <param name="bitsPerPixel">Reference to store bits per pixel of the image.</param>
    /// <returns><c>true</c> if the image data could be retrieved; otherwise <c>false</c>.</returns>
    bool GetBitmapDimensions(uint32_t& width, uint32_t& height, uint32_t& planes, uint32_t& bitsPerPixel);

    /// <summary>
    /// Gets the pixel data of the bitmap image for this entry.
    /// </summary>
    /// <param name="width">The image width.</param>
    /// <param name="height">The image height.</param>
    /// <param name="planes">The number of planes of the image.</param>
    /// <param name="bitsPerPixel">The bits per pixel of the image.</param>
    /// <param name="destination">A pointer to the buffer where the image data will be copied to.</param>
    /// <returns><c>true</c> if the image data could be retrieved; otherwise <c>false</c>.</returns>
    bool GetBitmapPixelData(uint32_t width, uint32_t height, uint32_t planes, uint32_t bitsPerPixel, void* destination);

    /// <summary>
    /// Executes the command for this entry.
    /// </summary>
    /// <param name="windowHandle">The handle to the window which executes the command.</param>
    /// <param name="executeSynchronous">A value indicating whether the command is executed synchronously or not.
    /// Note that synchronous execution is not supported by all context menu entries.</param>
    /// <returns>0 if the execution was successful, INT32_MIN if no valid context menu instance exist 
    /// or the HRESULT code from the call to InvokeCommand.</returns>
    int32_t ExecuteCommand(HWND windowHandle, bool executeSynchronous = false);

    /// <summary>
    /// Prints the context menu to the console.
    /// </summary>
    /// <param name="level">The indentation level for printing the commands.</param>
    void PrintMenu(uint32_t level = 0);

    /// <summary>
    /// Removes duplicate separators and the separator at the end (if it exists) from the child entries.
    /// </summary>
    void RemoveDuplicateSeparators();
  };
}
/***********************************************************************************************************************
 EOF
***********************************************************************************************************************/

