//  g++ -g -o d main.cpp `pkg-config --cflags --libs gtk4` -lcurl



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

struct Type_list_entry_container {
    GtkWidget *grid_1, *grid_2, *entry;
};

// Глобальная переменная для хранения 3 указателя на GtkEntry
Type_list_entry_container *list_entry_container;

// Окно
GtkWidget *window;
// Основной grid
GtkWidget *main_grid;

bool vidio = false;

bool audio = false;


class VidioDownload {
private:
    std::string exec(const char* cmd) {
        std::array<char, 128> buffer;
        std::string result;
        std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
        if (!pipe) throw std::runtime_error("popen() failed!");
        while (!feof(pipe.get())) {
            if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
                result += buffer.data();
        }
        return result;
    }
public:
    std::string command_cast(const std::string video_url) {
        // Команда для скачивания видео с помощью yt-dlp
        return exec(("yt-dlp -f bestvideo+bestaudio --merge-output-format mp4 -o \"%(title)s.%(ext)s\" \"" + video_url + "\"").c_str());
    }
};


class Error {
private:
    static void (*error_object_red)();
    static short state;
    static void apply_css(const char* css) {
        GtkCssProvider* provider = gtk_css_provider_new();
        gtk_css_provider_load_from_string(provider, css);
        gtk_style_context_add_provider_for_display(gtk_widget_get_display(window), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
        g_object_unref(provider);
    }

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
        const char* css = state % 2 == 0 ?
            " .titlebar { background-color: red; transition: background-color 0.6s ease; } " :
            " .titlebar { background-color: rgba(0,0,0,0.3); transition: background-color 2.5s ease; } ";
        apply_css(css);
    }

    static void error_entry_red() {
        const char* css = state % 2 == 0 ?
            "entry { background-color: red; transition: background-color 0.6s ease; }" :
            "entry { background-color: rgba(0,0,0,0.1); transition: background-color 2.5s ease; }";
        apply_css(css);
    }

