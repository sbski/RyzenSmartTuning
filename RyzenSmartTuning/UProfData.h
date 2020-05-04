#pragma once


<<<<<<< Updated upstream
=======
<<<<<<<< Updated upstream:Visual Sudio/RyzenSmartTuning/RyzenSmartTuning/RyzenSmartTuning.cpp

#include <AMDTPowerProfileApi.h>

#include <ryzenadj.h>
#include  "nb_smu_ops.h"
========
>>>>>>> Stashed changes
#include <AMDTPowerProfileApi.h>
#include <assert.h>


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
<<<<<<< Updated upstream
    UProfData(int setInterval)
    {
=======

    UProfData()
    {
        UProfData(100);
    }
    UProfData(int setInterval)
    {
        UProfData(setInterval, false, false, false);
    }

    UProfData(int setInterval, bool readPState, bool readCorePower, bool readFrequency)
    {
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream

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
=======
        




        //enable counters
        for (AMDTUInt32 cnt = 0; cnt < nbrCounters; cnt++, pCurrCounter++)
        {
            //check to see that we do not have a null pointer
            if (nullptr != pCurrCounter)
            {
                //to get the counter Desc
                AMDTPwrCounterDesc counterDesc;

                AMDTPwrGetCounterDesc(pCurrCounter->m_counterID, &counterDesc);
                std::string currName = counterDesc.m_name;
                
                fprintf(stdout, "%s\n", counterDesc.m_name);
                bool isPState = currName.find("P-State") != std::string::npos;
                bool isCorePower = currName.find("Core") != std::string::npos && currName.find("Power") != std::string::npos;
                bool isFrequency = currName.find(" Effective Frequency") != std::string::npos;
                
                
                if ((isPState && !readPState) || (isCorePower && !readCorePower) || (isFrequency && !readFrequency))
                {
                    //these are the counter that we do not want
                }
                else
                {
                    // Enable all the counters
                    hResult = AMDTPwrEnableCounter(pCurrCounter->m_counterID);
                    // Assert    
                    assert(AMDT_STATUS_OK == hResult);

                }
            }
        }

        //setting up sampling interval
        samplingInterval = setInterval;
        nbrSamples = 0;

        // passing off the sapling interval
>>>>>>> Stashed changes
        hResult = AMDTPwrSetTimerSamplingPeriod(samplingInterval);
        assert(AMDT_STATUS_OK == hResult);


        //system("pause");





    }


    bool update()
    {

        do
        {
<<<<<<< Updated upstream
=======
            
>>>>>>> Stashed changes
            // sleep for refresh duration - at least equivalent to the sampling interval specified
#if defined ( WIN32 )
        // Windows
            Sleep(samplingInterval);
#else
        // Linux
            usleep(samplingInterval * 1000);
#endif
<<<<<<< Updated upstream

=======
            
>>>>>>> Stashed changes
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
                for (AMDTUInt32 idx = nbrSamples - 1; idx < nbrSamples; idx++)
                {
                    // Iterate over the sampled counter values and print
                    for (unsigned int i = 0; i < pSampleData[idx].m_numOfCounter; i++)
                    {
                        if (nullptr != pSampleData[idx].m_counterValues)
                        {
                            // Get the counter descriptor to print the counter name
                            AMDTPwrCounterDesc counterDesc;
                            AMDTPwrGetCounterDesc(pSampleData[idx].m_counterValues->m_counterID, &counterDesc);
<<<<<<< Updated upstream

                            fprintf(stdout, "%s : %f\n", counterDesc.m_name, pSampleData[idx].m_counterValues->m_data);

                            pSampleData[idx].m_counterValues++;
                        }
                    } // iterate over the sampled counters

                    fprintf(stdout, "\n");
                } // iterate over all the samples collected

=======
>>>>>>>> Stashed changes:RyzenSmartTuning/UProfData.h

#include <UProfData.h>



<<<<<<<< Updated upstream:Visual Sudio/RyzenSmartTuning/RyzenSmartTuning/RyzenSmartTuning.cpp
========
>>>>>>> Stashed changes
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
<<<<<<< Updated upstream

    // Assert that Uprof is working fine and to prevent crashes
    bool assertStatus(AMDTResult hResult)
    {
        assert(AMDT_STATUS_OK == hResult);
        return true;
    }


};
=======
>>>>>>>> Stashed changes:RyzenSmartTuning/UProfData.h

int main()
{
    ryzen_access ry;
    ry = init_ryzenadj();
    uint32_t stapmLimit = 28000;


<<<<<<<< Updated upstream:Visual Sudio/RyzenSmartTuning/RyzenSmartTuning/RyzenSmartTuning.cpp
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
========
};
>>>>>>>> Stashed changes:RyzenSmartTuning/UProfData.h
>>>>>>> Stashed changes
