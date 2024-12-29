#include <fftw3.h>
#include <rtl-sdr.h>
#include <iostream>

void test_fftw3() {
    int N = 16;
    fftw_complex* in = fftw_alloc_complex(N);
    fftw_complex* out = fftw_alloc_complex(N);
    fftw_plan plan = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

 
    for (int i = 0; i < N; ++i) {
        in[i][0] = i;  
        in[i][1] = 0;  
    }

    fftw_execute(plan); 


    for (int i = 0; i < N; ++i) {
        std::cout << "out[" << i << "] = "
                  << out[i][0] << " + " << out[i][1] << "i" << std::endl;
    }

    fftw_destroy_plan(plan);
    fftw_free(in);
    fftw_free(out);
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
