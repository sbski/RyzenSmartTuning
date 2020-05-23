#pragma once
#ifndef _UPROFDATA_H_
#define _UPROFDATA_H_

//Imports needed for logging
#include <fstream>
#include<sstream> 
using namespace std;

#include <iostream>
#include <thread>
#include <time.h>

//imports needed for uProf
#include <AMDTPowerProfileApi.h>
#include <assert.h>


class UProfData
{
    
public:

    bool enableLog;
    bool firstPrint;
    bool didFirstPrint;

    //-------------------------- Other data --------------------------
    AMDTUInt32 threadIndex = 0;

    AMDTUInt32 nbrCounters;
    AMDTPwrCounterDesc* pCounters;
    AMDTPwrCounterDesc* pCurrCounter;
    AMDTResult hResult;

    AMDTUInt32 samplingInterval;       // in milliseconds
    //AMDTUInt32 profilingDuration;      // in seconds

    AMDTUInt32 nbrSamples;

    //pointer to sample data
    AMDTPwrSample* pSampleData;




    //creation of log
    ofstream outputLog;
    time_t now;

    std::string logName;

    //IDs of important counters
    AMDTUInt32 socketPowerId;
    AMDTUInt32 stapmLimitId;
    AMDTUInt32 fastLimitId;
    AMDTUInt32 slowLimitId;

    
    AMDTUInt32 stapmPowerId = 1;
    AMDTInt32 socTempId = 6;
    AMDTInt32 socketTempId = 61;
    AMDTInt32 threadFreqId[16];

    AMDTUInt32 numberOfSamples;

    //descriptions of important counters
    std::string socketPowerDesc;
    std::string stapmLimitDesc = "Socket0 STAPM Limit";
    std::string fastLimitDesc;
    std::string slowLimitDesc;
    std::string socketTempDesc = "Socket0 Temperature";
    std::string packagePowerDesc = "Socket0 Package Power";

    AMDTFloat32 lastSocketPower;
    AMDTFloat32 lastStapmLimit;
    AMDTFloat32 lastFastLimit;
    AMDTFloat32 lastSlowLimit;
    AMDTFloat32 lastPackagePower;

