#include <iostream>
#include <string>
#include <windows.h>
using namespace std;
//https://icoconvert.com/
/*
CF_TEXT: Text data as an ANSI string (null-terminated)
CF_UNICODETEXT: Text data as a Unicode string
CF_BITMAP: Handle to a bitmap (if the clipboard contains image data)
CF_HDROP: File paths (used for drag-and-drop operations)
*/

int main()
{
    string name = "";
    int i = 0;

    //open the clipboard and read text
    if (OpenClipboard(nullptr))
    {
        HANDLE handle_data = GetClipboardData(CF_TEXT); //get the text data from the clipboard
        if (handle_data != nullptr)
        {
            char* clipboard_text = static_cast<char*>(GlobalLock(handle_data));
            if (clipboard_text != nullptr)
            {
                name = clipboard_text; //copy clipboard text to the string(name)
                GlobalUnlock(handle_data);

                cout << "Your Input: " << name << endl;
            }
        }
        else
        {
            cout << "The clipboard is empty" << endl;
        }
        CloseClipboard();
    }
    else
    {
        cout << "Cannot open clipboard" << endl;
        return 1;
    }

    //convert the text to lowercase
    while (name[i] != '\0')
    {
        //cout << i << ' ';
        name[i] = tolower(name[i]);
        i++;
    }
    cout << endl;

    //write the modified text back to the clipboard
    if (OpenClipboard(nullptr))
    {
        EmptyClipboard(); //clear existing clipboard content

        //allocate memory for the new text
        size_t text_length = name.length() + 1; //include null terminator
        HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, text_length);
        if (hMem != nullptr)
        {
            //copy the modified text into the allocated memory
            char* pMem = static_cast<char*>(GlobalLock(hMem));
            if (pMem != nullptr)
            {
                memcpy(pMem, name.c_str(), text_length);
                GlobalUnlock(hMem);

                // Place the modified text on the clipboard
                if (SetClipboardData(CF_TEXT, hMem) == nullptr)
                {
                    cout << "Failed to set clipboard data" << endl;
                    GlobalFree(hMem); // Free memory if SetClipboardData fails
                }
                else
                {
                    cout << "Modified text copied to clipboard" << endl;
                }
            }
            else
            {
                cout << "Failed to lock memory for clipboard" << endl;
                GlobalFree(hMem);
            }
        }
        else
        {
            cout << "Failed to allocate memory for clipboard" << endl;
        }

        CloseClipboard();
    }
    else
    {
        cout << "Cannot open clipboard to write data" << endl;
    }

    cout << "Modifyed text: " << name << endl;

    return 0;
}
