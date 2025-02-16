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

// IDE helper (not needed for building)
#include "DistrhoPluginInfo.h"

#include "OneKnobUI.hpp"

START_NAMESPACE_DISTRHO

// --------------------------------------------------------------------------------------------------------------------

static const uint kDefaultWidth = 640;
static const uint kDefaultHeight = 400;

static const OneKnobMainControl main = {
    kParameterSelect,
    "A/B Select",
    "%",
    -100.0f,
    100.0f,
    0.0f
};

static const OneKnobAuxiliaryComboBoxValue comboBoxValues[] = {
    {
        0, "Mode 1", "Explain mode 1"
    },
    {
        0, "Mode 2", "Explain mode 2"
    },
};

static const OneKnobAuxiliaryComboBox comboBox = {
    kParameterMode,
    "Mode",
    sizeof(comboBoxValues)/sizeof(comboBoxValues[0]),
    comboBoxValues
};

// --------------------------------------------------------------------------------------------------------------------

class OneKnobOutputSelectorUI : public OneKnobUI
{
public:
    OneKnobOutputSelectorUI()
        : OneKnobUI(kDefaultWidth, kDefaultHeight)
    {
        // setup OneKnob UI
        const Rectangle<uint> mainArea(kSidePanelWidth,
                                       kDefaultHeight*3/16 - kSidePanelWidth,
                                       kDefaultWidth/2 - kSidePanelWidth,
                                       kDefaultHeight*9/16);
        createMainControl(mainArea, main);

        const Rectangle<uint> comboBoxArea(kDefaultWidth/2,
                                           kDefaultHeight/4,
                                           kDefaultWidth/2 - kSidePanelWidth,
                                           kDefaultHeight*3/4);
        createAuxiliaryComboBox(comboBoxArea, comboBox);

        repositionWidgets();

        // set default values
        programLoaded(0);
    }

protected:
    // -------------------------------------------------------------------
    // DSP Callbacks

    void parameterChanged(const uint32_t index, const float value) override
    {
        switch (index)
        {
        case kParameterSelect:
            setMainControlValue(value);
            break;
        case kParameterMode:
            setAuxiliaryComboBoxValue(value);
            break;
        }

        repaint();
    }

    void programLoaded(const uint32_t index) override
    {
        switch (index)
        {
        case kProgramInit:
            setMainControlValue(kParameterDefaults[kParameterSelect]);
            setAuxiliaryComboBoxValue(kParameterDefaults[kParameterMode]);
            break;
        }

        repaint();
    }

    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OneKnobOutputSelectorUI)
};

// --------------------------------------------------------------------------------------------------------------------

UI* createUI()
{
    return new OneKnobOutputSelectorUI();
}

// --------------------------------------------------------------------------------------------------------------------

END_NAMESPACE_DISTRHO