    AMDTFloat32 lastSocTemp;
    AMDTFloat32 lastSocketTemp;
    AMDTFloat32 lastStapmPower = -1;
    AMDTFloat32 lastThreadFreq[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

    AMDTUInt32 threadCount;


    //prevents false starts on uProf
    UProfData()
    {
        // Pringles aren't actually potato chips. Kinda like how this is actually not a constuctor, but a strategy for not triggering uProf. Don't trigger the uProf... It has drivers made by AMD after all.
    }

    UProfData(AMDTUInt32 setInterval, bool incomingEnableLog)
    {
        
        numberOfSamples = 0;
        firstPrint = true;
        didFirstPrint = false;

        enableLog = incomingEnableLog;
        nbrSamples = 0;
        pSampleData = nullptr;

        // Thread data
        threadCount = std::thread::hardware_concurrency();
        nbrCounters = 0;
        pCounters = nullptr;

        //setting up sampling interval
        samplingInterval = setInterval;
        nbrSamples = 0;

        //don't initalize uprof
    }

    
    


    //update() is really just a print(false)
    bool update()
    {
        std::string temp = getPrint(true, 0);
        return (&temp != nullptr);
    }


    std::string getPrint(bool notUpdate, AMDTUInt32 additionalLog)
    {
        std::string returnString = "";
        threadIndex = 0;
        //do while loop for the ability to break out if needed
        do
        {
            /*
            // sleep for refresh duration - at least equivalent to the sampling interval specified
#if defined ( WIN32 )
        // Windows
            if (samplingInterval < 50)
            {
                Sleep(50);

            }
            else
            {
                Sleep(samplingInterval);
            }
#else
        // Linux
            usleep(samplingInterval * 1000);
#endif
*/
            // read all the counter values
            AMDTPwrSample* pSampleData = nullptr;

            hResult = AMDTPwrReadAllEnabledCounters(&nbrSamples, &pSampleData);

            if (AMDT_STATUS_OK != hResult)
            {
                //Retries to read the data if it fails
                continue;

            }
            //std::cout << nbrSamples << "\n";
            if (nullptr != pSampleData)
            {
                // iterate over all the samples and report the sampled counter values
                for (AMDTUInt32 idx = 0; idx < nbrSamples; idx++)
                {
                    numberOfSamples++;

                    if (didFirstPrint)
                    {
                        firstPrint = false;
                    }
                    if (enableLog && !firstPrint)
                    {
                        while (!outputLog.is_open())
                        {
                            outputLog.open(logName);
                        }
                        outputLog << '\n';
                        outputLog.flush();
                    }

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

                            


                            //combining an update function and the print funciton as one
                            if (notUpdate && idx == (nbrSamples - 1))
                            {
                                stringstream ssName;
                                stringstream ssData;
                                stringstream ssCat;
                                ssName << counterDesc.m_name;
                                ssData << pSampleData[idx].m_counterValues->m_data;
                                ssCat << pSampleData[idx].m_counterValues->m_counterID;
                                returnString += ssName.str() + " : " + ssData.str() +'\n';

                                std::string temp = counterDesc.m_name;
                                if (socketPowerDesc.find(temp) != std::string::npos)
                                {
                                    lastSocketPower = pSampleData[idx].m_counterValues->m_data;
                                }
                                else if (stapmLimitDesc.find(temp) != std::string::npos)
                                {
                                    lastStapmLimit = pSampleData[idx].m_counterValues->m_data;
                                }
                                else if (fastLimitDesc.find(temp) != std::string::npos)
                                {
                                    lastFastLimit = pSampleData[idx].m_counterValues->m_data;
                                }
                                else if (slowLimitDesc.find(temp) != std::string::npos)
                                {
                                    lastSlowLimit = pSampleData[idx].m_counterValues->m_data;
                                }
                                else if (socketTempDesc.find(temp) != std::string::npos)
                                {
                                    lastSocketTemp = pSampleData[idx].m_counterValues->m_data;
                                }
                                else if (packagePowerDesc.find(temp) != std::string::npos)
                                {
                                    lastPackagePower = pSampleData[idx].m_counterValues->m_data;
                                }
                                else if (pSampleData[idx].m_counterValues->m_counterID == threadFreqId[threadIndex])
                                {
                                    lastThreadFreq[threadIndex] = pSampleData[idx].m_counterValues->m_data;
                                    threadIndex++;
                                }
                                else
                                {
                                    //std::cout << pSampleData[idx].m_counterValues->m_counterID << " | " << pSampleData[idx].m_counterValues->m_data << std::endl;
                                    switch (pSampleData[idx].m_counterValues->m_counterID)
                                    {
                                    case 1:
                                        lastStapmPower = AMDTFloat32(pSampleData[idx].m_counterValues->m_data);
                                        break;

                                    case 6:
                                        lastSocTemp = AMDTFloat32(pSampleData[idx].m_counterValues->m_data);
                                        break;

                                    case 66:
                                        lastSocketTemp = AMDTFloat32(pSampleData[idx].m_counterValues->m_data);
                                        break;
                                    }
                                }
                                //system("pause");
                            }
                            if (enableLog)
                            {
                                if (firstPrint)
                                {
                                    outputLog << counterDesc.m_name << ',';
                                    outputLog.flush();
                                    didFirstPrint = true;
                                }
                                else
                                {
                                    outputLog << pSampleData[idx].m_counterValues->m_data << ',';
                                    outputLog.flush();
                                }
                            }


                            pSampleData[idx].m_counterValues++;
                        }
                        if (enableLog)
                        {

                        }

                    } // iterate over the sampled counters
                    
                }
                if (additionalLog && nullptr != pSampleData)
                {

                    outputLog << additionalLog << ',';
                    outputLog.flush();

                    stringstream ssName;
                    stringstream ssData;
                    ssName << "Additional Info: ";
                    ssData << additionalLog;
                    returnString += ssName.str() + " : " + ssData.str() + '\n';
                }
            }
        } while (false);
        
