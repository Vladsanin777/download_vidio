//  g++ -g -o d main.cpp `pkg-config --cflags --libs gtk4` -lcurl
//  yay -S ffmpeg
//  yay -S youtube-dl



#include <gtk/gtk.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>
#include <vector>
#include <ctime>
#include <regex>

// Окно
GtkWidget *window;
// Основной grid
GtkWidget *main_grid;
// Поля с пользовательским url
GtkWidget *entry_url;
// Боковая панель
GtkWidget *scrolled_window_side_panel;
// Боковая панель grid
GtkWidget *grid_side_panel;
// Первый элемент боковой панели
GtkWidget *label_item_emptry;
// Средняя панель
GtkWidget *middle_panel_grid;
// Выбор вывода ошибки
GtkWidget *error_button;
// Включение и выключение боковой панели
GtkWidget *side_panel_button;
// Открытали панель с выводом информации о скачивании
bool download_info = false;
// Сколько видио скачивается в данный момент
int download_vidio = 0;
// Выбрано ли видио
bool vidio = false;
// Выбрано ли аудио
bool audio = false;

int division_into_two_parts_text_and_textual(std::string input){
    // Разделение строки на числовую и текстовую части
    std::string numberPart;
    std::string textPart;
    for (size_t i = 0; i < input.size(); ++i) {
        if (isdigit(input[i]) || input[i] == '.') {
            numberPart += input[i];
        } else {
            textPart = input.substr(i);
            break;
        }
	}
    float number;
    // Преобразование числовой части в float
    std::istringstream(numberPart) >> number;
    return 1;
}


class Base {
public:
    static void apply_css(const char* css) {
        GtkCssProvider* provider = gtk_css_provider_new();
        gtk_css_provider_load_from_string(provider, css);
        gtk_style_context_add_provider_for_display(gtk_widget_get_display(window), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
        g_object_unref(provider);
    }
};

class VidioDownload{
public:
    // Функция для выполнения команды в командной строке и возврата результата
    void command_cast(std::string video_url, std::string format) {
        std::string command;
	    std::cout<<format<<std::endl;
        // Создание команды для загрузки видео с YouTube
        if (format == "vidio"){command = "yt-dlp -f bestvideo+bestaudio --merge-output-format mp4 -o \"%(title)s.%(ext)s\" --newline --verbose \"" + video_url + "\"";}
        else if (format == "audio"){command = "yt-dlp -x --audio-format mp3 -o 'audio.%(ext)s' --newline --verbose \"" + video_url + "\"";}
        
	    std::cout<<command<<std::endl;
        // Запуск команды для загрузки файла
        FILE* pipe = popen(command.c_str(), "r");
        if (!pipe) {
            std::cerr << "Ошибка выполнения команды." << std::endl;
            return;
        }

        // Переменные для получения информации о загрузке аудио или видео
        char buffer[256];
        std::regex regexPattern(R"(\[download\]\s+(\d+\.\d+)% of\s+(\d+\.\d+KiB) at\s+([\d\.]+[KMG]iB/s) ETA (\d+:\d+))");
	    std::smatch match;
        download_vidio++;
        short dv_num = download_vidio;
        // Цикл для обновления информации о загрузке аудио или видео
        while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
	        std::cout<<"edsd"<<std::endl;
            std::string line(buffer);
	        std::cout<<buffer<<std::endl;


            if (std::regex_search(line, match, regexPattern)) {
                float percent = std::stof(match[1].str());
                std::string size = match[2].str();
                std::string speed = match[3].str();
                std::string eta = match[4].str();

                std::cout << "Процент: " << percent << "%, "
                          << "Размер: " << size << ", "
                          << "Скорость: " << speed << ", "
                          << "Оставшееся время: " << eta << std::endl;
            }
        }
        // Закрытие команды
        pclose(pipe);
    }
};


