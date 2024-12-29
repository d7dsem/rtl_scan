#ifndef __D7_AUX_H__
#define  __D7_AUX_H__

#if __cplusplus < 201703L
    #error "This code requires at least C++17. Please use a C++17 (or later) compliant compiler."
#endif

#include <stdint.h>

#if defined(_WIN32) || defined(_WIN64) 
#include <Windows.h>
#include <direct.h>
#include <sysinfoapi.h>  
using ssize_t = int64_t;
#else
    #include <unistd.h>
    #include <sys/wait.h>
    #include <sys/types.h>
    #include <sys/sysinfo.h> 
#endif

#ifndef MAX_PATH
    #define MAX_PATH 4096
#endif

// #include "../third-party/nlohmann/json.hpp"


#include <memory>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <locale>
#include <filesystem>
#include <functional>
#include <optional>
#include <vector>
#include <algorithm>
#include <random>
#include <regex>

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <sstream>

#ifndef __SHRT_MAX__
    #define __SHRT_MAX__ 32767
#endif

#ifndef __SHRT_MIN__
    #define __SHRT_MIN__ -32768
#endif

#define MEGA(v) ((v)*1000*1000)
#define KILO(v) ((v)*1000)

#ifndef _countof
#define _countof(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif

using OSS = std::ostringstream;
#define PUSH_HERE "[" << BASE_FILE_NAME(__FILE__) << ":" << __LINE__ << "]  "

using STD_PATH = std::filesystem::path;
#define BASE_FILE_NAME(path) (STD_PATH(path).filename().u8string())
#define HERE std::string("[" + BASE_FILE_NAME(__FILE__) + ":" + std::to_string(__LINE__) + "]  ")

#define FOLDER_NAME(path) (STD_PATH(path).parent_path().u8string())
#define PATH_2_STR(path) (STD_PATH(path).u8string ())



using error_handler_t = std::function<void(std::string)>;

using tick_t = std::chrono::steady_clock::time_point;
using steady_clock_t = std::chrono::steady_clock;


using duration_t = std::chrono::steady_clock::duration;

using TICK_HR = std::chrono::high_resolution_clock;


std::ostream& operator<<(std::ostream& os, const duration_t& duration);
#define SEC_FROM_DUR(duration) (std::chrono::duration_cast<std::chrono::milliseconds>((duration)).count()/1000.0)
#define DUR_FROM_SEC(duration_sec) (duration_t(static_cast<long long>((duration_sec) * 1e9)))

// Use in main (or where argc and argv present)
#define SHOW_COMMAND_LINE() {\
    for ( int i=0; i<argc; i++) {\
        printf("argv[%d]: %s\n",i, argv[i]);\
    }\
    printf("  argc: %d\n", argc);\
}


#if defined(_WIN32) || defined(_WIN64)

#define FILE_SIZE_x64(_file_size, file) { \
    _fseeki64(file, 0, SEEK_END); \
    (_file_size) = _ftelli64(file); \
    _fseeki64(file, 0, SEEK_SET); \
}

#else

#define FILE_SIZE_x64(_file_size, file) { \
    fseeko(file, 0, SEEK_END); \
    (_file_size) = ftello(file); \
    fseeko(file, 0, SEEK_SET); \
}

#endif  // #if defined(_WIN32) || defined(_WIN64)

using error_handler_t = std::function<void(const std::string)>;
using tick_t = std::chrono::steady_clock::time_point;
using duration_t = std::chrono::steady_clock::duration;

#define NOW std::chrono::steady_clock::now()
#define GET_SEC(v) std::chrono::duration_cast<std::chrono::duration<double>>((v)).count()

constexpr char PATH_SEP = std::filesystem::path::preferred_separator;

class PRINTABLE {
public:
    // virtual std::string to_string() const {return "[...]";}; 
    virtual std::string to_string() const  = 0;
    virtual ~PRINTABLE() = default;
};
std::ostream& operator<<(std::ostream& os, const PRINTABLE& obj);

