//  g++ -o youtube_downloader youtube_downloader.cpp



#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string>

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
    try {
        while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
            // Печать прогресса или другой информации
            std::cout << buffer << std::flush;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);

    return 0;
}

