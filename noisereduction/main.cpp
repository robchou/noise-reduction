#define LOG_TAG "main"
#include <iostream>
#include <fstream>
#include "NoiseReduction.h"
#include "cxxopts.hpp"
#include "log.h"

int main(int argc, char * argv[]) {
    cxxopts::Options options("NoiseReduction Test", "Driver for Noise Reduction");
    options.add_options()
        ("i,input", "Input file (required)", cxxopts::value<std::string>())
        ("o,output", "Output file (required)", cxxopts::value<std::string>())
        ("n,noise", "Noise profile file (required)", cxxopts::value<std::string>())
        ("noiseGain", "Noise Gain (dB)", cxxopts::value<float>()->default_value("48"))
        ("sensitivity", "Sensitivity", cxxopts::value<float>()->default_value("6.0"))
        ("smoothing", "Frequency Smoothing (bands)", cxxopts::value<int>()->default_value("3"))
        ("verbose", "Verbose Output");

    options.parse_positional(std::vector<std::string>{"input", "output", "noise"});

    int unparsedArgc = argc;
    auto result = options.parse(unparsedArgc, argv);
    if (!result.count("input") || !result.count("output") || !result.count("noise")) {
        auto help = options.help();
        std::cout << help << std::endl;
        return 1;
    }

    if (result["verbose"].count()) {
        Log::SetLogLevel(LOG_VERBOSE);
    }

    std::cout << "Processing " << result["input"].as<std::string>() << " -> " << result["output"].as<std::string>() << std::endl;

    Log::SetLogLevel(LOG_VERBOSE);

    std::string profilePath = result["noise"].as<std::string>();
    std::ifstream profile;
    profile.open(profilePath, std::ios::binary);
    if (!profile.is_open()) {
        Log::e(LOG_TAG, "%s error: failed to open %s\n", profilePath.c_str());
    }
    profile.seekg(0, std::ios::end);
    int profileSize = profile.tellg();
    profile.seekg(std::ios::beg);
    auto profileBuffer = std::make_unique<int16_t[]>(profileSize / 2);
    profile.read(reinterpret_cast<char *>(profileBuffer.get()), profileSize);
    profile.close();


    NoiseReduction::Settings settings;
    settings.mNewSensitivity = result["sensitivity"].as<float>();
    settings.mFreqSmoothingBands = result["smoothing"].as<int>();
    settings.mNoiseGain = result["noiseGain"].as<float>();
    NoiseReduction reduction(settings, 8000, 1);

    std::cout << "Profiling noise..." << std::endl;
    reduction.ExtractNoiseProfile(profileBuffer.get(), profileSize);
    std::cout << "Denoising..." << std::endl;
    std::ifstream src;
    std::ofstream out;
    std::string inputPath = result["input"].as<std::string>();
    src.open(inputPath, std::ios::binary);
    if (!src.is_open()) {
        Log::e(LOG_TAG, "%s error: open %s failed!\n", inputPath.c_str());
        return -1;
    }

    std::string outputPath = result["output"].as<std::string>();
    out.open(outputPath, std::ios::binary);

    if (!out.is_open()) {
        Log::e(LOG_TAG, "%s error: open %s failed!\n", outputPath.c_str());
        return -1;
    }

    src.seekg(0, std::ios::end);
    int srcSize = src.tellg();
    src.seekg(std::ios::beg);
    auto srcBuffer = std::make_unique<int16_t[]>(srcSize / 2);
    auto outBuffer = std::make_unique<int16_t[]>(srcSize / 2);
    src.read(reinterpret_cast<char *>(srcBuffer.get()), srcSize);
    src.close();
    reduction.ReduceNoise(srcBuffer.get(), outBuffer.get(), srcSize);

    out.write(reinterpret_cast<const char *>(outBuffer.get()), srcSize);
    out.close();

    return 0;
}
