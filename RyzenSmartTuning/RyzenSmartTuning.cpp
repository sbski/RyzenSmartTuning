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
#ifdef __linux__
#include <unistd.h>
#endif




#include <AMDTPowerProfileApi.h>

#include <ryzenadj.h>

#include <UProfData.h>
#include <RyzenSmartTuning.h>




int main()
{
    fprintf(stdout, "Enter In 0 for any setting you do not want to be applied\n");
    uint32_t userSettings[22];
    uint32_t temp;
    const char* description[22] = { "STAPM Limit (mW)", "Fast Limit (Mw)", "Slow Limit (Mw)",  "Slow Time (S)",
    "STAPM Time (S)", "Tctl Temperature (Degrees C)", "VRM Current Limit (mA)", "VRM SoC Current Limit (mA)",
        "VRM Maximum Current Limit (mA)", "VRM SoC Maximum Current Limit (mA)", "PSI0 Current Limit (mA)", "PSI0 SoC Current Limit(mA)",
        "Maximum SoC Clock Frequency(MHz)", "Minimum SoC Clock Frequency(MHz)", "Maximum Transmission(CPU - GPU) Frequency(MHz)", "Minimum Transmission(CPU - GPU) Frequency(MHz)",
        "Maximum Video Core Next(VCE - Video Coding Engine) (Value)", "Minimum Video Core Next(VCE - Video Coding Engine) (Value)", "Maximum Data Launch Clock(Value)", "Minimum Data Launch Clock(Value)",
        "Maximum GFX Clock(Value)", "Minimum GFX Clock(Value)" };
    for (int i = 0; i < 22; i++)
    {
        fprintf(stdout, "Enter In %s: ", description[i]);
        std::cin >> temp;
        userSettings[i] = temp;
    }
    RyzenSmartTuning ryzenST(true, 100, true, userSettings);
    system("cls");




    //ryzenST.startUProf();
    ryzenST.initRyzenAdj();
    if (ryzenST.ryzenAccess == NULL) {
        printf("Unable to init ryzenadj, check permission\n");
        system("pause");
    }
    ryzenST.startRST();
    
    /*
    UProfData test;
    system("cls");
    test.initalizeUProf(false, false, true);
    test.startProf();
    
    while (true)
    {
        test.update();
    }
    test.stopProf();
    system("pause");
    */
    return 0;
}
