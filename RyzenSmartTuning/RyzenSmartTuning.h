#ifndef _RYZENSMARTTUNING_H_
#define _RYZENSMARTTUNING_H_


#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>
#include <thread>


#include <AMDTPowerProfileApi.h>

#include "ryzenadj.h"

#include "UProfData.h"



struct RyzenSmartTuning
{
	//uProf data
	AMDTUInt32 samplingInterval = 100;
	UProfData uProf;

	//for holding target values
	uint32_t targetStapmLimit;
	uint32_t targetFastLimit;
	uint32_t targetSlowLimit;
	//enable the detection of changes
	bool enableChangeDetection;
	
	//arrrays to hold required info
	/*inline std::string description[22];{ "STAPM Limit (mW)", "Fast Limit (Mw)", "Slow Limit (Mw)",  "Slow Time (S)",
	"STAPM Time (S)", "Tctl Temperature (Degrees C)", "VRM Current Limit (mA)", "VRM SoC Current Limit (mA)",
		"VRM Maximum Current Limit (mA)", "VRM SoC Maximum Current Limit (mA)", "PSI0 Current Limit (mA)", "PSI0 SoC Current Limit(mA)",
		"Maximum SoC Clock Frequency(MHz)", "Minimum SoC Clock Frequency(MHz)", "Maximum Transmission(CPU - GPU) Frequency(MHz)", "Minimum Transmission(CPU - GPU) Frequency(MHz)",
		"Maximum Video Core Next(VCE - Video Coding Engine) (Value)", "Minimum Video Core Next(VCE - Video Coding Engine) (Value)", "Maximum Data Launch Clock(Value)", "Minimum Data Launch Clock(Value)",
		"Maximum GFX Clock(Value)", "Minimum GFX Clock(Value)"}*/
	uint32_t settings[23];
	bool enableSettings[23];

	ryzen_access ryzenAccess;
	int ryzenAdjSleep = 0;

	time_t now;
	time_t lastApplication;
	time_t timeSinceLastApplication;
	int secondsSinceLastApplication;
	int cooldownTime = 3;

	//enable the application of settings
	bool enableStapmLimit;
	bool enableFastLimit;
	bool enableSlowLimit;
	bool enableSlowTime;
	bool enableStapmTime;
	bool enableTctlTemp;
	bool enableVrmCurrent;
	bool enableVrmSocCurrent;
	bool enableVrmMaxCurrent;
	bool enableVrmSocMaxCurrent;
	bool enablePsi0Current;
	bool enablePsi0SocCurrent;
	bool enableMaxGfxClkFreq;
	bool enableMinGfxClkFreq;
	bool enableMaxSocClkFreq;
	bool enableMinSocClkFreq;
	bool enableMaxFclkFreq;
	bool enableMinFclkFreq;
	bool enableMaxVcn;
	bool enableMinVcn;
	bool enableMaxLclk;
	bool enableMinLclk;

	bool enableLog;

	
	//22 Limits
	//for holding general values
	uint32_t stapmLimit;
	uint32_t fastLimit;
	uint32_t slowLimit;
	uint32_t slowTime;
	uint32_t stapmTime;

	uint32_t tctlTemp;
	uint32_t vrmCurrent;
	uint32_t vrmSocCurrent;
	uint32_t vrmMaxCurrent;
	uint32_t vrmSocMaxCurrent;

	uint32_t psi0Current;
	uint32_t psi0SocCurrent;
	uint32_t maxGfxClkFreq;
	uint32_t minGfxClkFreq;

	uint32_t maxSocClkFreq;
	uint32_t minSocClkFreq;
	uint32_t maxFclkFreq;
	uint32_t minFclkFreq;

	uint32_t maxVcn;
	uint32_t minVcn;
	uint32_t maxLclk;
	uint32_t minLclk;


	uint32_t* ryzenAccessSettings[22] = { &stapmLimit, &fastLimit, &slowLimit, &slowTime, &stapmTime, &tctlTemp, &vrmCurrent, &vrmSocCurrent, &vrmMaxCurrent, &vrmSocMaxCurrent, &psi0Current, &psi0SocCurrent, &maxGfxClkFreq, &minGfxClkFreq, &maxSocClkFreq, &minSocClkFreq, &maxFclkFreq, &minFclkFreq, &maxVcn, &minVcn, &maxLclk, &minLclk};

	RyzenSmartTuning()
	{
		//so we don't get a false start on uProf and RyzenAdj this is blank. uProf is angrier than an Intel shill when you bring up AMD. So please, try not to make it mad.
	}
	RyzenSmartTuning(bool enableChangeDetection, AMDTUInt32 customSamplingInterval, bool startUProfNow)
	{
		// Setting all settings to 0 if none are provided.
		uint32_t tempSettings[23];
		for (int i = 0; i < 22; i++)
		{
			tempSettings[i] = 0;
		}
		RyzenSmartTuning(enableChangeDetection, customSamplingInterval, startUProfNow, tempSettings, true);
	}

