/*
 * DISTRHO OneKnob Series
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

#include "DistrhoPlugin.hpp"
#include "SharedMemory.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------

class OneKnobPlugin : public Plugin
{
public:
    OneKnobPlugin()
        : Plugin(kParameterCount, kProgramCount, 0),
          lineGraphFrameToReset(getSampleRate() / 120)
    {
        std::memset(parameters, 0, sizeof(parameters));
    }

protected:
    // -------------------------------------------------------------------
    // Information

    const char* getLabel() const noexcept override
    {
        return DISTRHO_PLUGIN_NAME;
    }

    const char* getMaker() const noexcept override
    {
        return DISTRHO_PLUGIN_BRAND;
    }

    const char* getHomePage() const override
    {
        return DISTRHO_PLUGIN_URI;
    }

    uint32_t getVersion() const noexcept override
    {
        return d_version(1, 0, 0);
    }

    // -------------------------------------------------------------------
    // Parameters

    float getParameterValue(const uint32_t index) const override
    {
        return parameters[index];
    }

    void setParameterValue(const uint32_t index, const float value) override
    {
        parameters[index] = value;
    }

    // -------------------------------------------------------------------
    // State

    void initState(uint32_t, String&, String&) override
    {
    }

    void setState(const char* const key, const char* const value) override
    {
        if (std::strcmp(key, "filemapping") == 0)
        {
            if (lineGraphsData.isCreatedOrConnected())
            {
                DISTRHO_SAFE_ASSERT(! lineGraphActive);
                lineGraphIn.setFloatFifo(nullptr);
                lineGraphOut.setFloatFifo(nullptr);
                lineGraphsData.close();
            }

            if (OneKnobLineGraphFifos* const fifos = lineGraphsData.connect(value))
            {
                lineGraphIn.setFloatFifo(&fifos->in);
                lineGraphOut.setFloatFifo(&fifos->out);
                lineGraphActive = true;
            }
        }
    }

    // -------------------------------------------------------------------
    // Process

    void activate() override
    {
        lineGraphFrameCounter = 0;
        lineGraphHighestIn = lineGraphHighestOut = 0.0f;
    }

    void sampleRateChanged(const double newSampleRate) override
    {
        lineGraphFrameToReset = newSampleRate / 120;
    }

    // -------------------------------------------------------------------

    void init()
    {
        // load values of default/first program
        loadDefaultParameterValues();

        // reset state if needed
        deactivate();
    }

    void loadDefaultParameterValues()
    {
        for (uint i=0; i<kParameterCount; ++i)
            parameters[i] = kParameterDefaults[i];
    }

    inline void setMeters(const float in, const float out)
    {
        if (! lineGraphActive)
            return;

        if (lineGraphsData.getDataPointer()->closed)
        {
            lineGraphActive = false;
            return;
        }

        lineGraphIn.write(in);
        lineGraphOut.write(out);
    }

    // -------------------------------------------------------------------

    float parameters[kParameterCount];

    bool lineGraphActive = false;
    uint32_t lineGraphFrameCounter = 0;
    uint32_t lineGraphFrameToReset;
    float lineGraphHighestIn = 0.0f;
    float lineGraphHighestOut = 0.0f;

private:
    OneKnobFloatFifoControl lineGraphIn;
    OneKnobFloatFifoControl lineGraphOut;
    SharedMemory<OneKnobLineGraphFifos> lineGraphsData;

    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OneKnobPlugin)
};

// -----------------------------------------------------------------------

END_NAMESPACE_DISTRHO
