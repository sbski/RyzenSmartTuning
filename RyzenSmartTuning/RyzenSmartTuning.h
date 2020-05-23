#ifndef _RYZENSMARTTUNING_H_
#define _RYZENSMARTTUNING_H_


#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>
#include <thread>
#include <queue> 
using namespace std;


#include <AMDTPowerProfileApi.h>

#include "ryzenadj.h"
#include "nb_smu_ops.h"

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
		"Maximum SoC Clock Frequefncy(MHz)", "Minimum SoC Clock Frequency(MHz)", "Maximum Transmission(CPU - GPU) Frequency(MHz)", "Minimum Transmission(CPU - GPU) Frequency(MHz)",
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
	int cooldownTime = 5;

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
	bool enableMaxSocClkFreq;
	bool enableMinSocClkFreq;
	bool enableMaxFclkFreq;
	bool enableMinFclkFreq;
	bool enableMaxVcn;
	bool enableMinVcn;
	bool enableMaxLclk;
	bool enableMinLclk;
	bool enableMaxGfxClkFreq;
	bool enableMinGfxClkFreq;

	bool enableLog;

	
	//22 Limits
	//for holding general values
	uint32_t stapmLimitIdx = 0;
	uint32_t fastLimitIdx = 1;
	uint32_t slowLimitIdx = 2;
	uint32_t slowTimeIdx = 3;
	uint32_t stapmTimeIdx = 4;

	uint32_t tctlTempIdx = 5;
	uint32_t vrmCurrentIdx = 6;
	uint32_t vrmSocCurrentIdx = 7;
	uint32_t vrmMaxCurrentIdx = 8;
	uint32_t vrmSocMaxCurrentIdx = 9;

	uint32_t psi0CurrentIdx = 10;
	uint32_t psi0SocCurrentIdx = 11;
	uint32_t maxSocClkFreqIdx = 12;
	uint32_t minSocClkFreqIdx = 13;
	
	uint32_t maxFclkFreqIdx = 14;
	uint32_t minFclkFreqIdx = 15;
	uint32_t maxVcnIdx = 16;
	uint32_t minVcnIdx = 17;

	uint32_t maxLclkIdx = 18;
	uint32_t minLclkIdx = 19;
	uint32_t maxGfxClkFreqIdx = 20;
	uint32_t minGfxClkFreqIdx = 21;

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
	uint32_t maxSocClkFreq;
	uint32_t minSocClkFreq;
	
	uint32_t maxFclkFreq;
	uint32_t minFclkFreq;
	uint32_t maxVcn;
	uint32_t minVcn;

	uint32_t maxLclk;
	uint32_t minLclk;
	uint32_t maxGfxClkFreq;
	uint32_t minGfxClkFreq;

	uint32_t* ryzenAccessSettings[22] = { &stapmLimit, &fastLimit, &slowLimit, &slowTime, &stapmTime, &tctlTemp, &vrmCurrent, &vrmSocCurrent, &vrmMaxCurrent, &vrmSocMaxCurrent, &psi0Current, &psi0SocCurrent, &maxSocClkFreq, &minSocClkFreq, &maxFclkFreq, &minFclkFreq, &maxVcn, &minVcn, &maxLclk, &minLclk, &maxGfxClkFreq, &minGfxClkFreq };
	/*"0. STAPM Limit (W)", "1. Fast Limit (W)", "2. Slow Limit (W)", "3. Slow Time (Package Power Tracking (PPT) - Slow period) (S)",
		"4. STAPM Time (CPU Boost Period) (S)", "5. Tctl Temperature (Degrees C)", "6. VRM Current Limit (A)", "7. VRM SoC Current Limit (mA)",
		"8. VRM Maximum Current Limit (A)", "9. VRM SoC Maximum Current Limit (A)", "10. PSI0 Current Limit (mA)", "11. PSI0 SoC Current Limit(mA)",
		"12. Maximum SoC Clock Frequency(MHz)", "13. Minimum SoC Clock Frequency(MHz)", "14. Maximum Transmission(Infinity Fabric) Frequency(MHz)", "15. Minimum Transmission(Infinity Fabric) Frequency(MHz)",
		"16. Maximum Video Core Next(VCE - Video Coding Engine) (Value)", "17. Minimum Video Core Next(VCE - Video Coding Engine) (Value)", "18. Maximum Data Launch Clock(Value)", "19. Minimum Data Launch Clock(Value)",
		"20. Maximum GFX Clock(Value)", "21. Minimum GFX Clock(Value)"*/
	
	AMDTFloat32 minThreadFreq;
	AMDTFloat32 avgThreadFreq;
	AMDTFloat32 maxThreadFreq;

	uint32_t targetPower;
	uint32_t powerLimitForTarget;
	uint32_t currentTargetPower;
	uint32_t startingPower;
	float powerToTempRatio = 1.25;
	float targetPowerTime;
	float powerChangeAmount;
	float changePerUpdate;
	float startingPowerRatio;
	float targetRatio;
	bool newTarget;
	

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
		float samplesPerSecond = 1000.0 / float(samplingInterval);
		//values for starting smart tuning
		powerChangeAmount = 1/samplesPerSecond;
		powerChangeAmount = 0.25;
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

		uProf.initializeUProf(false, false,true);
		
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
		enableMaxVcn = maxVcn != 0;
		enableMinVcn = minVcn != 0;
		enableMaxLclk = maxLclk != 0;
		enableMinLclk = minLclk != 0;
		enableMaxFclkFreq = maxFclkFreq != 0;
		enableMinFclkFreq = minFclkFreq != 0;
	}

	// passes off initalizing ryzenAdj
	bool initRyzenAdj()
	{
		//cleanup_ryzenadj(ryzenAccess);
		ryzenAccess = init_ryzenadj();
		return true;
	}

	//
	bool changeSettings(uint32_t incomingSettings[22])
	{
		for (int i = 0; i < 22; i++)
		{
			settings[i] = incomingSettings[i];
			*ryzenAccessSettings[i] = incomingSettings[i];
			//std::cout << "loaded: " << *ryzenAccessSettings[i] << '\n';
		}
	}

	//really usefull stuff here
	void print()
	{
		fprintf(stdout, "%s", uProf.getPrint(true,0));
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

	bool getUprofSettings()
	{
		fastLimit == round(uProf.lastFastLimit) * 1000;
		slowLimit == round(uProf.lastSlowLimit) * 1000;
		stapmLimit == round(uProf.lastSlowLimit) * 1000;
		return true;
	}

	bool applySettings()
	{
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

		//calculates the time since last application
		time(&now);
		timeSinceLastApplication = difftime(now, lastApplication);
		secondsSinceLastApplication = int(timeSinceLastApplication);
		
		if (cooldownTime == 0 || secondsSinceLastApplication >= cooldownTime)
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
			//std::cout << "Applied Settings!\n";
			//Sleep(1000);

			//resetting the time of lastApplication to now
			time(&now);
			lastApplication = now;
		}
		else
		{
			//fprintf(stdout, "%i seconds untill next aplication can happen.\n", (cooldownTime-secondsSinceLastApplication));
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
	void startStapmMulti(uint32_t boostPeriod, uint32_t startingBoostPeriod, int numberOfPasses)
	{
		Sleep(100);
		applySettings();
		Sleep(100);
		std::string temp = uProf.getPrint(true, 0);
		system("cls");
		std::cout << "Number of samples collected: " << uProf.numberOfSamples << "\n--------------------\n";
		std::cout << uProf.lastStapmPower << " | " << uProf.lastSocTemp << " | " << uProf.lastSocketTemp << '\n';
		std::cout << temp;
		while (true)
		{
			applySettings();
			set_stapm_time(ryzenAccess, startingBoostPeriod);
			Sleep(50);
			int i = 0;
			for (int i = 0; i < numberOfPasses; i++)
			{
				beatDownStapm(stapmLimit - 1100);
				temp = uProf.getPrint(true, 1);
				system("cls");
				std::cout << uint32_t(round(uProf.lastStapmPower * 1000.0)) << "\nWaiting getting settigns | " << uProf.lastStapmPower << "\nNumber of samples collected: " << uProf.numberOfSamples << "\n--------------------\n";
				std::cout << uProf.lastSocketPower << " | " << uProf.lastStapmPower << " | " << uProf.lastSocTemp << " | " << uProf.lastSocketTemp << '\n';
				std::cout << temp;
				Sleep(samplingInterval);
				int j = 0;
				while (uProf.lastSocketPower > float(slowLimit - 2000)/1000.0 || j < 4)
				{
					j++;
					temp = uProf.getPrint(true, 1);
					system("cls");
					std::cout << "Waiting On STAPM\nNumber of samples collected: " << uProf.numberOfSamples << "\n--------------------\n";
					std::cout << uProf.lastSocketPower << " | " << uProf.lastStapmPower << " | " << uProf.lastSocTemp << " | " << uProf.lastSocketTemp << '\n';
					std::cout << temp;
					Sleep(samplingInterval);
					if (settingsChanged())
					{
						applySettings();
					}
				}
				uProf.outputLog << '\n';
				uProf.outputLog.flush();
				while (uint32_t(uProf.lastSocketPower > float(stapmLimit + 1000) / 1000.0))//round(uProf.lastStapmPower * 1000.0)) < stapmLimit - 250)
				{
					temp = uProf.getPrint(true, startingBoostPeriod);
					system("cls");
					std::cout << uint32_t(round(uProf.lastStapmPower * 1000.0)) << "\nPass " << (i) + 1 << "/" << numberOfPasses << " for " << startingBoostPeriod << "\n";
					std::cout << "Running test\nNumber of samples collected: " << uProf.numberOfSamples << "\n--------------------\n";
					std::cout << uProf.lastStapmPower << " | " << uProf.lastSocTemp << " | " << uProf.lastSocketTemp << '\n';
					std::cout << temp;
					Sleep(samplingInterval);
				}
				uProf.outputLog << '\n';
				uProf.outputLog.flush();
			}
			startingBoostPeriod += boostPeriod;
		}
		
		applySettings();
		Sleep(100);
		system("cls");
		do
		{
			std::string temp = uProf.getPrint(true, 0);


			if (settingsChanged())
			{
				applySettings();
			}
			Sleep(samplingInterval);

		} while (true);
	}


	//starts Ryzen Smart Tuning. WIP sitll
	void startRST()
	{
		beatDownStapm();
		applySettings();
		Sleep(100);
		system("cls");
		do 
		{
			std::string temp = uProf.getPrint(true, 0);
			system("cls");
			std::cout <<"Number of samples collected: " << uProf.numberOfSamples << "\n--------------------\n";
			std::cout << uProf.lastStapmPower << " | " << uProf.lastSocTemp << " | " << uProf.lastSocketTemp << '\n';
			std::cout << temp;
			
			if (settingsChanged())
			{
				applySettings();
			}
			Sleep(samplingInterval);
			
		} while (true);
	}
	void startTestRST()
	{
		uint32_t startingPower = 25000;
		beatDownStapm();
		applySettings();
		Sleep(100);
		system("cls");

		stapmLimit = startingPower;
		fastLimit = stapmLimit;
		slowLimit = stapmLimit;
		do
		{
			stapmLimit = startingPower;
			fastLimit = stapmLimit;
			slowLimit = stapmLimit;
			//vrmMaxCurrent = stapmLimit + 15000;
			//vrmCurrent = vrmMaxCurrent;
			//Sleep(1000);



			set_stapm_limit(ryzenAccess, stapmLimit);
			set_fast_limit(ryzenAccess, fastLimit);
			set_slow_limit(ryzenAccess, slowLimit);
			std::string temp = uProf.getPrint(true, stapmLimit);
			system("cls");
			std::cout << "Number of samples collected: " << uProf.numberOfSamples << "\n--------------------\n";
			std::cout << "Current Power Limit: " << startingPower << '\n';
			std::cout << uProf.socketTempDesc << ": " <<  uProf.lastSocketTemp << "\n";
			std::cout << uProf.packagePowerDesc << ": " << uProf.lastPackagePower << "\n";

			for (int i = 0; i < uProf.threadCount; i++)
			{
				std::cout << "Thread" << i << " Core Effective Frequency: " << uProf.lastThreadFreq[i] << '\n';
			}
			
			Sleep(samplingInterval);
			startingPower += 10;

		} while (uProf.lastSocketTemp < 80.0);
		stapmLimit = 30000;
		fastLimit = stapmLimit;
		slowLimit = stapmLimit;

		set_stapm_limit(ryzenAccess, stapmLimit);
		set_fast_limit(ryzenAccess, fastLimit);
		set_slow_limit(ryzenAccess, slowLimit);
		set_vrmmax_current(ryzenAccess, vrmMaxCurrent);
		set_vrm_current(ryzenAccess, vrmCurrent);
	}

	void startRSTBenchMarkMode()
	{
		applySettings();
		Sleep(100);
		system("cls");
		do
		{
			std::string temp = uProf.getPrint(true, 0);
			
			if (settingsChanged())
			{
				applySettings();
				system("cls");
			}
			Sleep(samplingInterval);

		} while (true);
	}

	AMDTFloat32 findSafePower(AMDTFloat32 startingTemperature, AMDTFloat32 targetTemperature, uint32_t startingPower, uint32_t numberOfTests)
	{
		uint32_t maxPower = startingPower;
		AMDTFloat32 minThreadFreq;
		AMDTFloat32 avgThreadFreq;
		AMDTFloat32 maxThreadFreq;


		applySettings();
		Sleep(100);
		cooldownTime = 0;
		for (int i = 0; i < numberOfTests; i++)
		{
			system("smu-tool --message=0x27");
			Sleep(50);
			stapmLimit = startingPower;
			fastLimit = stapmLimit;
			slowLimit = stapmLimit;
			vrmMaxCurrent = (stapmLimit / 10 * 8) + 15000;
			vrmCurrent = vrmMaxCurrent;
			std::cout << stapmLimit << '\n' << vrmMaxCurrent << '\n';
			Sleep(1000);



			set_stapm_limit(ryzenAccess, stapmLimit);
			set_fast_limit(ryzenAccess, fastLimit);
			set_slow_limit(ryzenAccess, slowLimit);
			set_vrmmax_current(ryzenAccess, vrmMaxCurrent);
			set_vrm_current(ryzenAccess, vrmCurrent);
			


			
			for (int j = 0; j < 10; j++)
			{
				std::string temp = uProf.getPrint(true, 0);
				system("cls");
				std::cout << "In Cooldown\n" << uProf.numberOfSamples << '\n';
				std::cout << uProf.lastStapmPower << " | " << uProf.lastSocTemp << " | " << uProf.lastSocketTemp << '\n';
				std::cout << temp;
				Sleep(samplingInterval);
			}

			applySettings();

			while (uProf.lastSocTemp > startingTemperature && uProf.lastSocketTemp > startingTemperature)
			{
				std::string temp = uProf.getPrint(true, 0);
				system("cls");
				fprintf(stdout, "Waiting on temperature to drop| SOC:%f | Socket:%f\n", uProf.lastSocTemp, uProf.lastSocketTemp);
				std::cout << temp;
				Sleep(samplingInterval);
			}
			system("cls");
			while (uProf.lastSocTemp < targetTemperature && uProf.lastSocketTemp < targetTemperature)
			{
				Sleep(samplingInterval);
				std::string temp = uProf.getPrint(true, vrmMaxCurrent);
				system("cls");
				std::cout << "Pass: " << i+1 << " of " << numberOfTests << '\n';
				std::cout << uProf.numberOfSamples << '\n';
				std::cout << "Current limit: " << vrmMaxCurrent << '\n';
				//std::cout << uProf.lastStapmPower << " | " << uProf.lastSocTemp << " | " << uProf.lastSocketTemp << '\n';
				std::cout << temp;
				minThreadFreq = uProf.lastThreadFreq[0];
				avgThreadFreq = 0;
				maxThreadFreq = uProf.lastThreadFreq[0];

				
				for (int i = 0; i < uProf.threadCount; i++)
				{
					if (uProf.lastThreadFreq[i] > maxThreadFreq)
					{
						maxThreadFreq = uProf.lastThreadFreq[i];
					}
					else if (uProf.lastThreadFreq[i] < minThreadFreq)
					{
						minThreadFreq = uProf.lastThreadFreq[i];
					}
					avgThreadFreq += uProf.lastThreadFreq[i] / float(uProf.threadCount);
				}
				
				

				std::cout << uProf.lastSocketPower << " | " <<  uProf.lastFastLimit << '\n';
					
				
				stapmLimit += samplingInterval / 25;
				fastLimit = stapmLimit;
				slowLimit = stapmLimit;
				

				
				
				//setting the max VRM Current
				vrmMaxCurrent = stapmLimit + 15000;
				vrmCurrent = vrmMaxCurrent;

				if (maxPower > stapmLimit)
				{
					maxPower = stapmLimit;
				}

				set_stapm_limit(ryzenAccess, stapmLimit);
				set_fast_limit(ryzenAccess, fastLimit);
				set_slow_limit(ryzenAccess, slowLimit);
				set_vrmmax_current(ryzenAccess, vrmMaxCurrent);
				set_vrm_current(ryzenAccess, vrmCurrent);

				

			}
			//system("pause");
		}
		return maxPower;
	}

	int keepClocks()
	{
		int settingsChanged = 0;
		



		return settingsChanged;
	}

	void startSmartTune(AMDTFloat32 targetTemperature, uint32_t maxPower, uint32_t stockPower)
	{
		
		AMDTFloat32 delta;
		AMDTFloat32 deltaSum;
		AMDTFloat32 powerDelta;
		AMDTFloat32 lastHighDelta;
		AMDTFloat32 lastLowDelta;

		AMDTFloat32 deltaMultiplier = 250.0;

		uint32_t intDelta = 0;

		bool wasLow = true;
		bool reachedTarget = false;
		bool justReachedTarget = true;
		bool justLeftTarget = true;

		

		applySettings();
		Sleep(100);
		cooldownTime = 0;

		stapmLimit = stockPower;
		fastLimit = stapmLimit+5000;
		slowLimit = stapmLimit+1000;
		powerLimitForTarget = maxPower;
		
		applyPowerLimits();
		updateCurrentLimits();
		applyCurrentLimits();

		lastHighDelta = 0;
		lastLowDelta = 0;

		while (true)
		{
			std::string temp = uProf.getPrint(true, vrmMaxCurrent);
			system("cls");
			std::cout << uProf.numberOfSamples << '\n';
			std::cout << "Current limit: " << vrmMaxCurrent << '\n';
			//std::cout << uProf.lastStapmPower << " | " << uProf.lastSocTemp << " | " << uProf.lastSocketTemp << '\n';
			std::cout << temp;
			/*
			if (settingsChanged())
			{
				applySettings();
			}
			*/
			delta = targetTemperature - uProf.lastSocketTemp;
			bool isUnderLoad = ((slowLimit - 500 <= uProf.lastPackagePower * 1000) && (slowLimit + 500 >= uProf.lastPackagePower * 1000));
			bool isHotEnough = uProf.lastSocketTemp > 50;
			
			isUnderLoad = true;
			if (isUnderLoad)
			{
				deltaSum = 0;
				//delta is posive if its hot, negitive if it's not
				if (delta > 5)
				{
					std::cout << "Delta is more than 5\n";
					targetPower = maxPower;
					newTarget = true;
				}
				else if (delta < -5)
				{
					std::cout << "Delta is less than -5\n";
					targetPower = stockPower;
					newTarget = true;
				}
				else if (delta > 0)
				{
					std::cout << "Positive Delta\n";
					lastLowDelta = 0;
					if (lastHighDelta < delta)
					{
						int i = 0;
						for (float f = lastHighDelta; f < delta; f += 0.125)
						{
							i = round(f / 0.125);
							cout << i << " | " << pow(2, i) <<' \n';
							deltaSum += pow(2,i);
							i++;
						}
						lastHighDelta = delta;
						if (lastHighDelta < -10)
						{
							slowLimit = stockPower;
						}
						newTarget = true;
					}
					else
					{
						deltaSum = 0;
						newTarget = false;
					}
				}
				else if (delta < 0)
				{
					std::cout << "Negitive Delta\n";
					lastHighDelta = 0;
					if (lastLowDelta > delta)
					{
						int i = 0;
						for (float f = delta; f < lastLowDelta; f += 0.125)
						{
							cout << i << " | " << pow(2, i) << ' \n';
							i = round(f / 0.125);
							deltaSum -= pow(2, i);
						}
						lastLowDelta = delta;
						if (lastLowDelta > 10.0)
						{
							slowLimit = targetPower;
						}
						newTarget = true;
					}
					else
					{
						deltaSum = 0;
						newTarget = false;
					}
				}
				else
				{
					std::cout << "No Significant Delta\n";
					deltaSum = 0;
				}
				std::cout << "Last High Delta: " << lastHighDelta << '\n';
				std::cout << "Last Low Delta:  " << lastLowDelta << '\n';
				std::cout << "Delta:           " << delta << '\n';
				std::cout << "DeltaSum:        " << deltaSum << '\n';




				if (deltaSum != 0)
				{
					targetPower = slowLimit + round(deltaSum * deltaMultiplier);
				}
				if (targetPower > maxPower)
				{
					targetPower = maxPower;
				}
				if (targetPower < stockPower)
				{
					targetPower = stockPower;
				}

				std::cout << "Target Power: " << targetPower << '\n';
				updateRstSmooth(delta, maxPower, stockPower);
			}
			//sleep at the end of all of the output
			Sleep(samplingInterval);
		}
		//system("pause");
	}

	void updateRstSmooth(float delta, uint32_t maxPower, uint32_t stockPower)
	{
		beatDownStapm();
		std::cout << "Update!\n";
		std::cout << "Current Target: " << currentTargetPower << "\n";

		if (newTarget)
		{
			newTarget = false;
			targetPowerTime = 0;
			targetRatio = 0;
			startingPowerRatio = 1;
			currentTargetPower = targetPower;
			powerLimitForTarget = slowLimit;

			std::cout << "New target\n";
		}
		else
		{
			std::cout << "No new target\n";
		}
		if (targetRatio < 1)
		{
			targetRatio += powerChangeAmount;
			startingPowerRatio = 1.0 - targetRatio;
			slowLimit = powerLimitForTarget * (startingPowerRatio)+currentTargetPower * (targetRatio);

			fastLimit = slowLimit + 5000;
			updateCurrentLimits();
			applyPowerLimits();
			applyCurrentLimits();
		}
		else
		{
			slowLimit += round(delta * abs(delta) * samplingInterval);
			if (slowLimit > maxPower)
			{
				slowLimit = maxPower;
			}
			else if (slowLimit < stockPower)
			{
				slowLimit = stockPower;
			}
			updateCurrentLimits();
			applyPowerLimits();
			applyCurrentLimits();
		}
		std::cout << "Target Ratio: " << targetRatio << "\nStapm Ratio: " << startingPowerRatio << "\nSTAPM Limit: " << stapmLimit;
	}

	void updateCurrentLimits()
	{
		vrmMaxCurrent = slowLimit + 20000;
		fastLimit = slowLimit + 5000;
		vrmCurrent = vrmMaxCurrent;
	}

	void applyPowerLimits()
	{
		set_stapm_limit(ryzenAccess, stapmLimit);
		set_slow_limit(ryzenAccess, slowLimit);
		set_fast_limit(ryzenAccess, fastLimit);
	}

	void applyCurrentLimits()
	{
		set_vrmmax_current(ryzenAccess, vrmMaxCurrent);
		set_vrm_current(ryzenAccess, vrmCurrent);
	}

	bool beatDownStapm()
	{
		return(beatDownStapm(1));
	}

	bool beatDownStapm(uint32_t tempStapmLimit)
	{
		if (enableStapmLimit)
		{
			set_stapm_limit(ryzenAccess, tempStapmLimit);
			set_stapm_limit(ryzenAccess, stapmLimit);
			//Sleep(ryzenAdjSleep);
		}
		return true;
	}
	
	void startTactiPoke(int messageID)
	{ 
		while (messageID > 1)
		{
			system("cls");
			messageID--;
			ofstream lastMsgID("lastMsgID.txt");
			lastMsgID << messageID;
			lastMsgID.flush();
			lastMsgID.close();
			vector<string> results;


			stringstream ssMsgID;
			stringstream ssMsgReport;


			ssMsgReport << messageID << "smu-tool" << ".txt\0";
			string msgReport = ssMsgReport.str();
			char* file = new char[msgReport.length() - 1]; std::copy(msgReport.begin(), msgReport.end(), file);

			ssMsgID << "smu-tool --message=" << messageID << " --arg0=15000> " << file;
			string msgID = ssMsgID.str();
			string getPrint;
			getPrint = uProf.getPrint(true, messageID);
			while (getPrint.length() < 1)
			{
				uProf.stopProf();
				Sleep(1000);
				uProf.initializeUProf();
				Sleep(100);
				getPrint = uProf.getPrint(true, 0);
				Sleep(samplingInterval);
				std::cout << "uProf Died Restarting Uprof" << '\n';
			}
			char* dest = new char[msgID.length() - 1]; std::copy(msgID.begin(), msgID.end(), dest);
			
			std::cout << dest << '\n';
			std::cout << file << '\n';
			std::cout << getPrint;
			
			//char* systemCall = "smu-tool --message=" + ssMsgID.str() + " --arg0=15000> smu-tool.txt";
			system(dest);	//if the file doesn't already exist it will create it
			//system("exit");	     //this maybe doesn't have to be here

			//reads each line and stores it as a string in the array 'results'
			ifstream in(file);
			string line;
			while (getline(in, line))
				results.push_back(line);
			for (int i = 0; i < results.size(); i++)
			{
				std::cout << "Line " << i << ": " << results[i] << '\n';
			}

			Sleep(1000);
		}
		
	}

	
};

#endif
