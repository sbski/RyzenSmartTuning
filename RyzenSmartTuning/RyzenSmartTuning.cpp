// RyzenSmartTuning.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>
#include <thread>
#ifdef __linux__
#include <unistd.h>
#endif


#include <AMDTPowerProfileApi.h>

#include <ryzenadj.h>

#include "UProfData.h"




int main()
{

    AMDTUInt32 smplInter;
    std::cout << "Enter the desired sampleing interval: ";
    std::cin >> smplInter;
    UProfData test(smplInter, false, false, true);
    system("cls");
    test.startProf();
    test.update();
    test.update();
    test.update();
    system("pause");
    

    uint32_t targetStapmLimit = 28000;
    uint32_t targetFastLimit = 32000;
    uint32_t targetSlowLimit = 30000;

    ryzen_access ry;
    ry = init_ryzenadj();
    
    set_fast_limit(ry, targetFastLimit);
    set_slow_limit(ry, targetSlowLimit);
    set_stapm_limit(ry, targetStapmLimit);
    
    while (true)
    {
        test.update();
    }
    test.stopProf();
    system("pause");

    return 0;
}