	RyzenSmartTuning(bool enableChangeDetection, AMDTUInt32 customSamplingInterval, bool startUProfNow, uint32_t incomingSettings[22], bool userEnableLog)
	{
		//array of pointers for being able to manage all of the settings as an array


		//setting the time of last applicaion to now.
		time(&now);
		lastApplication = now;
		
		system("cls");
		//storing the incomingSettings into the local settings
		for (int i = 0; i < 22; i++)
		{
			settings[i] = incomingSettings[i];
			*ryzenAccessSettings[i] = incomingSettings[i];
			std::cout << "loaded: " << *ryzenAccessSettings[i] << '\n';
		}
		//system("pause");
		// I mean It is efficent in terms of getting the array into the individual variables.
		
		enableLog = userEnableLog;

		samplingInterval = customSamplingInterval;

		uProf = UProfData(customSamplingInterval, enableLog);

		uProf.initializeUProf(false,true,true);
		
		if (startUProfNow)
		{
			startUProf();
		}

		//getting a valid update
		Sleep(samplingInterval);
		uProf.update();
		
		//setting the enablers based on if I was moddified
		enableStapmLimit = stapmLimit != 0;
		enableFastLimit = fastLimit != 0;
		enableSlowLimit = slowLimit != 0;
		enableSlowTime = slowTime != 0;
		enableStapmTime = stapmTime != 0;
		enableTctlTemp = tctlTemp != 0;
		enableVrmCurrent = vrmCurrent != 0;
		enableVrmSocCurrent = vrmSocCurrent != 0;
		enableVrmMaxCurrent = vrmMaxCurrent != 0;
		enableVrmSocMaxCurrent = vrmSocMaxCurrent != 0;
		enablePsi0Current = psi0Current != 0;
		enablePsi0SocCurrent = psi0SocCurrent != 0;
		enableMaxGfxClkFreq = maxGfxClkFreq != 0;
		enableMinGfxClkFreq = minGfxClkFreq != 0;
		enableMaxSocClkFreq = maxSocClkFreq != 0;
		enableMinSocClkFreq = minSocClkFreq != 0;
		enableMaxFclkFreq = maxFclkFreq != 0;
		enableMinFclkFreq = minFclkFreq != 0;
		enableMaxVcn = maxVcn != 0;
		enableMinVcn = minVcn != 0;
		enableMaxLclk = maxLclk != 0;
		enableMinLclk = minLclk != 0;
	}

	// passes off initalizing ryzenAdj
	bool initRyzenAdj()
	{
		//cleanup_ryzenadj(ryzenAccess);
		ryzenAccess = init_ryzenadj();
		return true;
	}

	//
	bool changeSettings()
	{

	}

	//really usefull stuff here
	void print()
	{
		uProf.print();
	}

