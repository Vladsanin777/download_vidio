//  g++ -o youtube_downloader youtube_downloader.cpp



#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <regex>

// Функция для выполнения команды в командной строке и возврата результата
std::string exec(const char* cmd) {
    char buffer[128];
    std::string result = "";
    FILE* pipe = popen(cmd, "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
            result += buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}

int main() {
    std::string videoUrl;
    std::cout << "Введите ссылку на видео с YouTube: ";
    std::getline(std::cin, videoUrl);

    std::string option;
    std::cout << "Введите опцию (-v для видео, -a для аудио): ";
    std::getline(std::cin, option);

    std::string command;
    if (option == "-v") {
        // Скачивание видео
        command = "yt-dlp -f best -o 'video.%(ext)s' --newline --verbose \"" + videoUrl + "\"";
    } else if (option == "-a") {
        // Скачивание аудио
        command = "yt-dlp -x --audio-format mp3 -o 'audio.%(ext)s' --newline --verbose \"" + videoUrl + "\"";
    } else {
        std::cerr << "Неверная опция. Используйте -v или -a." << std::endl;
        return 1;
    }

    std::cout << "Выполняю загрузку..." << std::endl;

    // Выполнение команды и отображение прогресса
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        std::cerr << "Ошибка выполнения команды." << std::endl;
        return 1;
    }

    char buffer[128];
    std::regex regexPattern(R"(\[download\] +(\d+\.\d+)% of ~?(\d+\.\d+MiB) at +([\d\.]+[KMG]?iB/s) ETA (\d+:\d+))");
    std::smatch match;
    try {
        while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
            std::string line(buffer);

            if (std::regex_search(line, match, regexPattern)) {
                std::string percent = match[1].str();
                std::string size = match[2].str();
                std::string speed = match[3].str();
                std::string eta = match[4].str();

                std::cout << "Процент: " << percent << "%, "
                          << "Размер: " << size << ", "
                          << "Скорость: " << speed << ", "
                          << "Оставшееся время: " << eta << std::endl;
            }
	}
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);

    return 0;
}

