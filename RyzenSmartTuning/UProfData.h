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
    //creation of log
    ofstream outputLog;
    time_t now;

    std::string logName;
    

    bool enableLog;
    bool firstPrint;
    bool didFirstPrint;

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

    //prevents false starts on uProf
    UProfData()
    {
        // Pringles aren't actually potato chips. Kinda like how this is actually not a constuctor, but a strategy for not triggering uProf. Don't trigger the uProf... It has drivers made by AMD after all.
    }

    UProfData(AMDTUInt32 setInterval, bool incomingEnableLog)
    {
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
        return (print(false, true));
    }

    //pass a true to print(bool)
    bool print()
    {
        return print(true, true);
    }
    bool print(bool notUpdate)
    {
        return print(notUpdate, true);
    }

    //bool tells print if it's an update or not
    bool print(bool notUpdate, bool printLog)
    {
        

        //do while loop for the ability to break out if needed
        do
        {
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
                    if (notUpdate && idx == nbrSamples - 1)
                    {
                        system("cls");
                        std::cout << nbrSamples << std::endl;
                    }
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

                            
                            //combining an update function and the print funciton as one
                            if (notUpdate && idx == nbrSamples -1)
                                fprintf(stdout, "%s : %f\n", counterDesc.m_name, pSampleData[idx].m_counterValues->m_data);
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
                
            }
        } while (false);
        return true;

    }


    std::string getPrint(bool notUpdate, bool printLog)
    {
        std::string returnString = "";

        //do while loop for the ability to break out if needed
        do
        {
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


                            //combining an update function and the print funciton as one
                            if (notUpdate && idx == (nbrSamples - 1))
                            {
                                stringstream ssName;
                                stringstream ssData;
                                ssName << counterDesc.m_name;
                                ssData << pSampleData[idx].m_counterValues->m_data;;
                                returnString += ssName.str() + " : " + ssData.str() + '\n';
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

            }
        } while (false);
        return returnString;

    }

    

    // sets uprof up for starting
    void initializeUProf()
    {
        initializeUProf(true, true, true);
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
                std::string currName = counterDesc.m_name;

                //temperary bools to improve readability
                bool isPState = currName.find("P-State") != std::string::npos;
                bool isCorePower = currName.find("Core") != std::string::npos && currName.find("Power") != std::string::npos;
                bool isFrequency = currName.find(" Effective Frequency") != std::string::npos;

                // Check to see if we want to enable the counter or not
                if ((isPState && !readPState) || (isCorePower && !readCorePower) || (isFrequency && !readFrequency))
                {
                    // These are not the counters that you are looking for
                }
                else
                {
                    // Enable all the counter
                    hResult = AMDTPwrEnableCounter(pCurrCounter->m_counterID);
                    // Assert that the counter enabled properly (or that it was already enabled)
                    assert(AMDT_STATUS_OK == hResult || AMDT_ERROR_COUNTER_ALREADY_ENABLED == hResult);
                }
            }
        }
        
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

    
};

// You're still here? It's over. Go home. Go!
#endif