class View_DL : public Base{
public:
    static void include_side_panel(GtkWidget *widget = NULL, gpointer data = NULL){
        download_info = !download_info;
        int width = gtk_widget_get_width(GTK_WIDGET(window));
        int height = gtk_widget_get_height(GTK_WIDGET(window));

        if (download_info){
            // Увеличиваем ширину окна на ширину панели
            gtk_window_set_default_size(GTK_WINDOW(window), width + 250, height);

            gtk_widget_set_visible(scrolled_window_side_panel, TRUE);
        }else{
            gtk_widget_set_visible(scrolled_window_side_panel, FALSE);
            
            // Уменьшаем ширину окна на ширину панели
            gtk_window_set_default_size(GTK_WINDOW(window), width - 250, height);
        }
    }
    void include_emptry_side_panel(){
        gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window_side_panel), label_item_emptry);                
    }
    static void clicked_error_button(GtkWidget *widget, gpointer data){}
    void creation_side_panel(){
        scrolled_window_side_panel = gtk_scrolled_window_new();
        gtk_grid_attach(GTK_GRID(main_grid), scrolled_window_side_panel, 7, 0, 1, 3);
        gtk_widget_add_css_class(scrolled_window_side_panel, "scrolled_window_side_panel");
        apply_css(".scrolled_window_side_panel{ margin-left: 10px; background-color: rgba(0,0,0,0.1);}");
        gtk_widget_set_vexpand(scrolled_window_side_panel, TRUE);
        gtk_widget_set_size_request(scrolled_window_side_panel, 250, 0);
        gtk_widget_set_visible(scrolled_window_side_panel, FALSE);
    }
    void emptry_side_panel(){   
        label_item_emptry = gtk_editable_label_new("Вы ничего не скачиваете");
        gtk_widget_add_css_class(label_item_emptry, "label_item_emptry");
        //apply_css(".label_item_first{min-height: 100%; min-width: 100%;}");
    }
    void include_and_creation_middle_panel(){
        middle_panel_grid = gtk_grid_new();
        gtk_grid_attach(GTK_GRID(main_grid), middle_panel_grid, 6, 0, 1, 3);
        gtk_widget_add_css_class(middle_panel_grid, "middle_panel_grid");
        apply_css(".middle_panel_grid{ margin-left: 10px;}");
        error_button = gtk_button_new_with_label("Ошибка");
        gtk_grid_attach(GTK_GRID(middle_panel_grid), error_button, 0, 0, 1, 1);
        gtk_widget_set_vexpand(error_button, TRUE);
        g_signal_connect(error_button, "clicked", G_CALLBACK(clicked_error_button), NULL);
        side_panel_button = gtk_button_new_with_label("Боковая\nпанель");
        gtk_widget_set_vexpand(side_panel_button, TRUE);
        gtk_grid_attach(GTK_GRID(middle_panel_grid), side_panel_button, 0, 1, 1, 1);
        g_signal_connect(side_panel_button, "clicked", G_CALLBACK(include_side_panel), NULL);

    }
};

// class обработки ошибок пользователя
class Error : public Base{
private:
    static void (*error_object_red)();
    static short state;    
    static gboolean reset_margin(gpointer) {
        apply_css(" .grid_1 { margin: 0px 0px 0px 0px; transition: margin 0.05s ease; } ");
        return FALSE;
    }

    static gboolean set_margin(gpointer) {
        apply_css(" .grid_1 { margin: 0px 0px 0px 20px; transition: margin 0.05s ease; } ");
        g_timeout_add(50, reset_margin, nullptr);
        return FALSE;
    }

    static gboolean error_entry_trembling_step(gpointer data) {
        int* counter = static_cast<int*>(data);
        apply_css(" .grid_1 { margin: 0px 20px 0px 0px; transition: margin 0.05s ease; } ");
        g_timeout_add(100, set_margin, nullptr);
        (*counter)++;
        if (*counter < 10) {
            g_timeout_add(200, error_entry_trembling_step, data);
        } else {
            delete counter;
        }
        return FALSE;
    }

    static void error_entry_trembling() {
        int* counter = new int(0);
        error_entry_trembling_step(counter);
    }

    static void error_titlebar_red() {
        apply_css(state % 2 == 0 ? " .titlebar { background-color: red; transition: background-color 0.6s ease; } " : " .titlebar { background-color: rgba(0,0,0,0.3); transition: background-color 2.5s ease; } ");
    }

    static void error_entry_red() {
        apply_css(state % 2 == 0 ? "entry { background-color: red; transition: background-color 0.6s ease; }" : "entry { background-color: rgba(0,0,0,0.1); transition: background-color 2.5s ease; }");
    }

    static void error_button_red() {
        apply_css(state % 2 == 0 ? ".button_choice { background: red; transition: background 0.6s ease;}" : ".button_choice { background: rgba(0,0,0,0.3); transition: background 2.5s ease; } .button_choice:hover {background: rgba(0,0,0,0.2);}");
    }

    static gboolean error_start() {
        error_titlebar_red();
        error_object_red();
        state++;
        if (state == 2) {
            state = 0;
            return FALSE;
        } else {
            return TRUE;
        }
    }
    
    static void init(){};
    static void transition_none_button_choice(){apply_css(".button_choice { transition: none; }");}
    static void return_tittle(){gtk_window_set_title(GTK_WINDOW(window), "Скачка видио с youtube!");}
public:
    static void error_main(const char* object, const char* string_title) {
	    state = 0;
	    gchar *title = NULL;
	    gtk_window_set_title(GTK_WINDOW(window), string_title);
	    if (strcmp(object, "entry") == 0) {
	        error_object_red = error_entry_red;
            error_start();
            error_entry_trembling();
            g_timeout_add(1000, (GSourceFunc)error_start, NULL);
	    }else if(strcmp(object, "button") == 0){
	        error_object_red = error_button_red;
            error_start();
	        g_timeout_add(1000, (GSourceFunc)error_start, NULL);
	        g_timeout_add(3000, (GSourceFunc)transition_none_button_choice, NULL);
	    }
	    g_timeout_add(3000, (GSourceFunc)return_tittle, NULL);
    }
};

