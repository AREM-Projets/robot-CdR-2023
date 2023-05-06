#pragma once

#include "config.h"

volatile bool mpuInterrupt;

void dmpDataReady()
{
    mpuInterrupt = true;
}

bool dmpReady = false;

bool strategie = true;