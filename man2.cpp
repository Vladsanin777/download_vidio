#include <iostream>
#include <array>
#include <memory>
#include <stdexcept>
#include <string>
#include <cstdio> // для popen и pclose

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    FILE* pipe = popen(cmd, "r");
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    try {
        while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
            result += buffer.data();
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}

int main() {
	std::cout<<"Input url for Youtube video: ";
    std::string url;
	std::cin>>url;
    std::string command = "yt-dlp --get-title --get-thumbnail --skip-download " + url;

    try {
        std::string output = exec(command.c_str());
        std::cout << "Output: " << std::endl << output << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}

