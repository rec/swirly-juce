#ifndef __REC_AUDIO_FORMAT_AUDIOFORMATOUTPUTDESCRIPTION__
#define __REC_AUDIO_FORMAT_AUDIOFORMATOUTPUTDESCRIPTION__

namespace juce {

struct AudioFormatOutputDescription
{
    unsigned int channels;
    int bitsPerSample;
    int sampleRate;

    String encoderName;       // The name of the output encoder:  mp3, wav, aiff, etc.
    StringPair encoderHints;  // Name/value pairs which might be meaningful to the encoder.
};

#if 0

class AudioFormat
{
    // ...
    virtual AudioFormatWriter* createWriterFor (OutputStream*, const OutputDescription&) = 0;
    virtual AudioFormatWriter* createWriterFor (OutputStream* streamToWriteTo,
                                                double sampleRate,
                                                unsigned int channels,
                                                int bitsPerSample,
                                                const StringPairArray& metadataValues,
                                                int qualityOptionIndex) {
        AudioFormatOutputDescription desc;

        desc.sampleRate = sampleRate;
        desc.channels = channels;
        desc.bitsPerSample = bitsPerSample;
        desc.encoderHints = metadataValues;
        desc.encoderHints["quality"] = getQualityOptions()[qualityOptionIndex];

        return createWriterFor(streamToWriteTo, desc);
    }
};

#endif

}  // namespace juce

#endif  // __REC_AUDIO_FORMAT_AUDIOFORMATOUTPUTDESCRIPTION__
