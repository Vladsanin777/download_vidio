//  g++ -g -o d main.cpp `pkg-config --cflags --libs gtk+-3.0` -lcurl

#include <gtk/gtk.h>
#include <iostream>
#include <string>
#include <cstdlib>

// Заголовки для работы с процессами
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

//Окно
GtkWidget *window;
//Основной grid
GtkWidget *main_grid;

bool vidio = NULL;


struct type_keybord{
    GtkWidget *audio, *vidio;
};

type_keybord *keybord_s;

class VidioDownload{
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
    std::string command_cast(const std::string video_url){
	// Команда для скачивания видео с помощью yt-dlp
        return exec(("yt-dlp -f bestvideo+bestaudio --merge-output-format mp4 -o \"%(title)s.%(ext)s\" \"" + video_url + "\"").c_str());
    }
};


class StartUI {
private:
    static void unselected_item(GtkWidget *unselected_widget){
        GtkCssProvider *provider = gtk_css_provider_new();
        gtk_css_provider_load_from_data(provider,R"(
        button {
            background: rgba(0,0,0,0.3);
            border-radius: 0;
            color: rgb(255,255,255);
            border: 3px solid rgba(0,0,0,0);
            text-shadow: none;
            box-shadow: none; /* Убирает возможные тени */
        }
        button:hover {
            background: rgba(0,0,0,0.1);
        }
        button:active {
            background: rgba(0,0,0,0);
        }
        button:focus {
            background: rgba(0,0,0,0.3); /* Убирает изменение цвета при фокусировке */
        }
        )", -1, NULL);
        gtk_style_context_add_provider(gtk_widget_get_style_context(unselected_widget), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    }
    static void selected_item(GtkWidget *selected_widget){
        GtkCssProvider *provider = gtk_css_provider_new();
        gtk_css_provider_load_from_data(provider,R"(
        button {
            background: rgba(0,0,0,0.5);
            border-radius: 0;
            color: rgb(255,255,255);
            border: 3px solid rgba(0,0,0,0);
            text-shadow: none;
            box-shadow: none; /* Убирает возможные тени */
        }
        button:hover {
            background: rgba(0,0,0,0.1);
        }
        button:active {
            background: rgba(0,0,0,0);
        }
        button:focus {
            background: rgba(0,0,0,0.5); /* Убирает изменение цвета при фокусировке */
        }
        )", -1, NULL);
        gtk_style_context_add_provider(gtk_widget_get_style_context(selected_widget), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

    }

    static void button_clicked_vidio(GtkWidget *widget, gpointer data) {
	selected_item(keybord_s->vidio);
	unselected_item(keybord_s->audio);
	vidio = true;
    }
    
    static void button_clicked_audio(GtkWidget *widget, gpointer data) {
	selected_item(keybord_s->audio);
	unselected_item(keybord_s->vidio);
	vidio = false;
    }

    static void button_clicked_download(GtkWidget *widget, gpointer data) {
	const gchar *entry_text = gtk_entry_get_text(GTK_ENTRY(list_entry_container->entry));
        if (vidio){
            VidioDownload down;
            std::string output = down.command_cast(entry_text);   
	}else{
            std::cout<<"В разработке"<<std::endl;
	}
    }

    void window_s() {


        // Создаем окно
        window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_widget_set_size_request(window, 800, 300);
	GtkWidget *header_bar = gtk_header_bar_new();
	gtk_widget_set_name(header_bar, "custom-titlebar");
        gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(header_bar), TRUE);
        gtk_header_bar_set_title(GTK_HEADER_BAR(header_bar), "Скачка видио с youtube");
	GtkCssProvider *provider = gtk_css_provider_new();
        gtk_css_provider_load_from_data(provider,
            "#custom-titlebar {"
            "  background-color: rgba(0,0,0,0.3);"
            "}", -1, NULL);

        GtkStyleContext *context = gtk_widget_get_style_context(header_bar);
        gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
	g_object_unref(provider);
	gtk_window_set_titlebar(GTK_WINDOW(window), header_bar);

        // Инициализируем генератор случайных чисел текущим временем
        std::srand(static_cast<unsigned int>(std::time(nullptr)));

        // Рандомайзер для фона окна
        std::vector<std::string> fons_calor = {"#99FF18", "#FFF818", "#FFA918", "#FF6618", "#FF2018", "#FF1493", "#FF18C9", "#CB18FF", "#9118FF", "#5C18FF", "#1F75FE", "#00BFFF", "#18FFE5", "#00FA9A", "#00FF00", "#7FFF00", "#CEFF1D"};

        int randomNumber_1, randomNumber_2;
        randomNumber_1 = std::rand() % fons_calor.size();
        do {
            randomNumber_2 = std::rand() % fons_calor.size();
        } while (randomNumber_2 == randomNumber_1);

        // Генерируем CSS-запрос
        std::string css_1 = "window { background: linear-gradient(to bottom right, " + fons_calor[randomNumber_1] + ", " + fons_calor[randomNumber_2] + ");}";

        const gchar* css_data = css_1.c_str();

        // Устанавливаем CSS для фона окна
        GtkCssProvider *cssProvider = gtk_css_provider_new();
        gtk_css_provider_load_from_data(cssProvider, css_data, -1, NULL);
        gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    }

    void main_grid_s() {
        main_grid = gtk_grid_new();
        gtk_container_add(GTK_CONTAINER(window), main_grid);
        gtk_container_set_border_width(GTK_CONTAINER(main_grid), 10);
        gtk_widget_set_hexpand(main_grid, TRUE);
        gtk_widget_set_vexpand(main_grid, TRUE);
    }

    void entry() {
        GtkWidget *grid_1 = gtk_grid_new();
        gtk_grid_attach(GTK_GRID(main_grid), grid_1, 0, 0, 4, 1);
        gtk_widget_set_hexpand(grid_1, TRUE); // Растяжение по горизонтали
        gtk_widget_set_vexpand(grid_1, TRUE); // Не растягивать по вертикали


        // Создаем CSS-провайдер
        GtkCssProvider *provider = gtk_css_provider_new();
        gtk_css_provider_load_from_data(provider, "grid { border-bottom: 10px solid rgba(0, 0, 0, 0); }", -1, NULL);

        // Применяем CSS-провайдер к GtkGrid
        GtkStyleContext *context = gtk_widget_get_style_context(grid_1);
        gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);


        GtkWidget *grid_2 = gtk_grid_new();
        gtk_grid_attach(GTK_GRID(grid_1), grid_2, 0, 0, 1, 1);
        gtk_widget_set_hexpand(grid_2, TRUE); // Растяжение по горизонтали
        gtk_widget_set_vexpand(grid_2, TRUE); // Не растягивать по вертикали

        // Создаем GtkEntry для отображения ввода
        GtkWidget* entry = gtk_entry_new();
        gtk_entry_set_alignment(GTK_ENTRY(entry), 1); // Выравнивание по правому краю
        gtk_entry_set_text(GTK_ENTRY(entry), "0");
        gtk_editable_set_editable(GTK_EDITABLE(entry), TRUE); // Не редактируемый
        gtk_grid_attach(GTK_GRID(grid_2), entry, 0, 0, 1, 1);
        gtk_widget_set_hexpand(entry, TRUE); // Растяжение по горизонтали
        gtk_widget_set_vexpand(entry, TRUE); // Растяжение по вертикали

        provider = gtk_css_provider_new();
        gtk_css_provider_load_from_data(provider, "entry { background-color: rgba(0,0,0,0.1);}", -1, NULL);
        gtk_style_context_add_provider(gtk_widget_get_style_context(entry), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

        list_entry_container = new Type_list_entry_container{grid_1, grid_2, entry};
    }

    GtkWidget *create_custom_button(const gchar *label) {
        GtkWidget *button = gtk_button_new_with_label(label);
        gtk_widget_set_hexpand(button, TRUE); // Кнопки растягиваются по горизонтали
        gtk_widget_set_vexpand(button, TRUE); // Кнопки растягиваются по вертикали
        GtkCssProvider *provider = gtk_css_provider_new();
        gtk_css_provider_load_from_data(provider,R"(
        button {
            background: rgba(0,0,0,0.3);
            border-radius: 0;
            color: rgb(255,255,255);
            border: 3px solid rgba(0,0,0,0);
            text-shadow: none;
            box-shadow: none; /* Убирает возможные тени */
        }
        button:hover {
            background: rgba(0,0,0,0.1);
        }
        button:active {
            background: rgba(0,0,0,0);
        }
        button:focus {
            background: rgba(0,0,0,0.3); /* Убирает изменение цвета при фокусировке */
        }
        )", -1, NULL);
        gtk_style_context_add_provider(gtk_widget_get_style_context(button), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

	return button;
    }
    
    void keybord() {
        
        GtkWidget* button_audio = create_custom_button("Аудио");
        g_signal_connect(button_audio, "clicked", G_CALLBACK(button_clicked_audio), list_entry_container->entry);
        gtk_grid_attach(GTK_GRID(main_grid), button_audio, 1, 1, 1, 1);
        
        GtkWidget* button_vidio = create_custom_button("Видео");
        g_signal_connect(button_vidio, "clicked", G_CALLBACK(button_clicked_vidio), list_entry_container->entry);
        gtk_grid_attach(GTK_GRID(main_grid), button_vidio, 2, 1, 1, 1);

	GtkWidget* button_download = create_custom_button("Скачать");
        g_signal_connect(button_download, "clicked", G_CALLBACK(button_clicked_download), list_entry_container->entry);
        gtk_grid_attach(GTK_GRID(main_grid), button_download, 1, 2, 2, 1);

	keybord_s = new type_keybord{button_audio, button_vidio};
    }

    void view_vidio(){
        if (vidio){
	    
	}
    }

public:
    void start() {
        window_s();
        main_grid_s();
        entry();
        keybord();
        g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);


        gtk_widget_show_all(window);

        gtk_main();
    }
};


int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);


    StartUI start;
    start.start();

    return 0;
}
