//
// libtgvoip is free and unencumbered public domain software.
// For more information, see http://unlicense.org or the UNLICENSE file
// you should have received with this source code distribution.
//

#ifndef LIBTGVOIP_AUDIOOUTPUTPULSE_H
#define LIBTGVOIP_AUDIOOUTPUTPULSE_H

#include "../../audio/AudioOutput.h"
#include "../../threading.h"
#include <pulse/pulseaudio.h>

namespace tgvoip
{
namespace audio
{

    class AudioOutputPulse : public AudioOutput
    {
    public:
        AudioOutputPulse(pa_context* context, pa_threaded_mainloop* mainloop, std::string devID);
        virtual ~AudioOutputPulse();
        virtual void Start();
        virtual void Stop();
        virtual bool IsPlaying();
        virtual void SetCurrentDevice(std::string devID);
        static bool EnumerateDevices(std::vector<AudioOutputDevice>& devs);

    private:
        static void StreamStateCallback(pa_stream* s, void* arg);
        static void StreamWriteCallback(pa_stream* stream, std::size_t requested_bytes, void* userdata);
        void StreamWriteCallback(pa_stream* stream, std::size_t requestedBytes);
        pa_stream* CreateAndInitStream();

        pa_threaded_mainloop* mainloop;
        pa_context* context;
        pa_stream* stream;

        bool isPlaying;
        bool isConnected;
        bool didStart;
        bool isLocked;
        unsigned char remainingData[960 * 8 * 2];
        std::size_t remainingDataSize;
    };

}
}

#endif //LIBTGVOIP_AUDIOOUTPUTPULSE_H
