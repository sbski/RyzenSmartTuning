#include "ryzensmarttuningutility.h"
#include "ui_ryzensmarttuningutility.h"

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

RyzenSmartTuningUtility::RyzenSmartTuningUtility(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::RyzenSmartTuningUtility)
{
    ui->setupUi(this);
}

RyzenSmartTuningUtility::~RyzenSmartTuningUtility()
{
    delete ui;
}