namespace CONSOLE {


enum RGBColorsBase : uint32_t {
    RED     = 0xFF0000,  // Red
    GREEN   = 0x00FF00,  // Green
    BLUE    = 0x0000FF,  // Blue
    YELLOW  = 0xFFFF00,  // Yellow
    CYAN    = 0x00FFFF,  // Cyan
    MAGENTA = 0xFF00FF,  // Magenta
    WHITE   = 0xFFFFFF,  // White
    BLACK   = 0x000000   // Black
};

// Colors esk-str
namespace COLORS {
    constexpr const char* RED = "\x1b[31m";
    constexpr const char* GREEN = "\x1b[32m";
    constexpr const char* BRIGHT_GREEN = "\x1b[92m";
    constexpr const char* YELLOW = "\x1b[33m";
    constexpr const char* BLUE = "\x1b[34m";
    constexpr const char* MAGENTA = "\x1b[35m";
    constexpr const char* CYAN = "\x1b[36m";
    constexpr const char* GRAY = "\x1b[90m";
    constexpr const char* RESET = "\x1b[0m";
}

constexpr const char* ERROR_STR = "\x1b[31m""ERROR""\x1b[0m";

#define PUSH_GREEN(v) CONSOLE::COLORS::GREEN << (v) << CONSOLE::COLORS::RESET
#define PUSH_CYAN(v) CONSOLE::COLORS::CYAN << (v) << CONSOLE::COLORS::RESET
#define PUSH_YELLOW(v) CONSOLE::COLORS::YELLOW << (v) << CONSOLE::COLORS::RESET
#define PUSH_RED(v) CONSOLE::COLORS::RED << (v) << CONSOLE::COLORS::RESET
#define PUSH_BLUE(v) CONSOLE::COLORS::BLUE << (v) << CONSOLE::COLORS::RESET
#define PUSH_GRAY(v) CONSOLE::COLORS::GRAY << (v) << CONSOLE::COLORS::RESET
#define PUSH_MAGENTA(v) CONSOLE::COLORS::MAGENTA << (v) << CONSOLE::COLORS::RESET

#define PUSH_ERR CONSOLE::COLORS::RED << "[ERROR] " << CONSOLE::COLORS::RESET

#define PUSH_INFO CONSOLE::COLORS::BLUE << "[INFO] " << CONSOLE::COLORS::RESET
#define PUSH_DBG CONSOLE::COLORS::BRIGHT_GREEN << "[DBG] " << CONSOLE::COLORS::RESET


void enableVirtualTerminal();



// Default: from green(0x00FF00) to blue(0x0000FF)
void test_gradient(uint32_t color_from = 0x00FF00, uint32_t color_to = 0x0000FF, size_t count = 8);
std::string color_to_escape_code(uint32_t color);
std::vector<std::string> get_gradient(uint32_t start_colour, uint32_t end_colour, size_t n);
void show_gradient(std::vector<std::string> colors);

std::string get_gradien_colorized(std::string str, uint32_t color_from=0x00FF0000, uint32_t color_to=0x0000FF00, size_t n=0);

template<uint32_t color_from=CONSOLE::CYAN, uint32_t color_to=CONSOLE::GREEN>
void prog_greet(const std::string& prog_name) {
    CONSOLE::enableVirtualTerminal();
    #ifdef _DEBUG
        std::cout << std::endl << get_gradien_colorized("### DEBUG MODE ###", CONSOLE::CYAN,  CONSOLE::BLUE) << std::endl;
    #endif
    std::cout << std::endl << "<< " << get_gradien_colorized(prog_name, color_from, color_to) << " >>\n" << std::endl;
}

template<uint32_t color_from=CONSOLE::YELLOW, uint32_t color_to=CONSOLE::RED>
void prog_bye(const std::string& phrase) {
    std::cout << std::endl << "<< " << get_gradien_colorized(phrase, color_from, color_to) << " >>\n" << std::endl;
}

}  // namespace CONSOLE