	// detects if settings have been changed. Must have a successfull print()/update() before it can be used
	bool settingsChanged()
	{
		if (round(uProf.lastFastLimit) * 1000 != fastLimit)
		{
			return true;
		}
		else if (round(uProf.lastSlowLimit) * 1000 != slowLimit)
		{
			return true;
		}
		else if (round(uProf.lastStapmLimit) * 1000 != stapmLimit)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool applySettings()
	{
		//calculates the time since last application
		time(&now);
		timeSinceLastApplication = difftime(now, lastApplication);
		secondsSinceLastApplication = int(timeSinceLastApplication);
		
		if (secondsSinceLastApplication >= cooldownTime)
		{
			// If the setting has been enabled, it applies the setting.
			if (enableStapmLimit)
			{
				applySettingResults(set_stapm_limit(ryzenAccess, stapmLimit), "STAPM Limit");
				//Sleep(ryzenAdjSleep);
			}
			if (enableFastLimit)
			{
				applySettingResults(set_fast_limit(ryzenAccess, fastLimit), "Fast Limit");
				//Sleep(ryzenAdjSleep);
			}
			if (enableSlowLimit)
			{
				applySettingResults(set_slow_limit(ryzenAccess, slowLimit), "Slow Limit");
				//Sleep(ryzenAdjSleep);
			}
			if (enableSlowTime)
			{
				applySettingResults(set_slow_time(ryzenAccess, slowTime), "Slow Time");
				//Sleep(ryzenAdjSleep);
			}
			if (enableStapmTime)
			{
				applySettingResults(set_stapm_time(ryzenAccess, stapmTime), "STAPM Time");
				//Sleep(ryzenAdjSleep);
			}
			if (enableTctlTemp)
			{
				applySettingResults(set_tctl_temp(ryzenAccess, tctlTemp), "TCTL Temperature");
				//Sleep(ryzenAdjSleep);
			}
			if (enableVrmCurrent)
			{
				applySettingResults(set_vrm_current(ryzenAccess, vrmCurrent), "VRM Current");
				//Sleep(ryzenAdjSleep);
			}
			if (enableVrmSocCurrent)
			{
				applySettingResults(set_vrmsoc_current(ryzenAccess, vrmSocCurrent), "VRM SOC Current");
				//Sleep(ryzenAdjSleep);
			}
			if (enableVrmMaxCurrent)
			{
				applySettingResults(set_vrmmax_current(ryzenAccess, vrmMaxCurrent), "VRM Max Current");
				//Sleep(ryzenAdjSleep);
			}
			if (enableVrmSocMaxCurrent)
			{
				applySettingResults(set_vrmsocmax_current(ryzenAccess, vrmSocMaxCurrent), "VRM SOC Max Current");
				//Sleep(ryzenAdjSleep);
			}
			if (enablePsi0Current)
			{
				applySettingResults(set_psi0_current(ryzenAccess, psi0Current), "PSI0 Current");
				//Sleep(ryzenAdjSleep);
			}
			if (enablePsi0SocCurrent)
			{
				applySettingResults(set_psi0soc_current(ryzenAccess, psi0SocCurrent), "PSIO SOC Current");
				//Sleep(ryzenAdjSleep);
			}
			if (enableMaxGfxClkFreq)
			{
				applySettingResults(set_max_gfxclk_freq(ryzenAccess, maxGfxClkFreq), "Max GFX Frequency");
				//Sleep(ryzenAdjSleep);
			}
			if (enableMinGfxClkFreq)
			{
				applySettingResults(set_min_gfxclk_freq(ryzenAccess, minGfxClkFreq), "Min GFX Frequency");
				//Sleep(ryzenAdjSleep);
			}
			if (enableMaxSocClkFreq)
			{
				applySettingResults(set_max_socclk_freq(ryzenAccess, maxSocClkFreq), "Max SOC Frequency");
				//Sleep(ryzenAdjSleep);
			}
			if (enableMinSocClkFreq)
			{
				applySettingResults(set_min_socclk_freq(ryzenAccess, minSocClkFreq), "Min SOC Frequency");
				//Sleep(ryzenAdjSleep);
			}
			if (enableMaxFclkFreq)
			{
				applySettingResults(set_max_fclk_freq(ryzenAccess, maxFclkFreq), "Max FClk Frequenxy");
				//Sleep(ryzenAdjSleep);
			}
			if (enableMinFclkFreq)
			{
				applySettingResults(set_min_fclk_freq(ryzenAccess, minFclkFreq), "Min FClk Frequency");
				//Sleep(ryzenAdjSleep);
			}
			if (enableMaxVcn)
			{
				applySettingResults(set_max_vcn(ryzenAccess, maxVcn), "Max VCN Frequency");
				//Sleep(ryzenAdjSleep);
			}
			if (enableMinVcn)
			{
				applySettingResults(set_min_vcn(ryzenAccess, minVcn), "Min VCN Frequency");
				//Sleep(ryzenAdjSleep);
			}
			if (enableMaxLclk)
			{
				applySettingResults(!set_max_lclk(ryzenAccess, maxLclk), "Max LClk Frequency");
				//Sleep(ryzenAdjSleep);
			}
			if (enableMinLclk)
			{
				applySettingResults(set_min_lclk(ryzenAccess, minLclk), "Min LClk Frequency");
				//Sleep(ryzenAdjSleep);
			}
			std::cout << "Applied Settings!\n";
			//Sleep(1000);

			//resetting the time of lastApplication to now
			time(&now);
			lastApplication = now;
		}
		else
		{
			fprintf(stdout, "%i seconds untill next aplication can happen.\n", (cooldownTime-secondsSinceLastApplication));
		}
		return true;
	}

	bool applySettingResults(int i, std::string s)
	{
		if (!i)
		{
			std::cout << "Successfully applied " <<  s << std::endl;
			return true;
		}
		else
		{
			std::cout << "Was not able to apply " << s << std::endl;
			return false;
		}
	}

	//function to pass off starts to uProf
	bool startUProf()
	{
		return uProf.startProf();
	}

	//method to pass off stops to uProf
	bool stopUProf()
	{
		return uProf.stopProf();
	}


	//starts Ryzen Smart Tuning. WIP sitll
	void startRST()
	{
		Sleep(100);
		do 
		{
			std::string temp = uProf.getPrint(true, true);
			system("cls");
			std::cout << temp;
			if(settingsChanged())
			{
				applySettings();
			}
		} while (true);
	}
};

#endif
