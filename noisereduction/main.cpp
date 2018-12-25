#include <iostream>
#include "NoiseReduction.h"
#include <sndfile.h>
#include "loguru.hpp"
#include "Utils.h"
#include "cxxopts.hpp"

int main(int argc, char * argv[]) {
//    cxxopts::Options options("NoiseReduction Test", "Driver for Noise Reduction");
//    options.add_options()
//        ("i,input", "Input file (required)", cxxopts::value<std::string>())
//        ("o,output", "Output file (required)", cxxopts::value<std::string>())
//        ("noiseGain", "Noise Gain (dB)", cxxopts::value<float>()->default_value("48"))
//        ("sensitivity", "Sensitivity", cxxopts::value<float>()->default_value("6.0"))
//        ("smoothing", "Frequency Smoothing (bands)", cxxopts::value<int>()->default_value("3"))
//        ("verbose", "Verbose Output");
//
//    options.parse_positional(std::vector<std::string>{"input", "output"});

//    int unparsedArgc = argc;
//    auto result = options.parse(unparsedArgc, argv);
//    if (!result.count("input") || !result.count("output")) {
//        auto help = options.help();
//        std::cout << help << std::endl;
//        return 1;
//    }
//
//    loguru::g_stderr_verbosity = loguru::Verbosity_ERROR;
//    if (result["verbose"].count()) {
//        loguru::g_stderr_verbosity = loguru::Verbosity_1;
//    }
//
//    loguru::init(argc, argv);
//
//
//    std::cout << "Processing " << result["input"].as<std::string>() << " -> " << result["output"].as<std::string>() << std::endl;

    const char* out = "/Users/robin/Desktop/out.pcm";

    SndInfo inputCtxInfo = SndInfo {
        .channels = 1,
        .samplerate = 8000,
    };

    auto ctx = SndContext {
        .info = inputCtxInfo,
    };

    auto noiseContext = SndContext {
        .info = inputCtxInfo,
    };

    NoiseReduction::Settings settings;
//    settings.mNewSensitivity = result["sensitivity"].as<float>();
//    settings.mFreqSmoothingBands = result["smoothing"].as<int>();
//    settings.mNoiseGain = result["noiseGain"].as<float>();
    settings.mNewSensitivity = 6.0;
    settings.mFreqSmoothingBands = 3,
    settings.mNoiseGain = 48;
    NoiseReduction reduction(settings, ctx);

    std::cout << "Profiling noise..." << std::endl;
    reduction.ProfileNoise(noiseContext);
    std::cout << "Denoising..." << std::endl;
//    reduction.ReduceNoise(result["output"].as<std::string>().c_str());
    reduction.ReduceNoise(out);

    return 0;
}
