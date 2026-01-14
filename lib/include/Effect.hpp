#ifndef _EFFECT_HPP__
#define _EFFECT_HPP__
#include <xaudio2.h>
#include <xapo.h>
#include <xapobase.h>



// Classe custom

class __declspec(uuid("CF4AB938-8CE0-4AB7-A56C-0253B6018C26")) MyGainEffect : public CXAPOBase {
public:
    float m_gain;

    MyGainEffect() : CXAPOBase(&GainProperties), m_gain(1.0f) {}

    void Process(UINT32 inputProcessParameterCount,
        const XAPO_PROCESS_BUFFER_PARAMETERS* pInputProcessParameters,
        UINT32 outputProcessParameterCount,
        XAPO_PROCESS_BUFFER_PARAMETERS* pOutputProcessParameters,
        BOOL isEnabled) override
    {
        if (!isEnabled) {
            // Copier directement si désactivé
            memcpy(pOutputProcessParameters->pBuffer,
                pInputProcessParameters->pBuffer,
                pInputProcessParameters->ValidFrameCount * sizeof(float));
            return;
        }

        float* inBuf = (float*)pInputProcessParameters->pBuffer;
        float* outBuf = (float*)pOutputProcessParameters->pBuffer;

        for (UINT32 i = 0; i < pInputProcessParameters->ValidFrameCount; ++i) {
            outBuf[i] = inBuf[i] * m_gain; // Application du gain
        }
    }
};

// Propriétés de l’effet
static const XAPO_REGISTRATION_PROPERTIES GainProperties = {
    __uuidof(MyGainEffect), // GUID unique
    L"MyGainEffect",        // Nom
    L"Exemple custom gain", // Description
    XAPO_FLAG_FRAMERATE_MUST_MATCH,
    1, 1,                   // Nombre d’entrées/sorties
    0                       // Pas de paramètres dynamiques
};

#endif