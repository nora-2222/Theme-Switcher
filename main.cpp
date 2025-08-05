#include <iostream>
#include <windows.h>
#include <conio.h>

using std::cout;
using std::endl;
using std::wcerr;

const int LIGHT_THEME = 1;
const int DARK_THEME = 2;
const int EXIT = 0;

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
  DWORD value = (theme == LIGHT_THEME) ? 1 : 0;

  const wchar_t* regPath = L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize";

  LONG openResult = RegOpenKeyEx(HKEY_CURRENT_USER, regPath, 0, KEY_SET_VALUE, &hKey);
  if (openResult != ERROR_SUCCESS) {
    wcerr << L"Registry access failed. Error code: " << openResult << endl;
    return false;
  }

  bool setResult = (RegSetValueEx(hKey, L"AppsUseLightTheme", 0, REG_DWORD, (BYTE*)&value, sizeof(value)) == ERROR_SUCCESS) &&
    (RegSetValueEx(hKey, L"SystemUsesLightTheme", 0, REG_DWORD, (BYTE*)&value, sizeof(value)) == ERROR_SUCCESS);

  RegCloseKey(hKey);

  SendMessage(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)L"ImmersiveColorSet");
  SystemParametersInfoW(SPI_SETDESKWALLPAPER, 0, NULL, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);

  return setResult;
}

void printResult(bool success) {
  if (success) {
    cout << "\nTheme changed successfully.\n";
  }
  else {
    cout << "\nFailed to change theme.\n";
  }
  cout << "Press Enter to return to the menu...";

  char ch;
  do {
    ch = _getch();
  } while (ch != '\r');

  cout << "\n";
}

char getValidChoice() {
  char choice;
  do {
    printMenu();
    choice = _getch();
    if (choice != '0' && choice != '1' && choice != '2') {
      cout << "\nInvalid selection. Please try again.\n";
      Sleep(1000);
    }
  } while (choice != '0' && choice != '1' && choice != '2');
  return choice;
}

int main() {
  while (true) {
    char choice = getValidChoice();
    cout << choice << "\n";

    if (choice == '0') break;

    if (choice == '1' || choice == '2') {
      bool success = changeTheme(choice == '1' ? LIGHT_THEME : DARK_THEME);
      printResult(success);
    }
  }

  return 0;
}
