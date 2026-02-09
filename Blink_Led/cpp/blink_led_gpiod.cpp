#include <gpiod.hpp>
#include <iostream>
#include <chrono>
#include <thread>

int main() {
    // In v2, we use the path to the device
    std::string chip_path = "/dev/gpiochip0"; 
    unsigned int offset = 23; 

    try {
        gpiod::chip  chip = gpiod::chip(chip_path);
        gpiod::line_request request = chip.prepare_request()
            .set_consumer("BlinkApp")
            .add_line_settings(
                offset,
                gpiod::line_settings()
                    .set_direction(gpiod::line::direction::OUTPUT)
            )
            .do_request();

        std::cout << "Blinking LED on offset " << offset << "..." << std::endl;

        while (true) {
            // Toggle logic in v2
            auto current_val = request.get_value(offset);
            auto new_val = (current_val == gpiod::line::value::ACTIVE) ? 
                            gpiod::line::value::INACTIVE : gpiod::line::value::ACTIVE;
            
            request.set_value(offset, new_val);
            
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}