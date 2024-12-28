#include "d7.hpp"


namespace CONSOLE {

void enableVirtualTerminal() {
#if defined(_WIN32) || defined(_WIN64)
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) {
        return;
    }

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) {
        return;
    }

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
#endif
}

std::string color_to_escape_code(uint32_t color) {
    uint8_t r = (color >> 16) & 0xFF;
    uint8_t g = (color >> 8) & 0xFF;
    uint8_t b = color & 0xFF;

    std::ostringstream oss;
    oss << "\x1b[38;2;" << std::to_string(r) << ";" 
        << std::to_string(g) << ";" << std::to_string(b) << "m";
    return oss.str();
}

std::vector<std::string> get_gradient(uint32_t start_colour, uint32_t end_colour, size_t n) {
    std::vector<std::string> gradient;

    for (size_t i = 0; i < n; ++i) {
        float t = static_cast<float>(i) / (n - 1);
        uint8_t r = static_cast<uint8_t>((1 - t) * ((start_colour >> 16) & 0xFF) + t * ((end_colour >> 16) & 0xFF));
        uint8_t g = static_cast<uint8_t>((1 - t) * ((start_colour >> 8) & 0xFF) + t * ((end_colour >> 8) & 0xFF));
        uint8_t b = static_cast<uint8_t>((1 - t) * (start_colour & 0xFF) + t * (end_colour & 0xFF));

        uint32_t color = (r << 16) | (g << 8) | b;
        gradient.push_back(color_to_escape_code(color));
    }

    return gradient;
}


void test_gradient(uint32_t color_from, uint32_t color_to, size_t count) {
    std::cout << PUSH_MAGENTA("\n<< TEST GRADIENT >>\n") << std::endl;
    std::vector<std::string> gradient = get_gradient(color_from, color_to, count);

    for (size_t i = 0; i < gradient.size(); ++i) {
        std::cout << gradient[i] << "Color " << i + 1 << COLORS::RESET << std::endl;
    }

    std::cout << PUSH_MAGENTA("\n<< DONE >>\n") << std::endl;
    exit(0);
}
void show_gradient(std::vector<std::string> colors_gradient) {
        for (size_t i = 0; i < colors_gradient.size(); ++i) {
        std::cout << colors_gradient[i] << "Color " << i + 1 << COLORS::RESET << std::endl;
    }
}

std::string get_gradien_colorized(std::string str, uint32_t color_from, uint32_t color_to, size_t n) 
{
    static std::vector<std::string> colors;
    static uint32_t last_color_from=0, last_color_to=0;
    size_t ln = str.length();
    if ( n ) {
        ln = n;
    }

    if ( colors.size()!=ln || color_from != last_color_from || last_color_to!= color_to)
    {
        colors = get_gradient(color_from, color_to, ln);
        last_color_from = color_from;
        last_color_to = color_to;
    }

    OSS oss;
    for (size_t i = 0; i < ln && i < str.length(); ++i)
    {
        oss << colors[i] << str[i];
    }
    oss << CONSOLE::COLORS::RESET;
    return oss.str();
}

}  // namespace CONSOLE

std::ostream& operator<<(std::ostream& os, const PRINTABLE& obj) {
    os << obj.to_string();
    return os;
}

std::ostream& operator<<(std::ostream& os, const duration_t& duration) {
    double seconds = SEC_FROM_DUR(duration);
    double integer_part;
    double fractional_part = std::modf(std::fabs(seconds), &integer_part);
    std::streamsize prec = 1;
    if (fractional_part < 0.001) {
        prec = 4;
    } else if (fractional_part < 0.01) {
        prec = 2;
    } else if (fractional_part < 0.1) {
        prec = 2;
    }
    os << CONSOLE::COLORS::CYAN << std::fixed << std::setprecision(prec) << seconds  << CONSOLE::COLORS::RESET << " sec";
    os.unsetf(std::ios::fixed);
    
    return os;
}

