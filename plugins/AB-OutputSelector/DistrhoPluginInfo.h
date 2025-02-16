/*
 * DISTRHO OneKnob A/B Output Selector
 * Copyright (C) 2021 Filipe Coelho <falktx@falktx.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * For a full copy of the GNU General Public License see the LICENSE file.
 */

#pragma once

#include "OneKnobPluginInfo.h"

#define DISTRHO_PLUGIN_NAME         "OneKnob A/B Output Selector"
#define DISTRHO_PLUGIN_URI          "https://kx.studio/Plugins:OneKnob#AB-OutputSelector"
#define DISTRHO_PLUGIN_LV2_CATEGORY "lv2:UtilityPlugin"

#undef DISTRHO_PLUGIN_NUM_OUTPUTS
#define DISTRHO_PLUGIN_NUM_OUTPUTS 4

enum Parameters
{
    kParameterSelect,
    kParameterMode,
    kParameterCount
};

enum Programs
{
    kProgramInit,
    kProgramCount
};

enum States
{
    kStateCount
};

static const float kParameterDefaults[kParameterCount] = {
    0.0f,
    0.0f
};
