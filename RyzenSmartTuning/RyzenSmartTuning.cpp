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
    ryzen_access ry;
    ry = init_ryzenadj();
    uint32_t stapmLimit = 28000;


    set_stapm_limit(ry, stapmLimit);

    AMDTUInt32 smplInter;
    std::cout << "Enter the desired sampleing interval: ";
    std::cin >> smplInter;
    UProfData test(smplInter);
    system("cls");
    test.startProf();
    while (true)
    {
        
        test.update();
        
    }
    
    test.stopProf();
    system("pause");

    return 0;
}
