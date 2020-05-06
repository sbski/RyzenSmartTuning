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
    const char* description[22] = { "0. STAPM Limit (W)", "1. Fast Limit (W)", "2. Slow Limit (W)",  "3. Slow Time (S)",
    "4. STAPM Time (S)", "5. Tctl Temperature (Degrees C)", "6. VRM Current Limit (A)", "7. VRM SoC Current Limit (mA)",
        "8. VRM Maximum Current Limit (A)", "9. VRM SoC Maximum Current Limit (A)", "10. PSI0 Current Limit (mA)", "11. PSI0 SoC Current Limit(mA)",
        "12. Maximum SoC Clock Frequency(MHz)", "13. Minimum SoC Clock Frequency(MHz)", "14. Maximum Transmission(CPU - GPU) Frequency(MHz)", "15. Minimum Transmission(CPU - GPU) Frequency(MHz)",
        "16. Maximum Video Core Next(VCE - Video Coding Engine) (Value)", "17. Minimum Video Core Next(VCE - Video Coding Engine) (Value)", "18. Maximum Data Launch Clock(Value)", "19. Minimum Data Launch Clock(Value)",
        "20. Maximum GFX Clock(Value)", "21. Minimum GFX Clock(Value)" };
    for (int i = 0; i < 22; i++)
    {
        if ((i < 9 || i > 19) && i != 3 && i != 4 && i != 6 && i != 7)
        {
        
            fprintf(stdout, "Enter In %s: ", description[i]);
            std::cin >> temp;
            if (i == 0 || i == 1 || i == 2 || i == 6 || i == 7 || i == 8 || i == 9)
            {
                temp *= 1000;
            }
            userSettings[i] = temp;
        }
        else
        {
            userSettings[i] = 0;
        }
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
