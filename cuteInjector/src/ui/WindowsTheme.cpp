#include <QSettings>
#include "WindowsTheme.h"
#include "src/definitions/Themes.h"


void WindowsTheme::setDarkTitleBarWindows(HWND hwnd)
{
    HMODULE uxthemeLib = LoadLibraryExW(L"uxtheme.dll", NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
    HMODULE user32Lib = GetModuleHandleW(L"user32.dll");

    fnAllowDarkModeForWindow AllowDarkModeForWindow = reinterpret_cast<fnAllowDarkModeForWindow>(
    GetProcAddress(uxthemeLib, MAKEINTRESOURCEA(133))
    );
    fnSetPreferredAppMode SetPreferredAppMode = reinterpret_cast<fnSetPreferredAppMode>(
    GetProcAddress(uxthemeLib, MAKEINTRESOURCEA(135))
    );
    fnSetWindowCompositionAttribute SetWindowCompositionAttribute = reinterpret_cast<fnSetWindowCompositionAttribute>(
    GetProcAddress(user32Lib, "SetWindowCompositionAttribute")
    );

    BOOL dark = TRUE;
    SetPreferredAppMode(AllowDark);
    AllowDarkModeForWindow(hwnd, dark);

    WINDOWCOMPOSITIONATTRIBDATA data = {
        WCA_USEDARKMODECOLORS,
                &dark,
                sizeof(dark)
    };

    SetWindowCompositionAttribute(hwnd, &data.Attrib);
}


QString WindowsTheme::getWindowsSystemTheme() {
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
                       QSettings::NativeFormat);
    if (settings.value("AppsUseLightTheme") == 0) {
        return SYSTEMTHEMES.DARK;
    }

    return SYSTEMTHEMES.LIGHT;
}
