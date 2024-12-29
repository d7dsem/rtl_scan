#include <catch2/catch_test_macros.hpp>
#include <rtl-sdr.h>
#include <string>
#include <stdexcept>
#include <iostream>
#include <memory>

extern rtlsdr_dev_t *open_dev(const std::string &serial);

extern int parse_db_gain(std::string gain_str); 

extern void set_gain(rtlsdr_dev_t* dev, std::string gain_str);
extern std::vector<int> get_dev_gains(rtlsdr_dev_t* dev, bool show_gains);

#define ALL_ON 0
#if ALL_ON
TEST_CASE("Testing open_dev with various serial numbers") 
{

    SECTION("Valid serial number") {
        rtlsdr_dev_t* dev = nullptr;
        REQUIRE_NOTHROW(dev = open_dev("000000D7"));
        REQUIRE(dev != nullptr);
        rtlsdr_close(dev);
    }

    SECTION("Invalid serial number") {
        REQUIRE_THROWS_AS(open_dev("INVALID_SERIAL"), std::runtime_error);
    }

    SECTION("Empty serial number") {
        REQUIRE_THROWS_AS(open_dev(""), std::runtime_error);
    }
}
#endif // open_dev

#if ALL_ON 
TEST_CASE("Testing parse_db_gain with various input strings") 
{

    SECTION("Valid inputs") {
        std::vector<std::string> gains = {
            "-9.900000 dB", "-4.000000 dB", "7.100000 dB", "17.900000 dB", "19.200000 dB",
            "20.5 dB", "20.5dB", "15 dB", "0 dB", "-10 dB", "1.2db"};
        std::vector<int> expected = {-99, -40, 71, 179, 192, 205, 205, 150, 0, -100, 12};
        for (size_t i = 0; i < gains.size(); ++i) {
            REQUIRE(parse_db_gain(gains[i]) == expected[i]);
        }
    }

    SECTION("Inputs with spaces") {
        REQUIRE(parse_db_gain("  10 dB  ") == 100);
        REQUIRE(parse_db_gain("  -20.5  dB ") == -205);
    }

    SECTION("Invalid inputs") {
        REQUIRE_THROWS_AS(parse_db_gain("abcd"), std::invalid_argument);
        REQUIRE_THROWS_AS(parse_db_gain("10d"), std::invalid_argument);
        REQUIRE_THROWS_AS(parse_db_gain(""), std::invalid_argument);
        REQUIRE_THROWS_AS(parse_db_gain("awto"), std::invalid_argument);
    }
}
#endif // parse_db_gain


#if ALL_ON
struct RtlDeviceCloser {
    void operator()(rtlsdr_dev_t* dev) const {
        if (dev) {
            rtlsdr_close(dev);
        }
    }
};

#if ALL_ON
TEST_CASE("Testing set_gain 1") 
{
    auto dev = std::shared_ptr<rtlsdr_dev_t>(open_dev("000000D7"), RtlDeviceCloser());
    REQUIRE(dev != nullptr);
    
    auto gains_i = get_dev_gains(dev.get(), false);

    std::vector<std::string> gains(gains_i.size());
    for (size_t i = 0; i < gains_i.size(); ++i) {
        gains[i] = std::to_string(int(gains_i[i]) / 10.0) + " dB";
    }
    std::cout << "### Valid gains  ";
    for (const auto &gain : gains)
    {
        std::cout << gain << "  "; 
        
    }
    std::cout << std::endl;

    SECTION("Valid Gains") {
        REQUIRE_NOTHROW(set_gain(dev.get(), "auto"));
        REQUIRE_NOTHROW(set_gain(dev.get(), "Auto"));
        REQUIRE_NOTHROW(set_gain(dev.get(), "AUTO"));
        for( const auto &gain : gains) REQUIRE_NOTHROW(set_gain(dev.get(), gain));
    }
}
#endif // set_gain - valid

#if ALL_ON
TEST_CASE("Testing set_gain 2") 
{
    auto dev = std::shared_ptr<rtlsdr_dev_t>(open_dev("000000D7"), RtlDeviceCloser());
    REQUIRE(dev != nullptr);
    std::vector<std::string> gains = {"0 d", "4d", "awto00", "Aqw:"};
    SECTION("Invalid Gains") {
        SECTION("Invalid gains") 
        {
            auto check_exception = [&](const std::string& gain) {
                try {
                    set_gain(dev.get(), gain);
                    FAIL("Expected exception not thrown");
                } catch (const std::runtime_error& e) {
                    SUCCEED("Caught std::runtime_error");
                } catch (const std::invalid_argument& e) {
                    SUCCEED("Caught std::invalid_argument");
                } catch (...) {
                    FAIL("Unexpected exception type");
                }
            };
            for (const auto& gain : gains) {
                check_exception(gain);
            }
        }
    }
}
#endif // set_gain - invalid

#endif // set_gain
