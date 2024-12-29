
#include "../_aux/d7.hpp"

#include <fftw3.h>
#include <rtl-sdr.h>
#include <iostream>


void test_fftw3(int argc, char *argv[])
{
    
    int times = int(1e6);
    int N = 1024;
    bool verbose = true;
    if ( argc >= 2)
    {        
        times = std::stoi(argv[1]);
        if ( argc >= 3)
        {
            N = std::stoi(argv[2]);
            if (argc == 4) 
                verbose = false;
        }

    } 
    if (verbose) std::cout << PUSH_GREEN("<< FFTW3 some test >>") << std::endl;
    if ( argc == 1){
        if (verbose) std::cout << "Usage: " << argv[0] << " [times] [N] [silent]" << "  default is int(1e6) 1024" << std::endl;
    }
    if (verbose) 
        std::cout << "Test FFTW3 with " << PUSH_GREEN(times) << " iterations and " << PUSH_GREEN(N) << " points." << std::endl;
    else
        std::cout << "Iterations " << PUSH_GREEN(times) << " FFTW3 points " << PUSH_GREEN(N) << ". " << std::flush;
    tick_t start, end;
    duration_t dur, dur_avg;

    fftw_complex* in;
    fftw_complex* out;
    fftw_complex* chunk;
    fftw_plan plan;
    uint64_t crc_out, crc_chunk;

    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    chunk = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);

    plan = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

    auto CRC = [&N](fftw_complex* p) {
        uint64_t crc=0;
        for (int i = 0; i < N; ++i) 
        {
            crc ^= static_cast<uint64_t>(p[i][0]);
            crc ^= static_cast<uint64_t>(p[i][1]);
        }
        return crc; 
    };
    auto init_rand = [&N](fftw_complex* ptr) {
        for (int i = 0; i < N; ++i) {
            ptr[i][0] = rand();
            ptr[i][1] = rand();
        }
    };

    if (verbose)  std::cout << "> Check using existing plan and copy data " << PUSH_GREEN(times) <<  std::endl;
    crc_chunk = crc_out = 0;
    
    start = steady_clock_t::now();
    for ( int i = 0; i < times; ++i) {        
        init_rand(chunk);
        crc_chunk ^= CRC(chunk);
        memccpy(in, chunk, sizeof(fftw_complex), N);
        fftw_execute(plan); 

        crc_out ^= CRC(out);
    }   
    end = steady_clock_t::now();
    fftw_destroy_plan(plan);
    dur = end - start;
    dur_avg = dur / times;
    if (verbose) std::cout << "  Duration full: " << PUSH_CYAN(SEC_FROM_DUR(dur)) << " sec."
              << "  Duration avg: " << PUSH_CYAN(SEC_FROM_DUR(dur_avg)) << " sec."
              << "  CRC: " << PUSH_GREEN(crc_chunk) << ':' << PUSH_GREEN(crc_out) << '.' 
              << std::endl;
    else
        std::cout << " DUR (sec): " << PUSH_CYAN(SEC_FROM_DUR(dur));

    if (verbose) std::cout << "> Check create/destroy plan " << PUSH_GREEN(times) <<  std::endl;
    start = steady_clock_t::now();
    for ( int i = 0; i < times; ++i) {        
        init_rand(chunk);
        crc_chunk ^= CRC(chunk);

        plan = fftw_plan_dft_1d(N, chunk, out, FFTW_FORWARD, FFTW_ESTIMATE);
        fftw_execute(plan); 

        crc_out ^= CRC(out);

        fftw_destroy_plan(plan);
    }   
    end = steady_clock_t::now();
    dur = end - start;
    dur_avg = dur / times;
    if (verbose) std::cout << "  Duration full: " << PUSH_CYAN(SEC_FROM_DUR(dur)) << " sec. "
              << "  Duration avg: " << PUSH_CYAN(SEC_FROM_DUR(dur_avg)) << " sec. "
              << "CRC: " << PUSH_GREEN(crc_chunk) << ':' << PUSH_GREEN(crc_out) << '.' 
              << std::endl;
    else
        std::cout << " | " << PUSH_CYAN(SEC_FROM_DUR(dur)) << '.'  << std::endl;

    fftw_free(chunk);
    fftw_free(in);
    fftw_free(out);
    std::exit(0);
}


void test_rtl_device() {
    int device_count = rtlsdr_get_device_count();
    if (device_count == 0) {
        std::cerr << "No RTL-SDR devices found!" << std::endl;
        return;
    }

    char serial[256];
    for (int i = 0; i < device_count; ++i) {
        rtlsdr_get_device_usb_strings(i, nullptr, nullptr, serial);
        if (std::string(serial) == "000000D7") {
            rtlsdr_dev_t* dev;
            if (rtlsdr_open(&dev, i) != 0) {
                std::cerr << "Failed to open device." << std::endl;
                return;
            }
            std::cout << "Device 000000D7 successfully opened." << std::endl;
            rtlsdr_close(dev);
            return;
        }
    }

    std::cerr << "Device with serial 000000D7 not found." << std::endl;
}