// Определение статической переменной
short Error::state = 0;

// Инициализация указателя на функцию
void (*Error::error_object_red)() = Error::init;

class StartUI : public Error, public VidioDownload, public View_DL{
private:
    static void normal_selected_item(std::string button) {
	    apply_css(("." + button + " {background: rgba(0,0,0,0.3);} ." + button + ":hover {background: rgba(0,0,0,0.2);}").c_str());}

    static void selected_item(std::string button) {
	    apply_css(("." + button + " {background: rgba(0,0,0,0.5);} ." + button + ":hover {background: rgba(0,0,0,0.6);}").c_str());
    }

    static void unselected_item(std::string button) {
	    apply_css(("." + button + " {background: rgba(255,255,255,0.5);} ." + button + ":hover {background: rgba(255,255,255,0.6);}").c_str());
    }

    static void button_clicked_vidio(GtkWidget *widget, gpointer data) {
        vidio = vidio ? false : true;
	    audio = false;
	    coloring_button();
    }

    static void button_clicked_audio(GtkWidget *widget, gpointer data) {
        audio = audio ? false : true;
	    vidio = false;
	    coloring_button();
    }

    static void button_clicked_download(GtkWidget *widget, gpointer data) {
	    if (vidio || audio){
            const char *entry_text_cstr = gtk_editable_get_text(GTK_EDITABLE(entry_url));
	        std::string entry_text = entry_text_cstr ? std::string(gtk_editable_get_text(GTK_EDITABLE(entry_url))) : "";
    	    std::cout<<"d"<<std::endl;
	        std::cout<<entry_text_cstr<<std::endl;
	        if (entry_text.find("https://") != std::string::npos){
	  	        if ((entry_text.find("https://music.youtube.com/") != std::string::npos) || (entry_text.find("https://youtube.com/") != std::string::npos) || (entry_text.find("https://www.youtube.com/") != std::string::npos)) {
		            char format[6];
                    if (vidio) {strcpy(format, "vidio");}
                    else if (audio) {strcpy(format, "audio");}
		            VidioDownload VD;
		            VD.command_cast(entry_text, format);
	            }else error_main("entry", "Введена некоректная ссылка!!!");
	        }else error_main("entry", "Ссылка не введина!!!");
	    }else error_main("button", "Не выбран формат файла!!!");
    }

    static void coloring_button(){
	    if (vidio && audio) vidio = audio = false;
	    vidio || audio ? normal_selected_item("button_download") : unselected_item("button_download");
	    vidio ? selected_item("button_vidio") : normal_selected_item("button_vidio");
	    audio ? selected_item("button_audio") : normal_selected_item("button_audio");
    }

    void window_s() {
        // Создаем окно
	    window = gtk_window_new();

        gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);
        gtk_window_set_title(GTK_WINDOW(window), "Скачка видио с youtube!");

        // Инициализируем генератор случайных чисел текущим временем
        std::srand(static_cast<unsigned int>(std::time(nullptr)));

        // Рандомайзер для фона окна
        std::vector<std::string> fons_calor = { "#99FF18", "#FFF818", "#FFA918", "#FF6618", "#FF2018", "#FF1493", "#FF18C9", "#CB18FF", "#9118FF", "#5C18FF", "#1F75FE", "#00BFFF", "#18FFE5", "#00FA9A", "#00FF00", "#7FFF00", "#CEFF1D" };

        int randomNumber_1, randomNumber_2;
        randomNumber_1 = std::rand() % fons_calor.size();
        do {
            randomNumber_2 = std::rand() % fons_calor.size();
        } while (randomNumber_2 == randomNumber_1);