namespace COMMON{

size_t getAvailableMemory() {
#if defined(_WIN32) || defined(_WIN64)
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    return (size_t)memInfo.ullAvailPhys;
#elif __linux__
    struct sysinfo memInfo;
    sysinfo(&memInfo);
    return (size_t)(memInfo.freeram * memInfo.mem_unit);
#else
    #error "Unsupported platform"
#endif
}

std::string format_duration(int total_seconds) {
    int hours = total_seconds / 3600;
    int minutes = (total_seconds % 3600) / 60;
    int seconds = total_seconds % 60;
    std::string result;
    if (hours > 0) result += std::to_string(hours) + "h";
    if (minutes > 0) result += std::to_string(minutes) + "m";
    if (seconds > 0) result += std::to_string(seconds) + "s";
    return result.empty() ? "0s" : result;
}

int parse_duration(const std::string& duration_str) {
    std::regex pattern(R"(^(\d+h)?(\d+m)?(\d+s)?$)");
    std::smatch match;
    int total_seconds = 0;
    if (std::regex_match(duration_str, match, pattern)) 
    {
        if (match[1].matched) total_seconds += std::stoi(match[1].str()) * 3600;
        if (match[2].matched) total_seconds += std::stoi(match[2].str()) * 60;
        if (match[3].matched) total_seconds += std::stoi(match[3].str());
    }
    return total_seconds;
}

#if defined(_WIN32) || defined(_WIN64)
    namespace WIN_SPEC{
        void PrintLastErrorDescription(FILE* pf_out, uint32_t err) {
        char* errMsg=NULL;

        DWORD formatResult = FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            err,
            MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), // Використовуємо англійську мову
            (LPSTR)&errMsg,
            0,
            NULL
        );

        if (formatResult) {
            // Print the error message
            fprintf(pf_out, "Error %u: %s", err, errMsg);
            // Free the buffer allocated by FormatMessage
            LocalFree(errMsg);
        }
        else {
            fprintf(pf_out, "Error %u: Unable to retrieve error message.\n", err);
        }
    }

}
#endif  // #if defined(_WIN32) || defined(_WIN64)

#if 0
std::string get_json_sub_section(int argc, char* argv[], bool verbose)
{
    std::string config_json_str = "";
    STD_PATH cfg_file_path;
    if ( argc == 0 ) {
        std::cout << "> Command line arguments not set at all" << std::endl;
        return "";
    }
    if (verbose) COMMON::PrintStartupInfo(argc, argv);
    if (verbose) std::cout << "> Get " << PUSH_YELLOW("config_json_str");

    std::string arg0=argv[0];
    
    if (arg0.size() >= 5 && arg0.substr(arg0.size() - 5) == ".json") {
        cfg_file_path = STD_PATH(arg0);
        if (!std::filesystem::exists(cfg_file_path))
        {
            if (verbose){
                OSS oss;
                oss << PUSH_ERR << PUSH_HERE;
                oss << " file:" << cfg_file_path <<  " not exists";
                std::cout << oss.str() << std::endl;
            }
            return "";
        }
        std::ifstream cfg_file(cfg_file_path);
        if (!cfg_file)
        {
            if (verbose){
                OSS oss;
                 oss << PUSH_ERR << PUSH_HERE;
                oss << "failed to open config file: " << cfg_file_path;
                std::cout << oss.str() << std::endl;
            }
            return "";
        }
        if (verbose) std::cout << " from file " << PUSH_YELLOW(cfg_file_path.string());
        std::stringstream buffer;
        buffer << cfg_file.rdbuf();
        config_json_str = buffer.str();
    }
    else {
        config_json_str = argv[0];
        if (verbose) std::cout << " explicitly (from  argv[0])";
    }
    if (verbose) std::cout << std::endl;
    nlohmann::json j;
    try {
        j = nlohmann::json::parse(config_json_str);
    } catch (const nlohmann::json::exception& e) {
        if (verbose) {
            OSS oss;
             oss << PUSH_ERR << PUSH_HERE;
            oss << "Parsing JSON: " << e.what();
            std::cout << oss.str() << std::endl;
        }
        return "";
    }

    if ( argc > 1) 
    {
        if (verbose) std::cout << "  subkeys:";
        nlohmann::json current = j;  // Парсений JSON-об'єкт
        for (int i = 1; i < argc; ++i) {  // Починаємо з argv[1]
            std::string section = argv[i];
            if (verbose) std::cout << "\n  - " << PUSH_YELLOW(section);
            if (current.contains(section)) {
                current = current[section];  // Переходимо в підсекцію
            } else {
                if (verbose) {
                    OSS oss;
                    oss << PUSH_ERR << PUSH_HERE;
                    oss << "Section \"" << section << "\" not found in the configuration file.";
                    std::cout << oss.str() << std::endl;
                }
                return "";  // Якщо секція не знайдена, повертаємо порожній рядок
            }
        }
        if (verbose) std::cout << std::endl;
        config_json_str = current.dump();
    }
    
    if (verbose) std::cout << "  config_json_str:\n" << PUSH_GRAY(config_json_str) << std::endl;

    return config_json_str;
}
#endif

