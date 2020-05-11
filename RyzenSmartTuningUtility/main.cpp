#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>
#include <thread>
#include <cstdint>
#include <fstream>
#include<sstream>
using namespace std;

#include <AMDTPowerProfileApi.h>
#include <ryzenadj.h>

#include "C:/RyzenSmartTuning/RyzenSmartTuning/UProfData.h"
#include "C:/RyzenSmartTuning/RyzenSmartTuning/ryzensmarttuning.h"


#include "ryzensmarttuningutility.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RyzenSmartTuningUtility w;
    w.show();
    return a.exec();
}
