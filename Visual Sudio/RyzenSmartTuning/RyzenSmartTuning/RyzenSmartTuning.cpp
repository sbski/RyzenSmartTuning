// RyzenSmartTuning.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>
#ifdef __linux__
#include <unistd.h>
#endif

// TODO: The header file needs to be removed
// Need modification in source program.
#include <AMDTPowerProfileApi.h>


int main()
{
    AMDTResult hResult = AMDT_STATUS_OK;
    // Initialize online mode
    hResult = AMDTPwrProfileInitialize(AMDT_PWR_MODE_TIMELINE_ONLINE);
    // --- Handle the error

    // Configure the profile run
    //   1. Get the supported counters
    //   2. Enable all the counter
    //   3. Set the timer configuration

    // 1. Get the supported counter details
    AMDTUInt32 nbrCounters = 0;
    AMDTPwrCounterDesc* pCounters = nullptr;

    hResult = AMDTPwrGetSupportedCounters(&nbrCounters, &pCounters);
    assert(AMDT_STATUS_OK == hResult);

    AMDTPwrCounterDesc* pCurrCounter = pCounters;


    int limitIndex = 0; //not being used currently
    float targetSTAPM_Limit = 28;

    int slowIndex = 0;
    float targetSlowValue = 30;
    float slowValue = 0;

    int fastIndex = 0;
    float targetFastValue = 32;
    float fastValue = 0;

    int temperatureIndex = 0;
    float targetTemperatureValue = 85;
    float temperatureValue = 0;

    int targetGfxMax = 700;

    bool applySettings = false;
    
    bool justShowMismach = true;



    //--------------------------------------------------------------------------------------------------------------------
    // Neeed to add array to grab all the info here so that it's not done realtime (for efficency.)
    //--------------------------------------------------------------------------------------------------------------------


    for (AMDTUInt32 cnt = 0; cnt < nbrCounters; cnt++, pCurrCounter++)
    {
        if (nullptr != pCurrCounter)
        {
            // Enable all the counters
            hResult = AMDTPwrEnableCounter(pCurrCounter->m_counterID);
            assert(AMDT_STATUS_OK == hResult);
        }
    }

    
    
    
    // Set the timer configuration
    AMDTUInt32 samplingInterval = 500;      // in milliseconds
    AMDTUInt32 profilingDuration = 1000000;      // in seconds



    //setting the sampling period to the defined sampling interval
    hResult = AMDTPwrSetTimerSamplingPeriod(samplingInterval);
    assert(AMDT_STATUS_OK == hResult);



    // Start the Profile Run
    hResult = AMDTPwrStartProfiling();
    assert(AMDT_STATUS_OK == hResult);

    // Collect and report the counter values periodically
    //   1. Take the snapshot of the counter values
    //   2. Read the counter values
    //   3. Report the counter values

    volatile bool isProfiling = true;
    bool stopProfiling = false;
    AMDTUInt32 nbrSamples = 0;
    
    while (isProfiling)
    {
        // sleep for refresh duration - at least equivalent to the sampling interval specified
#if defined ( WIN32 )
        // Windows
        //Sleep(samplingInterval);
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

        if (nullptr != pSampleData)
        {
            // iterate over all the samples and report the sampled counter values
            for (AMDTUInt32 idx = 0; idx < nbrSamples; idx++)
            {

                system("CLS");
                // Iterate over the sampled counter values and print
                for (unsigned int i = 0; i < pSampleData[idx].m_numOfCounter; i++)
                {
                    if (nullptr != pSampleData[idx].m_counterValues)
                    {
                        // Get the counter descriptor to print the counter name
                        AMDTPwrCounterDesc counterDesc;
                        AMDTPwrGetCounterDesc(pSampleData[idx].m_counterValues->m_counterID, &counterDesc);

                        fprintf(stdout, "%s : %f\n", counterDesc.m_name, pSampleData[idx].m_counterValues->m_data);

                        if (strcmp(counterDesc.m_name, "Socket0 PPT Fast Limit") == 0)
                        {
                            fastIndex = i;
                            fastValue = pSampleData[idx].m_counterValues->m_data;
                            
                            //fprintf(stdout, "index: %i value: %f\n", i, fastValue);
                            //system("PAUSE");
                        }
                        else if (strcmp(counterDesc.m_name, "Socket0 PPT Slow Limit") == 0)
                        {
                            slowIndex = i;
                            slowValue = pSampleData[idx].m_counterValues->m_data;
                            //fprintf(stdout, "index: %i value: %f\n", i, slowValue);
                        }
                        else if (strcmp(counterDesc.m_name, "Socket0 Temperature") == 0)
                        {
                            temperatureIndex = i;
                            temperatureValue = pSampleData[idx].m_counterValues->m_data;
                            //fprintf(stdout, "index: %i value: %f\n", i, temperatureValue);
                        }



                        pSampleData[idx].m_counterValues++;
                    }

                    


                } // iterate over the sampled counters



                //check that the actual values mach 
                //AMDTPwrCounterDesc counterDesc;
                //AMDTPwrGetCounterDesc(pSampleData[idx].m_counterValues->m_counterID, &counterDesc);

                if (round(fastValue) != round(targetFastValue))
                {
                    fprintf(stdout, "Fast Mismach: %f | %f\n", fastValue, targetFastValue);
                    applySettings = TRUE;
                }
                if (round(slowValue) != round(targetSlowValue))
                {
                    fprintf(stdout, "Slow Mismach: %f | %f\n", slowValue, targetSlowValue);
                    applySettings = TRUE;
                }
                if (temperatureValue >= targetTemperatureValue)
                {
                    fprintf(stdout, "Temperature Mismach: %f | %f\n", temperatureValue, targetTemperatureValue);
                    applySettings = TRUE;
                }

                if (applySettings)
                {
                    if (justShowMismach)
                    {
                        fprintf(stdout, "\n-------------------\n- change detected -\n-------------------");
                    }
                    else
                    {
                        system("ryzenadj.exe --stapm-limit=28000 --fast-limit=32000 --slow-limit=30000 --tctl-temp=85");
                        fprintf(stdout, "Settings Applied!");
                    }

                    //system("TIMEOUT 1");
                    applySettings = FALSE;
                }



                //fprintf(stdout, "\n\n\n\n");
            } // iterate over all the samples collected

            // check if we exceeded the profile duration
            if ((profilingDuration > 0)
                && (pSampleData->m_elapsedTimeMs >= (profilingDuration * 1000)))
            {
                stopProfiling = true;
            }

            if (stopProfiling)
            {
                // stop the profiling
                hResult = AMDTPwrStopProfiling();
                assert(AMDT_STATUS_OK == hResult);
                isProfiling = false;
            }
        }
    }

    // Close the profiler
    hResult = AMDTPwrProfileClose();
    assert(AMDT_STATUS_OK == hResult);

    return hResult;
}
