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

#include <UProfData.h>
#include <RyzenSmartTuning.h>




int main()
{
    
    RyzenSmartTuning ryzenST(true, 100, true);
    ryzenST.targetFastLimit = 32000;
    ryzenST.targetSlowLimit= 30000;
    ryzenST.targetStapmLimit = 28000;
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