        return returnString;

    }

    

    // sets uprof up for starting
    void initializeUProf()
    {
        initializeUProf(false, false, true);
    }

    bool initializeUProf(bool readPState, bool readCorePower, bool readFrequency)
    {
        
        
        //create log file if enabled
        if (enableLog)
        {
            time(&now);
            stringstream ss;
            string temp;
            
            ss << now;
            ss >> temp;
            temp += ".csv";
            temp = "log" + temp;
            
            
            logName = temp;
            outputLog.open(logName);
        }
        time(&now);
        now -= 3;

        //try to initialize online mode and try to catch exceptions (it doesn't work in reality, IDK why)
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
        hResult = AMDTPwrSetTimerSamplingPeriod(samplingInterval);

        //assert(AMDT_STATUS_OK == hResult);


        //Temperary CounterDesc to get the description of the variables that we will need to test.
        AMDTPwrCounterDesc tempDesc;
        // Grabbing the IDs of important counters
        hResult = AMDTPwrGetCounterId(AMD_PWR_SOCKET_POWER, &socketPowerId);
        assert(AMDT_STATUS_OK == hResult);//you know the drill by now... I am starting to thing this damn API doesn't like running
        //grabbing the description of important counters
        hResult = AMDTPwrGetCounterDesc(socketPowerId, &tempDesc);
        assert(AMDT_STATUS_OK == hResult);
        //storing the name into the proper variable
        socketPowerDesc = tempDesc.m_name;
        //fprintf(stdout, "%s\n", tempDesc.m_name);


        //same for STAPM Limit
        hResult = AMDTPwrGetCounterId(AMD_PWR_SOCKET_STAPM_LIMIT, &stapmLimitId);
        assert(AMDT_STATUS_OK == hResult);
        hResult = AMDTPwrGetCounterDesc(stapmLimitId, &tempDesc);
        assert(AMDT_STATUS_OK == hResult);
        stapmLimitDesc = tempDesc.m_name;
        //fprintf(stdout, "%s\n", tempDesc.m_name);

        //same for Fast Limit
        hResult = AMDTPwrGetCounterId(AMD_PWR_SOCKET_PPT_FAST_LIMIT, &fastLimitId);
        assert(AMDT_STATUS_OK == hResult);
        hResult = AMDTPwrGetCounterDesc(fastLimitId, &tempDesc);
        assert(AMDT_STATUS_OK == hResult);
        fastLimitDesc = tempDesc.m_name;
        
        //same for Slow Limit
        hResult = AMDTPwrGetCounterId(AMD_PWR_SOCKET_PPT_SLOW_LIMIT, &slowLimitId);
        assert(AMDT_STATUS_OK == hResult);
        hResult = AMDTPwrGetCounterDesc(slowLimitId, &tempDesc);
        assert(AMDT_STATUS_OK == hResult);
        slowLimitDesc = tempDesc.m_name;
        


        //get the number of counters
        hResult = AMDTPwrGetSupportedCounters(&nbrCounters, &pCounters);
        //assurt results
        assert(AMDT_STATUS_OK == hResult);

        //setting the pointer of pCurrCounter
        pCurrCounter = pCounters;

        //set the number of samples to 0
        AMDTUInt32 nbrSamples = 0;

        //create output file.
        ofstream counterOutputLog;
        counterOutputLog.open("CounterInfo.txt");


        //enable counters
        for (AMDTUInt32 cnt = 0; cnt < nbrCounters; cnt++, pCurrCounter++)
        {
            //check to see that we do not have a null pointer
            if (nullptr != pCurrCounter)
            {
                //to get the counter Desc
                AMDTPwrCounterDesc counterDesc;

                //getting the description of the current counter ID so that we can disable it based on name
                AMDTPwrGetCounterDesc(pCurrCounter->m_counterID, &counterDesc);
                stringstream ssCntDesc;
                ssCntDesc << pCurrCounter->m_name;
                std::string currName = ssCntDesc.str();

                //temperary bools to improve readability
                bool isPState = currName.find("P-State") != std::string::npos;
                bool isCorePower = currName.find("Core") != std::string::npos && currName.find("Power") != std::string::npos;
                bool isFrequency = currName.find(" Effective Frequency") != std::string::npos;
                bool isCoreFreqency = currName.length() == 32 && currName.find("Thread") != std::string::npos && currName.find(" Effective Frequency") != std::string::npos;

                isCoreFreqency = isCoreFreqency || ((currName.length() == 32 || currName.length() == 32) && currName.find("Thread") != std::string::npos && currName.find("Core Effective Frequency") != std::string::npos);
                if (currName.find("Socket0 STAPM Power") != std::string::npos)
                {
                    //std::cout << pCurrCounter->m_counterID << endl;
                }
                
                std::cout << cnt << "|" << currName << '\n';

                // Check to see if we want to enable the counter or not
                if ((isPState && !readPState) || (isCorePower && !readCorePower) || (isFrequency && !readFrequency))
                {
                    // These are not the counters that you are looking for
                }
                else
                {
                    if (isCoreFreqency)
                    {
                        threadFreqId[threadIndex] = pCurrCounter->m_counterID;
                        threadIndex++;
                    }
                    // Enable all the counter
                    hResult = AMDTPwrEnableCounter(pCurrCounter->m_counterID);
                    // Assert that the counter enabled properly (or that it was already enabled)
                    assert(AMDT_STATUS_OK == hResult || AMDT_ERROR_COUNTER_ALREADY_ENABLED == hResult);

                }
            }
        }
        //system("pause");
        return true;
    }

    // Start uProf (and verify that it did)
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




    AMDTFloat32 getFastLimit()
    {
        return lastFastLimit;
    }

    AMDTFloat32 getSlowLimit()
    {
        return lastSlowLimit;
    }

    AMDTFloat32 getStapmLimit()
    {
        return lastStapmLimit;
    }

    AMDTFloat32 getSocketPower()
    {
        return lastSocketPower;
    }


};

