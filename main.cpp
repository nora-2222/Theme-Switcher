#include <iostream>
#include <windows.h>
#include <conio.h>

using namespace std;

void printMenu() {
  system("cls");
  cout << "Windows Theme Switcher\n";
  cout << "https://github.com/nora-2222\n\n";
  cout << "1. Light Theme\n";
  cout << "2. Dark Theme\n";
  cout << "0. Exit\n\n";
  cout << "Select number: ";
}

bool changeTheme(int theme) {
  HKEY hKey;
  DWORD value = (theme == 1) ? 1 : 0;

  const wchar_t* regPath = L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize";

  if (RegOpenKeyEx(HKEY_CURRENT_USER, regPath, 0, KEY_SET_VALUE, &hKey) != ERROR_SUCCESS) {
    return false;
  }

  bool result = (RegSetValueEx(hKey, L"AppsUseLightTheme", 0, REG_DWORD, (BYTE*)&value, sizeof(value)) == ERROR_SUCCESS) &&
    (RegSetValueEx(hKey, L"SystemUsesLightTheme", 0, REG_DWORD, (BYTE*)&value, sizeof(value)) == ERROR_SUCCESS);

  RegCloseKey(hKey);

  SendMessage(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)L"ImmersiveColorSet");
  SystemParametersInfoW(SPI_SETDESKWALLPAPER, 0, NULL, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);

  return result;
}

void printResult(bool success) {
  if (success) {
    cout << "\ncomplete.\n";
  }
  else {
    cout << "\nfailed.\n";
  }
  cout << "Press Enter to return to the menu...";
  char ch;
  do {
    ch = _getch();
  } while (ch != '\r');

  cout << "\n";
}

int main() {
  while (true) {
    char choice;

    do {
      printMenu();
      choice = _getch();
    } while (choice != '0' && choice != '1' && choice != '2');

    cout << choice << "\n";

    if (choice == '0') break;
    if (choice == '1' || choice == '2') {
      bool success = changeTheme(choice - '0');
      printResult(success);
    }
  }

  return 0;
}
