#include <rtl-sdr.h>

#include <vector>
#include <iostream>

#include <string>
#include <stdexcept>
#include <algorithm>



rtlsdr_dev_t* open_dev(const std::string& serial) {
    int device_count = rtlsdr_get_device_count();
    if (device_count <= 0) {
        throw std::runtime_error("No RTL-SDR devices found");
    }

    char device_serial[256];
    for (int i = 0; i < device_count; ++i) {
        rtlsdr_get_device_usb_strings(i, nullptr, nullptr, device_serial);
        if (serial == device_serial) {
            rtlsdr_dev_t* dev = nullptr;
            if (rtlsdr_open(&dev, i) != 0 || dev == nullptr) {
                throw std::runtime_error("Failed to open device with serial " + serial);
            }
            return dev;
        }
    }

    throw std::runtime_error("Device with serial " + serial + " not found");
}

// true - auto gain, false - manual gain
inline void set_gain_mode(rtlsdr_dev_t* dev, bool auto_gain) {
    if (rtlsdr_set_tuner_gain_mode(dev, auto_gain ? 1 : 0) != 0) {
        throw std::runtime_error("Failed to set tuner gain mode");
    }
}

// Parse gain value in dB
// Example: parse_db_gain("20.5 dB") -> 205
// Removes all whitespaces from the string
int parse_db_gain(std::string gain_str)
{
#define DBG_VERBOSE 0
#if DBG_VERBOSE
        std::cout << "### Parsing gain string: " << gain_str; 
#endif 
    gain_str.erase(std::remove_if(gain_str.begin(), gain_str.end(), ::isspace), gain_str.end());
    // if ( gain_str.size() < 2)  
    // {
    //     if ( gain_str.substr(gain_str.size() - 2) != "db" )
    //         if ( gain_str.substr(gain_str.size() - 2) != "dB")
    //             throw std::invalid_argument("Invalid gain value (db is abscent): " + gain_str);
    // } 
    if (gain_str.size() < 2 || 
        (gain_str.substr(gain_str.size() - 2) != "db" && gain_str.substr(gain_str.size() - 2) != "dB")) 
    {
#if DBG_VERBOSE
        std::cout << " - db fail " << std::endl; 
#endif        
        throw std::invalid_argument("Invalid gain value (db is absent): " + gain_str);
    }

    int rv = int(std::stof(gain_str) * 10);
#if DBG_VERBOSE
       std::cout << " result " << rv << std::endl; 
#endif 
    return rv;
}

std::vector<int> get_dev_gains(rtlsdr_dev_t* dev, bool show_gains = false)
{
    // get supported gain values
    int num_gains = rtlsdr_get_tuner_gains(dev, nullptr);
    if (num_gains <= 0) {
        throw std::runtime_error("Failed to get tuner gains");
    }

    // allocate memory for gain values
    auto gains = std::vector<int>(num_gains);
    if (rtlsdr_get_tuner_gains(dev, gains.data()) != num_gains) {
        throw std::runtime_error("Failed to get tuner gains");
    }

    if (show_gains)
    {
        std::cout << "### Supported gain values (dB): ";
        for (int i = 0; i < num_gains; ++i) {
            std::cout << gains[i] / 10.0 << " ";
        }
    }
    return gains;
}

void set_gain(rtlsdr_dev_t* dev, std::string gain_str)
{
    std::transform(gain_str.begin(), gain_str.end(), gain_str.begin(), ::tolower);

    if (gain_str == "auto") {
        set_gain_mode(dev, true);
        return;
    }
    
    int gain_value = parse_db_gain(gain_str);
    // get supported gain values
    {
        int num_gains = rtlsdr_get_tuner_gains(dev, nullptr);
        if (num_gains <= 0) {
            throw std::runtime_error("Failed to get tuner gains");
        }

        // allocate memory for gain values
        auto gains = std::vector<int>(num_gains);
        if (rtlsdr_get_tuner_gains(dev, gains.data()) != num_gains) {
            throw std::runtime_error("Failed to get tuner gains");
        }

        bool gain_supported = false;
        for (int i = 0; i < num_gains; ++i) {
            if (gains[i] == gain_value) {
                gain_supported = true;
                break;
            }
        }

        if (!gain_supported) {
            throw std::runtime_error("Gain value " + gain_str + " is not supported");
        }
    }
    

    set_gain_mode(dev, true);
    if (rtlsdr_set_tuner_gain(dev, gain_value) != 0) {
        throw std::runtime_error("Failed to set tuner gain");
    }

}