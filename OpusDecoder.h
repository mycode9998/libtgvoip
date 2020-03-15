//
// libtgvoip is free and unencumbered public domain software.
// For more information, see http://unlicense.org or the UNLICENSE file
// you should have received with this source code distribution.
//

#ifndef LIBTGVOIP_OPUSDECODER_H
#define LIBTGVOIP_OPUSDECODER_H

#include "BlockingQueue.h"
#include "Buffers.h"
#include "EchoCanceller.h"
#include "JitterBuffer.h"
#include "MediaStreamItf.h"
#include "threading.h"
#include "utils.h"
#include <atomic>
#include <memory>
#include <cstdio>
#include <vector>

struct OpusDecoder;

namespace tgvoip
{
class OpusDecoder
{
public:
    TGVOIP_DISALLOW_COPY_AND_ASSIGN(OpusDecoder);
    virtual void Start();

    virtual void Stop();

    OpusDecoder(const std::shared_ptr<MediaStreamItf>& dst, bool isAsync, bool needEC);
    OpusDecoder(const std::unique_ptr<MediaStreamItf>& dst, bool isAsync, bool needEC);
    OpusDecoder(MediaStreamItf* dst, bool isAsync, bool needEC);
    virtual ~OpusDecoder();
    std::size_t HandleCallback(unsigned char* data, std::size_t len);
    void SetEchoCanceller(EchoCanceller* canceller);
    void SetFrameDuration(std::uint32_t duration);
    void SetJitterBuffer(std::shared_ptr<JitterBuffer> jitterBuffer);
    void SetDTX(bool enable);
    void SetLevelMeter(AudioLevelMeter* levelMeter);
    void AddAudioEffect(effects::AudioEffect* effect);
    void RemoveAudioEffect(effects::AudioEffect* effect);

private:
    void Initialize(bool isAsync, bool needEC);
    static std::size_t Callback(unsigned char* data, std::size_t len, void* param);
    void RunThread();
    int DecodeNextFrame();
    ::OpusDecoder* dec;
    ::OpusDecoder* ecDec;
    BlockingQueue<Buffer>* decodedQueue;
    BufferPool<960 * 2, 32> bufferPool;
    unsigned char* buffer;
    unsigned char* lastDecoded;
    unsigned char* processedBuffer;
    std::size_t outputBufferSize;
    std::atomic<bool> running;
    Thread* thread;
    Semaphore* semaphore;
    std::uint32_t frameDuration;
    EchoCanceller* echoCanceller;
    std::shared_ptr<JitterBuffer> jitterBuffer;
    AudioLevelMeter* levelMeter;
    int consecutiveLostPackets;
    bool enableDTX;
    std::size_t silentPacketCount;
    std::vector<effects::AudioEffect*> postProcEffects;
    std::atomic<bool> async;
    alignas(2) unsigned char nextBuffer[8192];
    alignas(2) unsigned char decodeBuffer[8192];
    std::size_t nextLen;
    unsigned int packetsPerFrame;
    ptrdiff_t remainingDataLen;
    bool prevWasEC;
    std::int16_t prevLastSample;
};
}

#endif //LIBTGVOIP_OPUSDECODER_H