        // Генерируем CSS-запрос
        apply_css(("window { background: linear-gradient(to bottom right, " + fons_calor[randomNumber_1] + ", " + fons_calor[randomNumber_2] + ");}").c_str());
        apply_css(" .titlebar { background-color: rgba(0,0,0,0.2); color: rgb(255,255,255); background-image: none; background-blend-mode: overlay; } ");
    }

    void main_grid_s() {
        main_grid = gtk_grid_new();
        gtk_window_set_child(GTK_WINDOW(window), main_grid);
        gtk_widget_add_css_class(main_grid, "main_grid");
        apply_css(".main_grid{margin: 10px;}");
        gtk_widget_set_hexpand(main_grid, TRUE);
        gtk_widget_set_vexpand(main_grid, TRUE);
    }

    void entry() {
        GtkWidget *grid_1 = gtk_grid_new();
        gtk_grid_attach(GTK_GRID(main_grid), grid_1, 0, 0, 6, 1);
        gtk_widget_set_hexpand(grid_1, TRUE);
        gtk_widget_set_vexpand(grid_1, TRUE);
	    gtk_widget_add_css_class(grid_1, "grid_1");
	
        apply_css(" .grid_1{ margin-bottom: 10px; } ");

        
        GtkWidget *grid_2 = gtk_grid_new();
        gtk_grid_attach(GTK_GRID(grid_1), grid_2, 0, 0, 1, 1);
        gtk_widget_set_hexpand(grid_2, TRUE);
        gtk_widget_set_vexpand(grid_2, TRUE);

        // Создаем GtkEntry для отображения ввода
        entry_url = gtk_entry_new();
        gtk_entry_set_alignment(GTK_ENTRY(entry_url), 1);
        // gtk_entry_set_text(GTK_ENTRY(entry_url), "Прямая ссылка на видио!!!");
        gtk_editable_set_editable(GTK_EDITABLE(entry_url), TRUE);
        gtk_widget_set_hexpand(entry_url, TRUE);
        gtk_widget_set_vexpand(entry_url, TRUE);
	    gtk_grid_attach(GTK_GRID(grid_2), entry_url, 0, 0, 1, 1);
	
        apply_css("entry { background-color: rgba(0,0,0,0.1); color: rgb(255,255,255); }");
    }

    GtkWidget *create_custom_button(const gchar *label) {
        GtkWidget *button = gtk_button_new_with_label(label);
        gtk_widget_set_hexpand(button, TRUE);
        gtk_widget_set_vexpand(button, TRUE);
        return button;
    }

    void keybord() {
        GtkWidget* button_audio = create_custom_button("Аудио");        
        gtk_widget_add_css_class(button_audio, "button_audio");
	    gtk_widget_add_css_class(button_audio, "button_choice");
        g_signal_connect(button_audio, "clicked", G_CALLBACK(button_clicked_audio), NULL);
        gtk_grid_attach(GTK_GRID(main_grid), button_audio, 0, 1, 3, 1);

        GtkWidget* button_vidio = create_custom_button("Видео");
	    gtk_widget_add_css_class(button_vidio, "button_vidio");
	    gtk_widget_add_css_class(button_vidio, "button_choice");
        g_signal_connect(button_vidio, "clicked", G_CALLBACK(button_clicked_vidio), NULL);
        gtk_grid_attach(GTK_GRID(main_grid), button_vidio, 3, 1, 3, 1);

        GtkWidget* button_download = create_custom_button("Скачать");
	    gtk_widget_add_css_class(button_download, "button_download");
        g_signal_connect(button_download, "clicked", G_CALLBACK(button_clicked_download), NULL);
        gtk_grid_attach(GTK_GRID(main_grid), button_download, 0, 2, 6, 1);

	    coloring_button();
    }


    // Функция завершения основного цикла
    static void on_window_destroy(GtkWidget *widget, gpointer data) {
        g_main_loop_quit((GMainLoop *)data);
    }
    void button_style_start(){
        apply_css("button { background: rgba(0,0,0,0.3); border-radius: 0; color: rgb(255,255,255); border: 3px solid rgba(0,0,0,0); text-shadow: none; box-shadow: none; transition: background 0.3s ease;} button:active { background: rgba(0,0,0,0);} button:hover {background: rgba(0,0,0,0.2);}");
    }
    
public:
    void start() {
	    std::cout<<"ty"<<std::endl;
        window_s();
        std::cout<<"ty1"<<std::endl;

        main_grid_s();
	    std::cout<<"ty2"<<std::endl;

        entry();
	    std::cout<<"ty3"<<std::endl;

        keybord();
	    std::cout<<"ty4"<<std::endl;

        creation_side_panel();
        std::cout<<"hj"<<std::endl;

        include_and_creation_middle_panel();
        std::cout<<"fsad"<<std::endl;

    	        
        
        button_style_start();

        
       	// Создание основного цикла
        GMainLoop *loop = g_main_loop_new(NULL, FALSE);

	    // Установка сигнала для закрытия окна
        g_signal_connect(window, "destroy", G_CALLBACK(on_window_destroy), loop);

        // Показ окна
        gtk_window_present(GTK_WINDOW(window));

        g_main_loop_run(loop);

        // Освобождение ресурсов
        g_main_loop_unref(loop);

        

        std::cout<<"gh"<<std::endl;

    }
};


int main() {
    gtk_init();
    std::cout<<"dsf"<<std::endl;
    StartUI start;
    start.start();
    
    return 0;
}

