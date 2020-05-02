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

// TODO: The header file needs to be removed
// Need modification in source program.
#include <AMDTPowerProfileApi.h>


//integration of RyzenADJ
class RyzenAdj
{

};

class UProfData
{
    //-------------------------- Other data --------------------------
    AMDTUInt32 threadCount;

    AMDTUInt32 nbrCounters;
    AMDTPwrCounterDesc* pCounters;
    AMDTPwrCounterDesc* pCurrCounter;
    AMDTResult hResult;
    
    AMDTUInt32 samplingInterval;       // in milliseconds
    //AMDTUInt32 profilingDuration;      // in seconds
    
    AMDTUInt32 nbrSamples;

    //pointer to sample data
    AMDTPwrSample* pSampleData;

    AMDTUInt32 blah = 0;


public:
    UProfData(int setInterval)
    {
        nbrSamples = 0;
        pSampleData = nullptr;
        
        // Thread data
        threadCount = std::thread::hardware_concurrency();

        nbrCounters = 0;
        pCounters = nullptr;

        //try to initialize online mode
        hResult = AMDTPwrProfileInitialize(AMDT_PWR_MODE_TIMELINE_ONLINE);
        //assert that it does
        assert(AMDT_STATUS_OK == hResult);


        //get the number of counters
        hResult = AMDTPwrGetSupportedCounters(&nbrCounters, &pCounters);
        //assurt results
        assert(AMDT_STATUS_OK == hResult);

        //setting the pointer of pCurrCounter
        pCurrCounter = pCounters;


        AMDTUInt32 nbrSamples = 0;
        
        //std::cout << nbrCounters << "\n";

        //enable all counters
        for (AMDTUInt32 cnt = 0; cnt < nbrCounters; cnt++, pCurrCounter++)
        {
            //std::cout << "         cnt: " << cnt << "\n";
            //std::cout << " nbrCounters: " << nbrCounters << "\n";
            //std::cout << "pCurrCounter: " << pCurrCounter << "\n";
            if (nullptr != pCurrCounter)
            {
                // Enable all the counters
                hResult = AMDTPwrEnableCounter(pCurrCounter->m_counterID);
                if (AMDT_STATUS_OK == hResult)
                {
                    //std::cout << "AMDT_STATUS_OK\n";
                    AMDTPwrCounterDesc counterDesc;
                    AMDTPwrGetCounterDesc(pCurrCounter->m_counterID, &counterDesc);
                    //fprintf(stdout, "%s\n", counterDesc.m_name);
                }
                else
                {
                    //std::cout << "Error: " << hResult << "\n";
                    //system("pause");
                }

                //assert 
                //assert(AMDT_STATUS_OK == hResult);
            }
            //else std::cout << "nullptr\n";
                
        }



        //setting up sampling interval
        samplingInterval = setInterval;
        nbrSamples = 0;
        hResult = AMDTPwrSetTimerSamplingPeriod(samplingInterval);
        assert(AMDT_STATUS_OK == hResult);
        

        //system("pause");


        
        
        
    }


    bool update()
    {
        
        do
        {
            // sleep for refresh duration - at least equivalent to the sampling interval specified
#if defined ( WIN32 )
        // Windows
            Sleep(samplingInterval);
#else
        // Linux
            usleep(samplingInterval * 1000);
#endif

            // read all the counter values
            AMDTPwrSample* pSampleData = nullptr;

            hResult = AMDTPwrReadAllEnabledCounters(&nbrSamples, &pSampleData);

            if (AMDT_STATUS_OK != hResult)
            {
                continue;
            }
            system("cls");

            if (nullptr != pSampleData)
            {
                // iterate over all the samples and report the sampled counter values
                for (AMDTUInt32 idx = nbrSamples-1; idx < nbrSamples; idx++)
                {
                    // Iterate over the sampled counter values and print
                    for (unsigned int i = 0; i < pSampleData[idx].m_numOfCounter; i++)
                    {
                        if (nullptr != pSampleData[idx].m_counterValues)
                        {
                            // Get the counter descriptor to print the counter name
                            AMDTPwrCounterDesc counterDesc;
                            AMDTPwrGetCounterDesc(pSampleData[idx].m_counterValues->m_counterID, &counterDesc);

                            fprintf(stdout, "%s : %f\n", counterDesc.m_name, pSampleData[idx].m_counterValues->m_data);

                            pSampleData[idx].m_counterValues++;
                        }
                    } // iterate over the sampled counters

                    fprintf(stdout, "\n");
                } // iterate over all the samples collected

            }
        } while (false);
        return true;
        
    }
    bool startProf()
    {
        hResult = AMDTPwrStartProfiling();
        return(AMDT_STATUS_OK == hResult);
    }
    bool stopProf()
    {
        hResult = AMDTPwrStopProfiling();
        return (AMDT_STATUS_OK == hResult);
    }

    // Assert that Uprof is working fine and to prevent crashes
    bool assertStatus(AMDTResult hResult)
    {
        assert(AMDT_STATUS_OK == hResult);
        return true;
    }


};

int main()
{
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
