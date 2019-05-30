#pragma once

#include <portaudio/portaudio.h>
#include <vector>

#define DO 261.626f
#define RE 293.665f
#define MI 329.628f
#define FA 349.228f
#define SOL 391.995f
#define LA 440.f
#define SI 493.883f

class Imp_Audio
{
public:
    void InitAudio(int startFire, int startGeometry, int startPlasma, int startPlanes, int startEnding);
    void UpdateSound(float deltaTime, float accumulatedTime, bool tunnelBeat);
    void CloseAudio();

private:
    struct Envelope
    {
        float attack, decay, sustain, release;
        float peakAmplitude, sustainAmplitude;
    };

    struct Note
    {
        float (*generateWave)(float frequency, long int currentCount);
        Envelope envelope;
        float frequency = 440.f;
        float volume = 1.f;
        float position = 0.5f; // 0 = left, 1 = right, 0.5 = center
        float lifetime = 0.f;
        float currentEnvelopeValue = 0.f;
        float resultingSound;
    };

    const float mask[12] = {0.01f, 0.025f, 0.05f, 0.075f, 0.14f, 0.2f, 0.2f, 0.14f, 0.075f, 0.05f, 0.025f, 0.01f};

    const int SAMPLE_RATE = 44100;
    const int FRAMES_PER_BUFFER = 256;
    const int INPUT_CHANNELS = 0;
    const int OUTPUT_CHANNELS = 2;

    void RemoveDeadNotes();
    void UpdateEnvelopes(float deltaTime);
    static int AudioCallback(const void *, void *, unsigned long, const PaStreamCallbackTimeInfo *, PaStreamCallbackFlags, void *);

    static float GetLeftValue();
    static float GetRightValue();
    static void UpdateNotes(long int currentCount);

    PaStream *stream;
    static std::vector<Note> notes;

    int START_FIRE, START_GEOMETRY, START_PLASMA, START_PLANES, START_ENDING;
};