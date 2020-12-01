//берем id жесткого диска
#include <iostream>
#include <windows.h>


int GetHDD()
{
    // Получаем тип носителя с которого запущена программа
    unsigned int drive_type = GetDriveType(NULL);

    char VolumeNameBuffer[100];
    char FileSystemNameBuffer[100];
    LPDWORD sz = NULL, fs = NULL;
    unsigned long drive_sn;
    GetVolumeInformationA(NULL, VolumeNameBuffer, 100, &drive_sn, sz, fs, FileSystemNameBuffer, 100);
    return drive_sn;
}

int main(){
    int hdd=GetHDD();
    std::cout<<hdd<<std::endl;
}