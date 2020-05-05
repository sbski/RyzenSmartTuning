#pragma once


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
	const std::string description[22] = {"STAPM Limit (mW)", "Fast Limit (Mw)", "Slow Limit (Mw)",  "Slow Time (S)",  "STAPM Time (S)",  "Tctl Temperature (Degrees C)",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  ""};
	uint32_t settings[26];
	bool enableSettings[26];

	ryzen_access ryzenAccess;
	

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


	RyzenSmartTuning()
	{
		
	}
	RyzenSmartTuning(bool enableChangeDetection, AMDTUInt32 customSamplingInterval, bool startUProfNow)
	{

		//Sleep(1000);

		samplingInterval = customSamplingInterval;

		uProf = UProfData(customSamplingInterval);

		uProf.initializeUProf(false,true,true);
		
		if (startUProfNow)
		{
			startUProf();
		}
		//getting a valid update
		Sleep(samplingInterval);
		uProf.print(false);
		stapmLimit = 28000;
		fastLimit = 32000;
		slowLimit = 30000;

		std::cout << stapmLimit << std::endl;
		std::cout << fastLimit << std::endl;
		std::cout << slowLimit << std::endl;

		slowTime = 0;
		stapmTime = 0;
		tctlTemp = 0;
		vrmCurrent = 0;
		vrmSocCurrent = 0;
		vrmMaxCurrent = 0;
		vrmSocMaxCurrent = 0;
		psi0Current = 0;
		psi0SocCurrent = 0;
		maxGfxClkFreq = 0;
		minGfxClkFreq = 0;
		maxSocClkFreq = 0;
		minSocClkFreq = 0;
		maxFclkFreq = 0;
		minFclkFreq = 0;
		maxVcn = 0;
		minVcn = 0;
		maxLclk = 0;
		minLclk = 0;
		
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

	bool initRyzenAdj()
	{
		//cleanup_ryzenadj(ryzenAccess);
		ryzenAccess = init_ryzenadj();
		return true;
	}
	bool changeSettings()
	{

	}

	void print()
	{
		uProf.print();
	}

	bool settingsChanged()
	{
		//uProf.print(false);
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
		//pretty straight forward. If the setting has a value other than 0, it applies the setting.
		if (enableStapmLimit)
		{
			set_stapm_limit(ryzenAccess, stapmLimit);
			Sleep(50);
		}
		if (enableFastLimit)
		{
			set_fast_limit(ryzenAccess, fastLimit);
			Sleep(50);
		}
		if (enableSlowLimit)
		{
			set_slow_limit(ryzenAccess, slowLimit);
			Sleep(50);
		}
		if(enableSlowTime)
		{
			set_slow_time(ryzenAccess, slowTime);
			Sleep(50);
		}
		if(enableStapmTime)
		{
			set_stapm_time(ryzenAccess, stapmTime);
			Sleep(50);
		}
		if(enableTctlTemp)
		{
			set_tctl_temp(ryzenAccess, tctlTemp); 
			Sleep(50);
		}
		if(enableVrmCurrent)
		{
			set_vrm_current(ryzenAccess, vrmCurrent);
			Sleep(50);
		}
		if(enableVrmSocCurrent)
		{
			set_vrmsoc_current(ryzenAccess, vrmSocCurrent);
			Sleep(50);
		}
		if(enableVrmMaxCurrent)
		{
			set_vrmmax_current(ryzenAccess, vrmMaxCurrent);
			Sleep(50);
		}
		if(enableVrmSocMaxCurrent)
		{
			set_vrmsocmax_current(ryzenAccess, vrmSocMaxCurrent);
			Sleep(50);
		}
		if(enablePsi0Current)
		{
			set_psi0_current(ryzenAccess, psi0Current);
			Sleep(50);
		}
		if(enablePsi0SocCurrent)
		{
			set_psi0soc_current(ryzenAccess, psi0SocCurrent);
			Sleep(50);
		}
		if(enableMaxGfxClkFreq)
		{
			set_max_gfxclk_freq(ryzenAccess, maxGfxClkFreq);
			Sleep(50);
		}
		if(enableMinGfxClkFreq)
		{
			set_min_gfxclk_freq(ryzenAccess, minGfxClkFreq);
			Sleep(50);
		}
		if(enableMaxSocClkFreq)
		{
			set_max_socclk_freq(ryzenAccess, maxSocClkFreq);
			Sleep(50);
		}
		if(enableMinSocClkFreq)
		{
			set_min_socclk_freq(ryzenAccess, minSocClkFreq);
			Sleep(50);
		}
		if (enableMaxFclkFreq)
		{
			set_max_fclk_freq(ryzenAccess, maxFclkFreq);
			Sleep(50);
		}
		if(enableMinFclkFreq)
		{
			set_min_fclk_freq(ryzenAccess, minFclkFreq);
			Sleep(50);
		}
		if (enableMaxVcn)
		{
			set_max_vcn(ryzenAccess, maxVcn);
			Sleep(50);
		}
		if(enableMinVcn)
		{
			set_min_vcn(ryzenAccess, minVcn);
			Sleep(50);
		}
		if (enableMaxLclk)
		{
			set_max_lclk(ryzenAccess, maxLclk);
			Sleep(50);
		}
		if (enableMinLclk)
		{
			set_min_lclk(ryzenAccess, minLclk);
			Sleep(50);
		}
		std::cout << "Applied Settings!\n";
		Sleep(1000);
		return true;
	}

	bool startUProf()
	{
		return uProf.startProf();
	}

	bool stopUProf()
	{
		return uProf.stopProf();
	}

	void startRST()
	{
		while (true)
		{
			print();
			if (settingsChanged())
			{
				applySettings();
			}
		}
	}
};