    static void error_button_red() {
        const char* css = state % 2 == 0 ?
	    ".button_choice { background: red; transition: background 0.6s ease;}" :
	    ".button_choice { background: rgba(0,0,0,0.3); transition: background 2.5s ease; } .button_choice:hover {background: rgba(0,0,0,0.2);}";
	apply_css(css);
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
    static void error_main(const char* object) {
	state = 0;
	gchar *title = NULL;
	if (strcmp(object, "entry") == 0) {
	    gtk_window_set_title(GTK_WINDOW(window), "Некоректная ссылка!");
	    error_object_red = error_entry_red;
            error_start();
            error_entry_trembling();
            g_timeout_add(1000, (GSourceFunc)error_start, NULL);
	}else if(strcmp(object, "button") == 0){
	    gtk_window_set_title(GTK_WINDOW(window), "Невыбран формат файла!"); 
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

class StartUI : public Error {
private:
    static void normal_selected_item(std::string button) {
        GtkCssProvider *provider = gtk_css_provider_new();
	std::string css = "." + button + " {"
            "background: rgba(0,0,0,0.3);"
        "}"
        "." + button + ":hover {"
            "background: rgba(0,0,0,0.2);"
        "}";
       	gtk_css_provider_load_from_string(provider, css.c_str());
        gtk_style_context_add_provider_for_display(gtk_widget_get_display(window), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    }

    static void selected_item(std::string button) {
        GtkCssProvider *provider = gtk_css_provider_new();
	std::string css = "." + button + " {"
            "background: rgba(0,0,0,0.5);"
        "}"
        "." + button + ":hover {"
            "background: rgba(0,0,0,0.6);"
        "}";
        gtk_css_provider_load_from_string(provider, css.c_str());
        gtk_style_context_add_provider_for_display(gtk_widget_get_display(window), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    }

    static void unselected_item(std::string button) {
        GtkCssProvider *provider = gtk_css_provider_new();
	std::string css = "." + button + " {"
            "background: rgba(255,255,255,0.5);"
        "}"
        "." + button + ":hover {"
            "background: rgba(255,255,255,0.6);"
        "}";
        gtk_css_provider_load_from_string(provider, css.c_str());
        gtk_style_context_add_provider_for_display(gtk_widget_get_display(window), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
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
            const gchar *entry_text = gtk_entry_get_placeholder_text(GTK_ENTRY(list_entry_container->entry));
            if (vidio) {
		error_main("entry");
                std::cout << "В разработке" << std::endl;
                // VidioDownload down;
                // std::string output = down.command_cast(entry_text);
            }
	    if (audio) {
		error_main("button");
                std::cout << "В разработке" << std::endl;
            }
	}
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


        gtk_window_set_default_size(GTK_WINDOW(window), 800, 300);
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
        std::string css_1 = "window { background: linear-gradient(to bottom right, " + fons_calor[randomNumber_1] + ", " + fons_calor[randomNumber_2] + ");}";
        // Устанавливаем CSS для фона окна
        GtkCssProvider *cssProvider = gtk_css_provider_new();
        gtk_css_provider_load_from_string(cssProvider, css_1.c_str());

        gtk_style_context_add_provider_for_display(gdk_display_get_default(), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

	g_object_unref(cssProvider);
	GtkCssProvider *provider = gtk_css_provider_new();

        // Устанавливаем CSS
        gtk_css_provider_load_from_string(provider,
            " .titlebar { "
            "     background-color: rgba(0,0,0,0.2); "
            "     color: rgb(255,255,255); "
	    "     background-image: none;"
	    "     background-blend-mode: overlay; "
            " } "
	    );
	
        // Добавляем провайдер CSS в контекст стиля окна
        gtk_style_context_add_provider_for_display(gtk_widget_get_display(window), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
	g_object_unref(provider);
    }

    void main_grid_s() {
        main_grid = gtk_grid_new();
        gtk_window_set_child(GTK_WINDOW(window), main_grid);
        gtk_widget_set_margin_start(main_grid, 10);
        gtk_widget_set_margin_end(main_grid, 10);
        gtk_widget_set_margin_top(main_grid, 10);
        gtk_widget_set_margin_bottom(main_grid, 10);
        gtk_widget_set_hexpand(main_grid, TRUE);
        gtk_widget_set_vexpand(main_grid, TRUE);
    }

    void entry() {
        GtkWidget *grid_1 = gtk_grid_new();
        gtk_grid_attach(GTK_GRID(main_grid), grid_1, 0, 0, 4, 1);
        gtk_widget_set_hexpand(grid_1, TRUE);
        gtk_widget_set_vexpand(grid_1, TRUE);
	gtk_widget_add_css_class(grid_1, "grid_1");
	
        // Создаем CSS-провайдер
        GtkCssProvider *provider = gtk_css_provider_new();
        gtk_css_provider_load_from_string(provider, "grid { border-bottom: 10px solid rgba(0, 0, 0, 0); }");

        // Применяем CSS-провайдер к GtkGrid
	gtk_style_context_add_provider_for_display(gtk_widget_get_display(grid_1), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

        GtkWidget *grid_2 = gtk_grid_new();
        gtk_grid_attach(GTK_GRID(grid_1), grid_2, 0, 0, 1, 1);
        gtk_widget_set_hexpand(grid_2, TRUE);
        gtk_widget_set_vexpand(grid_2, TRUE);

	        
        // Создаем GtkEntry для отображения ввода
        GtkWidget *entry = gtk_entry_new();
        gtk_entry_set_alignment(GTK_ENTRY(entry), 1);
        // gtk_entry_set_text(GTK_ENTRY(entry), "Прямая ссылка на видио!!!");
        gtk_editable_set_editable(GTK_EDITABLE(entry), TRUE);
        gtk_widget_set_hexpand(entry, TRUE);
        gtk_widget_set_vexpand(entry, TRUE);
	gtk_grid_attach(GTK_GRID(grid_2), entry, 0, 0, 1, 1);
	
        provider = gtk_css_provider_new();
        gtk_css_provider_load_from_string(provider, "entry { background-color: rgba(0,0,0,0.1); color: rgb(255,255,255); }");
        gtk_style_context_add_provider_for_display(gtk_widget_get_display(entry), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

        list_entry_container = new Type_list_entry_container{ grid_1, grid_2, entry};
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
        gtk_grid_attach(GTK_GRID(main_grid), button_audio, 1, 1, 1, 1);

        GtkWidget* button_vidio = create_custom_button("Видео");
	gtk_widget_add_css_class(button_vidio, "button_vidio");
	gtk_widget_add_css_class(button_vidio, "button_choice");
        g_signal_connect(button_vidio, "clicked", G_CALLBACK(button_clicked_vidio), NULL);
        gtk_grid_attach(GTK_GRID(main_grid), button_vidio, 2, 1, 1, 1);

        GtkWidget* button_download = create_custom_button("Скачать");
	gtk_widget_add_css_class(button_download, "button_download");
        g_signal_connect(button_download, "clicked", G_CALLBACK(button_clicked_download), NULL);
        gtk_grid_attach(GTK_GRID(main_grid), button_download, 1, 2, 2, 1);

	coloring_button();
    }


    // Функция завершения основного цикла
    static void on_window_destroy(GtkWidget *widget, gpointer data) {
        g_main_loop_quit((GMainLoop *)data);
    }
    void button_style_start(){
	GtkCssProvider *provider = gtk_css_provider_new();
	std::string css = "button {"
            "border-radius: 0;"
            "color: rgb(255,255,255);"
            "border: 3px solid rgba(0,0,0,0);"
            "text-shadow: none;"
            "box-shadow: none;"
            "transition: background 0.3s ease;"
        "}"
        "button:active {"
            "background: rgba(0,0,0,0);"
        "}";
        gtk_css_provider_load_from_string(provider, css.c_str());
        gtk_style_context_add_provider_for_display(gtk_widget_get_display(window), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
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

