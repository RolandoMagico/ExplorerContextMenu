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
#include "NetExplorerContextMenuEntry.h"

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
  using namespace System::Drawing::Imaging;

  NetExplorerContextMenuEntry::NetExplorerContextMenuEntry()
  {
  }

  NetExplorerContextMenuEntry::NetExplorerContextMenuEntry(ExplorerContextMenuEntry* entry)
  {
    this->CopyNativeData(entry);
  }

  void NetExplorerContextMenuEntry::CopyNativeData(ExplorerContextMenuEntry* entry)
  {
    this->nativeEntry = entry;

    // Copy data for this instance
    if (this->nativeEntry->Text != nullptr)
    {
      this->Text = gcnew String(this->nativeEntry->Text->c_str());
    }

    if (this->nativeEntry->HelpText != nullptr)
    {
      this->HelpText = gcnew String(this->nativeEntry->Text->c_str());
    }

    if (this->nativeEntry->CommandString != nullptr)
    {
      this->CommandString = gcnew String(this->nativeEntry->CommandString->c_str());
    }

    uint32_t width, height, planes, bitsPerPixel;
    if (this->nativeEntry->GetBitmapDimensions(width, height, planes, bitsPerPixel) == true)
    {
      Bitmap^ bitmap = gcnew Bitmap(width, height);
      System::Drawing::Rectangle rectangle = System::Drawing::Rectangle(0, 0, width, height);
      BitmapData^ bitmapData = bitmap->LockBits(rectangle, ImageLockMode::ReadWrite, PixelFormat::Format32bppArgb);
      if (this->nativeEntry->GetBitmapPixelData(width, height, planes, bitsPerPixel, bitmapData->Scan0.ToPointer()) == true)
      {
        this->icon = bitmap;
      }

      bitmap->UnlockBits(bitmapData);
    }

    // Create child entries
    for each (ExplorerContextMenuEntry* nativeEntry in this->nativeEntry->menuEntries)
    {
      NetExplorerContextMenuEntry^ entry = gcnew NetExplorerContextMenuEntry(nativeEntry);
      this->childs->Add(entry);
    }
  }

  void NetExplorerContextMenuEntry::ExecuteCommand()
  {
    if (this->nativeEntry != nullptr)
    {
      this->nativeEntry->ExecuteCommand();
    }
  }

  UInt32 NetExplorerContextMenuEntry::CommandId::get()
  {
    return this->nativeEntry->CommandId;
  }

  Boolean NetExplorerContextMenuEntry::IsSeperator::get()
  {
    return this->nativeEntry->IsSeparator;
  }

  Image^ NetExplorerContextMenuEntry::Icon::get()
  {
    return this->icon;
  }

  IEnumerable<NetExplorerContextMenuEntry^>^ NetExplorerContextMenuEntry::Childs::get()
  {
    return this->childs;
  }
}
/***********************************************************************************************************************
 EOF
***********************************************************************************************************************/