namespace COMMON{

size_t getAvailableMemory();

// In developing
template<bool WaitForCompletion = false>
std::string __runExternalProgram(const STD_PATH& exec_path, const std::vector<std::string>& args) {
    std::string command;
    {
        OSS oss;
        oss << exec_path.string();
        for (const auto& arg : args) {
            oss << " " << std::quoted(arg); // Екрануємо аргумент для командного рядка
        }
        command = oss.str();
    }
    std::cout << "[DBG] command:\n";
    std::cout << command;
    std::cout << std::endl;

#if defined(_WIN32) || defined(_WIN64) 
    if constexpr (!WaitForCompletion) {
        command = "start " + command;
        system(command.c_str());
        return "[INFO] Program launched in a separate terminal (Windows).";
    } else {
        STARTUPINFO si = { sizeof(si) };
        PROCESS_INFORMATION pi = {};
        std::vector<char> command_cstr(command.begin(), command.end());
        command_cstr.push_back('\0');

        if (!CreateProcess(NULL, command_cstr.data(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
            OSS oss;
            oss << "[ERROR] Failed to start the process: " << GetLastError();
            return oss.str();
        }

        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        return "[INFO] Program executed and completed (Windows).";
    }

#else
    if constexpr (!WaitForCompletion) {
        command = "echo 'Testing terminal'; sleep 5";
        std::string command_line = "gnome-terminal -- bash -c \"" + command + "\" &";
        system(command_line.c_str());
        return "[INFO] Program launched in a separate terminal (Linux).";
    } else {
        // Випадок, коли чекаємо завершення
        pid_t pid = fork();
        if (pid == 0) {
            // Дочірній процес
            std::vector<char*> argv;
            argv.push_back(const_cast<char*>(exec_path.string().c_str()));
            for (const auto& arg : args) {
                argv.push_back(const_cast<char*>(arg.c_str()));
            }
            argv.push_back(nullptr);
            execvp(argv[0], argv.data());

            // Якщо execvp завершився помилкою
            std::cerr << "[ERROR] Failed to execute command\n";
            std::exit(EXIT_FAILURE);
        } else if (pid > 0) {
            // Батьківський процес чекає на завершення дочірнього процесу
            int status;
            waitpid(pid, &status, 0);
            return "[INFO] Program executed and completed (Linux).";
        } else {
            return "[ERROR] Fork failed";
        }
    }
#endif
}


template <typename T>
std::vector<T> get_random_subset(std::vector<T>& input_vector, ssize_t sz=-1) {
    if (input_vector.empty()) {
        return {};
    }
    std::random_device rd;
    std::mt19937 generator(rd());
    size_t subset_size = sz;
    if( sz < 0) {
        std::uniform_int_distribution<size_t> distribution(0, input_vector.size());
        subset_size = distribution(generator);
    }
    std::vector<T> result;
    std::sample(input_vector.begin(), input_vector.end(), std::back_inserter(result), subset_size, generator);

    return result;
}

int parse_duration(const std::string& duration_str);
std::string format_duration(int total_seconds);
/**
 * @brief Retrieves a JSON sub-section based on command-line arguments.
 *
 * This function processes the command-line arguments to determine the source of the JSON configuration.
 * It can load the configuration directly from an argument or from a specified configuration file.
 * If additional arguments are provided after the configuration file, they are treated as a hierarchical
 * path to nested sections within the JSON structure. The function navigates through the JSON hierarchy
 * according to these arguments and returns the corresponding JSON string.
 *
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * @param verbose If set to true, the function outputs status and error messages to the console.
 *
 * @return A JSON string representing the specified sub-section of the configuration.
 *         Returns an empty string if an error occurs during processing.
 */
std::string get_json_sub_section(int argc, char* argv[], bool verbose);

std::string get_time_stamp(const char* format="%H_%M_%S");

/// @brief Convert double number representation to string with scalefactor
/// @param value The frequency value to convert
/// @param scale Scale factor: 1 - default Hz, 1e3 - KHz, 1e6 - MHz
/// @return String representation of the value with M|K|_ suffix
std::string val_to_string(double value, double scale);

/// @brief Convert double freq representation to string
/// @param freq The frequency value to convert
/// @param scale Scale factor: 1 - default Hz, 1e3 - KHz, 1e6 - MHz
/// @return String representation of the frequency
std::string freq_to_string(double freq, double scale = 1e3);

#if 0
template <class T>
std::optional<T> initialize_from_json(const std::string& json_str, error_handler_t onError=nullptr) {
    try 
    {
        nlohmann::json j = nlohmann::json::parse(json_str);
        return j.get<T>();
    } 
    catch (const nlohmann::json::exception& e) 
    {
        OSS oss;
        oss << PUSH_ERR << PUSH_HERE;
        oss << "JSON Exception while initializing: '" << e.what() << "'";
        if (onError) onError(oss.str());
        return std::nullopt;
    }
    catch (const std::exception& e) 
    {
        OSS oss;
        oss << PUSH_HERE;
        oss << "Exception while initialize: '" << e.what() << "'";
        if (onError) onError(oss.str());
        return std::nullopt;
    }
}

template <class T>
std::optional<T> initialize_from_json_file(const STD_PATH& file_path, const std::string& sub_key, error_handler_t onError=nullptr) 
{
    try 
    {
        std::ifstream inp_f_stream(file_path);
        if (!inp_f_stream.is_open()) {
            OSS oss;
            oss << PUSH_HERE;
            oss << "Fail to open file '" << file_path << "'";
            if (onError) onError(oss.str());
            return std::nullopt;
        }

        nlohmann::json j;
        inp_f_stream >> j;
        return j.at(sub_key).get<T>();

    } 
    catch (const nlohmann::json::exception& e) 
    {
        OSS oss;
        oss << PUSH_HERE;
        oss << "JSON Exception while initializing: '" << e.what() << "'";
        if (onError) onError(oss.str());
        return std::nullopt;
    }
    catch (const std::exception& e) 
    {
        OSS oss;
        oss << PUSH_HERE;
        oss << "Exception while initialize: '" << e.what() << "'";
        if (onError) onError(oss.str());
        return std::nullopt;
    }
}

#endif

#if defined(_WIN32) || defined(_WIN64)
namespace WIN_SPEC{
    void PrintLastErrorDescription(FILE* pf_out, uint32_t err);
}
#endif

std::string format_with_underscores(int64_t num);
void PrintStartupInfo(int argc, char *argv[]);
namespace CRC {

typedef struct _CRC32_context {
    uint32_t crc32;
    uint32_t crc32_table[256];
} _CRC32_, *P_CRC;

void init_crc_calk(P_CRC p, uint32_t salt);
void update_crc_calk(P_CRC p, uint8_t* data, size_t count);
#define GET_CRC(pCRC) ((pCRC)->crc32 ^ 0xFFFFFFFF)

}  // namespace CRC

}  // namespace COMMON

#endif // #ifndef __D7_AUX_H__