std::string get_time_stamp(const char* format) {
    auto now = std::chrono::system_clock::now();
    std::time_t time_now = std::chrono::system_clock::to_time_t(now);
    std::tm* tm_now = std::localtime(&time_now);

    std::ostringstream oss;
    // "%Y-%m-%d %H:%M:%S"
    oss << std::put_time(tm_now, format);
    
    return oss.str();
}

std::string format_with_underscores(int64_t num) {
    OSS oss;
    std::string result;
    oss.imbue(std::locale("en_US.UTF-8"));
    oss << std::fixed << num;
    result = oss.str();
    for (auto& ch : result) {
        if (ch == ',') {
            ch = '_';
        }
    }
    return result;
}


/// @brief Convert double freq representation to string
/// @param freq The frequency value to convert
/// @param scale Scale factor: 1 - default Hz, 1e3 - KHz, 1e6 - MHz
/// @return String representation of the frequency
std::string freq_to_string(double freq, double scale)
{
    std::ostringstream oss;
    oss << CONSOLE::COLORS::YELLOW;
    oss << std::fixed << std::setprecision(2) << (freq / scale);
    
    if (scale == 1e3) {
        oss << " KHz";
    } else if (scale == 1e6) {
        oss << " MHz";
    } else {
        oss << " Hz";
    }
    oss << CONSOLE::COLORS::RESET;
    return oss.str();
}

std::string val_to_string(double value, double scale)
{
    std::ostringstream oss;

    double ratio = value / scale;

    if (scale == 1e3) {
        oss << std::fixed << std::setprecision(2) << (ratio);
        oss << " K";
    } else if (scale == 1e6) {
        oss << std::fixed << std::setprecision(1) << (ratio);
        oss << " M";
    } else {
        oss << (ratio) << " ";
    }

    return oss.str();
}

void PrintStartupInfo(int argc, char *argv[]) {
    char path[MAX_PATH];
#if defined(_WIN32) || defined(_WIN64)
    _getcwd(path, (int)sizeof(path));
#else
    getcwd(path, sizeof(path));
#endif
    printf("\n");
    printf("[CWD: %s]\n",path);
    SHOW_COMMAND_LINE();
    printf("-----------------------------------------------------------------------------------------------------\n");
}

namespace CRC {

void init_crc_calk(P_CRC p, uint32_t salt) {
    p->crc32 = salt;
    uint32_t polynomial = 0xEDB88320;
    for (uint32_t i = 0; i < 256; i++) {
        uint32_t c = i;
        for (int j = 0; j < 8; j++) {
            if (c & 1) {
                c = polynomial ^ (c >> 1);
            } else {
                c >>= 1;
            }
        }
        p->crc32_table[i] = c;
    }
}

void update_crc_calk(P_CRC p, uint8_t* bytes, size_t count) {
    // Process each byte in the input data
    for (size_t i = 0; i < count; i++) {
        p->crc32 = p->crc32_table[(p->crc32 ^ bytes[i]) & 0xFF] ^ (p->crc32 >> 8);
    }
}

}  // namespace CRC

}  // namespace COMMON