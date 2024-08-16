#include <iostream>
#include <string>
#include <cstdlib>

int main() {
    std::string command = "zenity --file-selection --directory 2>/dev/null";
    char buffer[1024];
    
    // Открытие диалогового окна и получение выбранного пути
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        std::cerr << "Не удалось открыть диалоговое окно!" << std::endl;
        return 1;
    }
    
    // Чтение пути, который вернул Zenity
    if (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        std::string selected_path(buffer);
        
        // Удаляем символ новой строки, если он есть
        if (!selected_path.empty() && selected_path.back() == '\n') {
            selected_path.pop_back();
        }
        
        std::cout << "Вы выбрали папку: " << selected_path << std::endl;
    }
    
    pclose(pipe);
    return 0;
}

