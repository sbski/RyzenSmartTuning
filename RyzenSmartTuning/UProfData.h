#pragma once


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



public:
    //IDs of important counters
    AMDTUInt32 socketPowerId;
    AMDTUInt32 stapmLimitId;
    AMDTUInt32 fastLimitId;
    AMDTUInt32 slowLimitId;

    //descriptions of important counters
    std::string socketPowerDesc;
    std::string stapmLimitDesc;
    std::string fastLimitDesc;
    std::string slowLimitDesc;

    float lastSocketPower;
    float lastStapmLimit;
    float lastFastLimit;
    float lastSlowLimit;

    UProfData()
    {
        //UProfData(100);
    }

    UProfData(AMDTUInt32 setInterval)
    {
        nbrSamples = 0;
        pSampleData = nullptr;

        // Thread data
        threadCount = std::thread::hardware_concurrency();

        nbrCounters = 0;
        pCounters = nullptr;

        //setting up sampling interval
        samplingInterval = setInterval;
        nbrSamples = 0;

        initializeUProf(true, true, true);
    }

    AMDTUInt32 getStapmLimit()
    {
        return 0;
    }

    AMDTUInt32 getFastLimit()
    {
        return 0;
    }

    AMDTUInt32 getSlowLimit()
    {
        return 0;
    }

    bool update()
    {
        return (print(false));
    }

    bool print()
    {
        return print(true);
    }

    bool print(bool notUpdate)
    {

        //do while loop for the ability to break out if needed
        do
        {
            //std::cout << "in loop: " << "\n";
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
                return false;
                continue;
                
            }
            if (notUpdate)
            {
                system("cls");
            }
            //std::cout << nbrSamples << "\n";
            if (nullptr != pSampleData)
            {
                // iterate over all the samples and report the sampled counter values
                for (AMDTUInt32 idx = nbrSamples - 1; idx < nbrSamples; idx++)
                {
                    //std::cout << "idx =" << idx <<"\n";
                    //system("pause");
                    // Iterate over the sampled counter values and print
                    for (unsigned int i = 0; i < pSampleData[idx].m_numOfCounter; i++)
                    {
                        if (nullptr != pSampleData[idx].m_counterValues)
                        {
                            // Get the counter descriptor to print the counter name
                            AMDTPwrCounterDesc counterDesc;
                            hResult = AMDTPwrGetCounterDesc(pSampleData[idx].m_counterValues->m_counterID, &counterDesc);

                            std::string temp = counterDesc.m_name;
                            if (socketPowerDesc.find(temp) != std::string::npos)
                            {
                                lastSocketPower = pSampleData[idx].m_counterValues->m_data;
                                //std::cout << lastSocketPower << "\n";

                            }
                            else if (stapmLimitDesc.find(temp) != std::string::npos)
                            {
                                lastStapmLimit = pSampleData[idx].m_counterValues->m_data;
                                //std::cout << lastSlowLimit << "\n";

                            }
                            else if (fastLimitDesc.find(temp) != std::string::npos)
                            {
                                lastFastLimit = pSampleData[idx].m_counterValues->m_data;
                                //std::cout << lastSlowLimit << "\n";

                            }
                            else if (slowLimitDesc.find(temp) != std::string::npos)
                            {
                                lastSlowLimit = pSampleData[idx].m_counterValues->m_data;
                                //std::cout << lastSlowLimit << "\n";
                            }

                            
                            //combining an update function and the print funciton as one
                            if (notUpdate)
                                fprintf(stdout, "%s : %f\n", counterDesc.m_name, pSampleData[nbrSamples - 1].m_counterValues->m_data);

                            pSampleData[idx].m_counterValues++;
                        }
                    } // iterate over the sampled counters
                }

            }
        } while (false);
        return true;

    }

    

    // sets uprof up for starting
    void initializeUProf()
    {
        initializeUProf(true, true, true);
    }

    bool initializeUProf(bool readPState, bool readCorePower, bool readFrequency)
    {
        //try to initialize online mode
        try
        {
            hResult = AMDTPwrProfileInitialize(AMDT_PWR_MODE_TIMELINE_ONLINE);
        }
        catch (int e)
        {
            std::cout << "An exception occurred. Exception Nr. " << e << '\n';
            system("pause");
        }
        //assert that it does
        assert(AMDT_STATUS_OK == hResult);


        // passing off the sapling interval
        hResult = AMDTPwrSetTimerSamplingPeriod(100);
        //std::cout << hResult << "\n";
        //assert(AMDT_STATUS_OK == hResult);


        //system("pause");
        AMDTPwrCounterDesc tempDesc;
        // Grabbing the IDs of important counters
        hResult = AMDTPwrGetCounterId(AMD_PWR_SOCKET_POWER, &socketPowerId);
        assert(AMDT_STATUS_OK == hResult);
        //grabbing the description of important counters
        hResult = AMDTPwrGetCounterDesc(socketPowerId, &tempDesc);
        assert(AMDT_STATUS_OK == hResult);
        //storing the name into the proper variable
        socketPowerDesc = tempDesc.m_name;
        //fprintf(stdout, "%s\n", tempDesc.m_name);

        hResult = AMDTPwrGetCounterId(AMD_PWR_SOCKET_STAPM_LIMIT, &stapmLimitId);
        assert(AMDT_STATUS_OK == hResult);
        hResult = AMDTPwrGetCounterDesc(stapmLimitId, &tempDesc);
        assert(AMDT_STATUS_OK == hResult);
        stapmLimitDesc = tempDesc.m_name;
        //fprintf(stdout, "%s\n", tempDesc.m_name);

        hResult = AMDTPwrGetCounterId(AMD_PWR_SOCKET_PPT_FAST_LIMIT, &fastLimitId);
        assert(AMDT_STATUS_OK == hResult);
        hResult = AMDTPwrGetCounterDesc(fastLimitId, &tempDesc);
        assert(AMDT_STATUS_OK == hResult);
        fastLimitDesc = tempDesc.m_name;
        //fprintf(stdout, "%s\n", tempDesc.m_name);

        hResult = AMDTPwrGetCounterId(AMD_PWR_SOCKET_PPT_SLOW_LIMIT, &slowLimitId);
        assert(AMDT_STATUS_OK == hResult);
        hResult = AMDTPwrGetCounterDesc(slowLimitId, &tempDesc);
        assert(AMDT_STATUS_OK == hResult);
        slowLimitDesc = tempDesc.m_name;
        //fprintf(stdout, "%s\n", tempDesc.m_name);

        //get the number of counters
        hResult = AMDTPwrGetSupportedCounters(&nbrCounters, &pCounters);
        //assurt results
        assert(AMDT_STATUS_OK == hResult);

        //setting the pointer of pCurrCounter
        pCurrCounter = pCounters;


        AMDTUInt32 nbrSamples = 0;



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

                bool isPState = currName.find("P-State") != std::string::npos;
                bool isCorePower = currName.find("Core") != std::string::npos && currName.find("Power") != std::string::npos;
                bool isFrequency = currName.find(" Effective Frequency") != std::string::npos;

                if ((isPState && !readPState) || (isCorePower && !readCorePower) || (isFrequency && !readFrequency))
                {
                    //these are not the counters you are looking for
                }
                else
                {
                    // Enable all the counters
                    hResult = AMDTPwrEnableCounter(pCurrCounter->m_counterID);
                    // Assert that the counter enabled properly
                    assert(AMDT_STATUS_OK == hResult || AMDT_ERROR_COUNTER_ALREADY_ENABLED == hResult);

                }
            }
        }
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