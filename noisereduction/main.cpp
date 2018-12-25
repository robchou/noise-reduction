#define LOG_TAG "main"
#include <iostream>
#include <fstream>
#include "NoiseReduction.h"
#include "cxxopts.hpp"
#include "log.h"

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

    Log::SetLogLevel(LOG_VERBOSE);
    const char* profilePath = "/Users/robin/Desktop/noise.pcm";
    const char* inputPath = "/Users/robin/Desktop/short.pcm";
    const char* outputPath = "/Users/robin/Desktop/out.pcm";
    const char* out = "/Users/robin/Desktop/out.pcm";

    std::ifstream profile;
    profile.open(profilePath, std::ios::binary);
    if (!profile.is_open()) {
        Log::e(LOG_TAG, "%s error: failed to open %s\n", profilePath);
    }
    profile.seekg(0, std::ios::end);
    int profileSize = profile.tellg();
    profile.seekg(std::ios::beg);
    auto profileBuffer = std::make_unique<int16_t[]>(profileSize / 2);
    profile.read(reinterpret_cast<char *>(profileBuffer.get()), profileSize / 2);
    profile.close();


    NoiseReduction::Settings settings;
//    settings.mNewSensitivity = result["sensitivity"].as<float>();
//    settings.mFreqSmoothingBands = result["smoothing"].as<int>();
//    settings.mNoiseGain = result["noiseGain"].as<float>();
    settings.mNewSensitivity = 6.0;
    settings.mFreqSmoothingBands = 3,
    settings.mNoiseGain = 48;
    NoiseReduction reduction(settings, 8000, 1);

    std::cout << "Profiling noise..." << std::endl;
    reduction.ExtractNoiseProfile(profileBuffer.get(), profileSize);
    std::cout << "Denoising..." << std::endl;
//    reduction.ReduceNoise(result["output"].as<std::string>().c_str());
    reduction.ReduceNoise(out);

    return 0;
}