// You're still here? It's over. Go home. Go!
#endif


/*

0|Thread0 Core Effective Frequency
1|Thread0 Histogram-Core Effective Frequency
2|Thread1 Core Effective Frequency
3|Thread1 Histogram-Core Effective Frequency
4|Thread2 Core Effective Frequency
5|Thread2 Histogram-Core Effective Frequency
6|Thread3 Core Effective Frequency
7|Thread3 Histogram-Core Effective Frequency
8|Thread4 Core Effective Frequency
9|Thread4 Histogram-Core Effective Frequency
10|Thread5 Core Effective Frequency
11|Thread5 Histogram-Core Effective Frequency
12|Thread6 Core Effective Frequency
13|Thread6 Histogram-Core Effective Frequency
14|Thread7 Core Effective Frequency
15|Thread7 Histogram-Core Effective Frequency
16|Thread8 Core Effective Frequency
17|Thread8 Histogram-Core Effective Frequency
18|Thread9 Core Effective Frequency
19|Thread9 Histogram-Core Effective Frequency
20|Thread10 Core Effective Frequency
21|Thread10 Histogram-Core Effective Frequency
22|Thread11 Core Effective Frequency
23|Thread11 Histogram-Core Effective Frequency
24|Thread12 Core Effective Frequency
25|Thread12 Histogram-Core Effective Frequency
26|Thread13 Core Effective Frequency
27|Thread13 Histogram-Core Effective Frequency
28|Thread14 Core Effective Frequency
29|Thread14 Histogram-Core Effective Frequency
30|Thread15 Core Effective Frequency
31|Thread15 Histogram-Core Effective Frequency
32|Thread0 P-State
33|Thread1 P-State
34|Thread2 P-State
35|Thread3 P-State
36|Thread4 P-State
37|Thread5 P-State
38|Thread6 P-State
39|Thread7 P-State
40|Thread8 P-State
41|Thread9 P-State
42|Thread10 P-State
43|Thread11 P-State
44|Thread12 P-State
45|Thread13 P-State
46|Thread14 P-State
47|Thread15 P-State
48|Socket0 Package Power
49|Socket0 Cummulative-Package Power
50|Core0 Power
51|Core0 Cummulative-Power
52|Core1 Power
53|Core1 Cummulative-Power
54|Core2 Power
55|Core2 Cummulative-Power
56|Core3 Power
57|Core3 Cummulative-Power
58|Core4 Power
59|Core4 Cummulative-Power
60|Core5 Power
61|Core5 Cummulative-Power
62|Core6 Power
63|Core6 Cummulative-Power
64|Core7 Power
65|Core7 Cummulative-Power
66|Socket0 Temperature


*/