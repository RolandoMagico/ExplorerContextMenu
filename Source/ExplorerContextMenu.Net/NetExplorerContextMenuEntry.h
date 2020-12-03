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
#include "ExplorerContextMenuEntry.h"

/***********************************************************************************************************************
 CLASS
***********************************************************************************************************************/
namespace ContextQuickie
{
  using namespace System;
  using namespace System::Collections::Generic;
  using namespace System::Drawing;

  /// <summary>
  /// This class represents one entry in a context menu.
  /// </summary>
  public ref class NetExplorerContextMenuEntry
  {
  private:
    /// <summary>
    /// Backing storage for <see cref="Icon"/>.
    /// </summary>
    Image^ icon;

    /// <summary>
    /// Reference to the natvie entry of this instance.
    /// </summary>
    ExplorerContextMenuEntry* nativeEntry;

    /// <summary>
    /// Backing storage for <see cref="Childs"/>.
    /// </summary>
    List<NetExplorerContextMenuEntry^>^ childs = gcnew List<NetExplorerContextMenuEntry^>();

    /// <summary>
    /// Creates a new instance of the <see cref="NetExplorerContextMenuEntry"/> class.
    /// This constructor is used to wrap an existing instacne of a native context menu entry.
    /// </summary>
    /// <param name="entry">The native intsance which is wrapped.</param>
    NetExplorerContextMenuEntry(ExplorerContextMenuEntry* entry);

  protected:
    /// <summary>
    /// Copies the data from a native entry to this instance.
    /// </summary>
    /// <param name="entry">The native instance from which the data is copied.</param>
    void CopyNativeData(ExplorerContextMenuEntry* entry);

  public:
    /// <summary>
    /// Creates a new instance of the <see cref="NetExplorerContextMenuEntry"/> class.
    /// </summary>
    NetExplorerContextMenuEntry();

    /// <summary>
    /// Executes the context menu entry command.
    /// </summary>
    void ExecuteCommand();

    /// <summary>
    /// Gets or sets the text of the entry.
    /// </summary>
    property String^ Text;

    /// <summary>
    /// Gets or sets the help text of the entry.
    /// </summary>
    property String^ HelpText;

    /// <summary>
    /// Gets the command ID of the entry.
    /// </summary>
    property UInt32 CommandId
    {
      UInt32 get();
    }

    /// <summary>
    /// Gets or sets the command string of the entry.
    /// </summary>
    property String^ CommandString;

    /// <summary>
    /// Gets a value indicating whether this instance is a seperator or not.
    /// </summary>
    property Boolean IsSeperator
    {
      Boolean get();
    }

    /// <summary>
    /// Gets the child elements of this entry.
    /// </summary>
    property IEnumerable<NetExplorerContextMenuEntry^>^ Childs
    {
      IEnumerable<NetExplorerContextMenuEntry^>^ get();
    }

    /// <summary>
    /// Gets the icon of this entry.
    /// </summary>
    property Image^ Icon
    {
      Image^ get();
    }
  };
}

/***********************************************************************************************************************
 EOF
***********************************************************************************************************************/
