// RyzenSmartTuning.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>
#include <thread>
#include <cstdint>
#include <fstream>
#include<sstream> 
using namespace std;
#ifdef __linux__
#include <unistd.h>
#endif



//might need 
#include <AMDTPowerProfileApi.h>

#include <ryzenadj.h>

#include <UProfData.h>
#include <RyzenSmartTuning.h>




int main()
{
    string currentSetting;
    ifstream inputSettingsFile;
    ofstream createSettingsFile;
    
    AMDTUInt32 customSamplingInterval = 100;

    uint32_t userSettings[22];
    uint32_t temp;
    //opening out setttings file
    inputSettingsFile.open("settings.txt");

    int userSettingsSize = *(&userSettings + 1) - userSettings;
    int totalUserOptions = 24;

    bool enableLog;

    if (inputSettingsFile.is_open())
    {
        int i = 0;
        std::cout << userSettingsSize << "\n";
        //reading in file into userSettings if it exists
        for (int i = 0; i < totalUserOptions; i++)
        {
            std::cout << i << "| ";
            if (i < 22)
            {
                getline(inputSettingsFile, currentSetting);
                std::string::size_type sz;   // alias of size_t
                temp = std::stoi(currentSetting, &sz);

                //std::cout << currentSetting << std::endl;
                std::cout << "Loaded RyzenAdj: " << temp << std::endl;
                userSettings[i] = temp;
            }
            else
            {
                switch (i)
                {
                case 22:
                    getline(inputSettingsFile, currentSetting);
                    std::string::size_type sz;   // alias of size_t
                    temp = std::stoi(currentSetting, &sz);

                    //std::cout << currentSetting << std::endl;
                    std::cout << "Loaded enableLog: " << temp << std::endl;
                    enableLog = (temp == 1);
                    break;
                case 23:
                    getline(inputSettingsFile, currentSetting);
                    temp = std::stoi(currentSetting, &sz);

                    //std::cout << currentSetting << std::endl;
                    std::cout << "Loaded customSamplingInterval: " << temp << std::endl;
                    customSamplingInterval = temp;
                    //system("pause");
                    break;
                }

               
            }
            
        }


        inputSettingsFile.close();
    }
    else
    {
        inputSettingsFile.close();
        createSettingsFile.open("settings.txt");
        if (createSettingsFile.is_open())
        {
            cout << "Created New Setings File\n";

        }





        //user input
        fprintf(stdout, "--------------------------------------------------------\nEnter In 0 for any setting you do not want to be applied\n--------------------------------------------------------\n");
        //all of the descriptions here with the index of where they go when the go off to RST
        const char* description[24] = {"0. STAPM Limit (W)", "1. Fast Limit (W)", "2. Slow Limit (W)",  "3. Slow Time (Package Power Tracking (PPT) - Slow period) (S)",
        "4. STAPM Time (CPU Boost Period) (S)", "5. Tctl Temperature (Degrees C)", "6. VRM Current Limit (A)", "7. VRM SoC Current Limit (mA)",
            "8. VRM Maximum Current Limit (A)", "9. VRM SoC Maximum Current Limit (A)", "10. PSI0 Current Limit (mA)", "11. PSI0 SoC Current Limit(mA)",
            "12. Maximum SoC Clock Frequency(MHz)", "13. Minimum SoC Clock Frequency(MHz)", "14. Maximum Transmission(Infinity Fabric) Frequency(MHz)", "15. Minimum Transmission(Infinity Fabric) Frequency(MHz)",
            "16. Maximum Video Core Next(VCE - Video Coding Engine) (Value)", "17. Minimum Video Core Next(VCE - Video Coding Engine) (Value)", "18. Maximum Data Launch Clock(Value)", "19. Minimum Data Launch Clock(Value)",
            "20. Maximum GFX Clock(Value)", "21. Minimum GFX Clock(Value)", "Enable log (0: No, 1: Yes)", "sampling interval"};
        //removing extra options for now
        for (int i = 0; i < 24; i++)
        {
            if (((i < 9 || i > 19   ) && i != 3 && i != 6 && i != 7) || i == 10 || i == 14 || i == 15&&true)
            {
                


                fprintf(stdout, "Enter In %s: ", description[i]);
                std::cin >> temp;
                //miltiplying limits that need to be
                if (i == 0 || i == 1 || i == 2 || i == 6 || i == 7 || i == 8 || i == 9)
                {
                    temp *= 1000;
                }
                if (i != 23)
                {
                    userSettings[i] = temp;
                }
                else
                {
                    customSamplingInterval = temp;
                }
                

                stringstream ss;
                ss << temp;
                ss >> currentSetting;

                createSettingsFile << currentSetting << '\n';
            }
            else
            {
                userSettings[i] = 0;

                createSettingsFile << "0\n";
            }
            
        }
        createSettingsFile.close();
    }


    //user input
   
    
    //std::system("pause");
    
    if (customSamplingInterval < 100)
    {
        customSamplingInterval = 100;
    }

    //sending all the data off to RST
    RyzenSmartTuning ryzenST(true, customSamplingInterval, true, userSettings, enableLog);
    std::system("cls");




    //initing RyzenAdj
    ryzenST.initRyzenAdj();
    if (ryzenST.ryzenAccess == NULL) {
        printf("Unable to init ryzenadj, check permission\n");
        std::system("pause");
    }


    /*
    ifstream lastPosition("lastMsgID.txt");
    string strID;
    int messageID;
    getline(lastPosition, strID);
    std::istringstream(strID) >> messageID;
    if (messageID <= 0)
    {
        std::cout << "LastMsgID not found please enter the last Message ID: ";
        std::cin >> messageID;
    }
    //ryzenST.startTactiPoke(messageID);*/
    //ryzenST.startStapmMulti(200, 200, 3);

    //ryzenST.findSafePower(65, 85, 15000, 3);

    //starting RST
    //ryzenST.startRSTBenchMarkMode();
    //ryzenST.startTestRST();
    ryzenST.startSmartTune(75, 40000, 15000);

    return 0;
}
