#include "Amp.h"
#include <iostream>

Amp::Amp(const char *ampData, const int ampDataSize) : mAmpData(ampData), mAmpDataSize(ampDataSize)
{

    std::string json_str(reinterpret_cast<const char *>(mAmpData), mAmpDataSize);

    auto j = nlohmann::json::parse(json_str);
    nam::verify_config_version(j["version"]);

    auto architecture = j["architecture"];
    nlohmann::json config = j["config"];

    std::vector<float> weights;
    if (j.find("weights") != j.end())
    {
        auto weight_list = j["weights"];
        for (auto it = weight_list.begin(); it != weight_list.end(); ++it)
            weights.push_back(*it);
    }
    else
        throw std::runtime_error("Corrupted model file is missing weights.");

    nam::dspData returnedConfig;
    // Assign values to returnedConfig
    returnedConfig.version = j["version"];
    returnedConfig.architecture = j["architecture"];
    returnedConfig.config = j["config"];
    returnedConfig.metadata = j["metadata"];
    returnedConfig.weights = weights;
    if (j.find("sample_rate") != j.end())
        returnedConfig.expected_sample_rate = j["sample_rate"];
    else
    {
        returnedConfig.expected_sample_rate = -1.0;
    }

    /*Copy to a new dsp_config object for get_dsp below,
    since not sure if params actually get modified as being non-const references on some
    model constructors inside get_dsp(dsp_config& conf).
    We need to return unmodified version of dsp_config via returnedConfig.*/
    nam::dspData conf = returnedConfig;

    mNamModel = get_dsp(conf);

    if (!mNamModel)
    {
        std::cout << "Nam FAILED!!!" << std::endl;
        exit(0);
    }

    // mIR = std::make_shared<Convolution>(test_raw, test_raw_len, 48000);
    // mNoiseGate = std::make_shared<NoiseGate>(-80.0, 0.00001, 0.05, 0.05, 48000);

    // try
    // {
    //     std::string json_str(
    //         reinterpret_cast<const char *>(__Fireball_Ironmaster_v1_Fireball_IronMaster_v3_0_0_reamp_cm_St_nam),
    //         __Fireball_Ironmaster_v1_Fireball_IronMaster_v3_0_0_reamp_cm_St_nam_len);

    //     auto j = nlohmann::json::parse(json_str);
    //     nam::verify_config_version(j["version"]);

    //     auto architecture = j["architecture"];
    //     nlohmann::json config = j["config"];

    //     std::vector<float> weights;
    //     if (j.find("weights") != j.end())
    //     {
    //         auto weight_list = j["weights"];
    //         for (auto it = weight_list.begin(); it != weight_list.end(); ++it)
    //             weights.push_back(*it);
    //     }
    //     else
    //     {
    //         throw std::runtime_error("Corrupted model file is missing weights.");
    //     }

    //     nam::dspData returnedConfig;
    //     returnedConfig.version = j["version"];
    //     returnedConfig.architecture = j["architecture"];
    //     returnedConfig.config = j["config"];
    //     returnedConfig.metadata = j["metadata"];
    //     returnedConfig.weights = weights;
    //     returnedConfig.expected_sample_rate = j.value("sample_rate", -1.0);

    //     // Copy to a new `dspData` object for `get_dsp`
    //     nam::dspData conf = returnedConfig;

    //     mNamModel = get_dsp(conf);

    //     if (!mNamModel)
    //     {
    //         throw std::runtime_error("NAM model initialization failed!");
    //     }

    // mNoiseGate = std::make_unique<NoiseGate>(-40.0, 48000);
    // }
    // catch (const std::exception &e)
    // {
    //     std::cerr << "Error initializing BlackLoungeAmp: " << e.what() << std::endl;
    //     mNamModel.reset(); // Reset to ensure it's null if initialization failed
    // }
    // auto test = "C:/Users/erikg/Development/audio/Black_Lounge/Fireball "
    //             "Ironmaster_v1_Fireball-IronMaster-v3_0_0-reamp.cm-St.nam";
    // auto path = std::filesystem::path(test);
    // mNamModel = nam::get_dsp(path);
    // if (mNamModel)
    // {
    //     // mAmp->getNamModel()->ResetAndPrewarm(sampleRate, samplesPerBlock);
    //     // mNamModel->Reset()
    //     int hej;
    //     hej = 3;
    // }
}

Amp::~Amp()
{
}

void Amp::prepare()
{
}

void Amp::process(juce::AudioBuffer<float> &buffer)
{
    std::ignore = buffer;
    // if (mNamModel)
    // {
    //     mNamModel->process(input, output, numFrames);
    // }

    // for (int i = 0; i < numFrames; ++i)
    // {
    //     output[i] *= mNoiseGate->process(input[i]);
    // }
}

void Amp::setThreshold(float threshold)
{
    std::ignore = threshold;
    // mNoiseGate->setThreshold(threshold);
}
