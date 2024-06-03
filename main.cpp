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

short vidio = 0;

struct type_keybord {
    GtkWidget *audio, *vidio, *download;
};

type_keybord *keybord_s;

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

class StartUI {
private:
    static void normal_selected_item(GtkWidget *selected_widget) {
        GtkCssProvider *provider = gtk_css_provider_new();
        gtk_css_provider_load_from_string(provider, R"(
        button {
            background: rgba(0,0,0,0.3);
            border-radius: 0;
            color: rgb(255,255,255);
            border: 3px solid rgba(0,0,0,0);
            text-shadow: none;
            box-shadow: none;
            transition: background 0.3s ease;
        }
        button:hover {
            background: rgba(0,0,0,0.1);
        }
        button:active {
            background: rgba(0,0,0,0);
        }
        )");
        gtk_style_context_add_provider_for_display(gtk_widget_get_display(selected_widget), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    }

    static void selected_item(GtkWidget *selected_widget) {
        GtkCssProvider *provider = gtk_css_provider_new();
        gtk_css_provider_load_from_string(provider, R"(
        button {
            background: rgba(0,0,0,0.5);
            border-radius: 0;
            color: rgb(255,255,255);
            border: 3px solid rgba(0,0,0,0);
            text-shadow: none;
            box-shadow: none;
            transition: background 0.3s ease;
        }
        button:hover {
            background: rgba(0,0,0,0.6);
        }
        button:active {
            background: rgba(0,0,0,0);
        }
        )");
        gtk_style_context_add_provider_for_display(gtk_widget_get_display(selected_widget), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    }

    static void unselected_item(GtkWidget *selected_widget) {
        GtkCssProvider *provider = gtk_css_provider_new();
        gtk_css_provider_load_from_string(provider, R"(
        button {
            background: rgba(255,255,255,0.5);
            border-radius: 0;
            color: rgb(255,255,255);
            border: 3px solid rgba(0,0,0,0);
            text-shadow: none;
            box-shadow: none;
            transition: background 0.3s ease;
        }
        button:hover {
            background: rgba(255,255,255,0.6);
        }
        button:active {
            background: rgba(0,0,0,0);
        }
        )");
        gtk_style_context_add_provider_for_display(gtk_widget_get_display(selected_widget), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    }

    static gboolean button_clicked_vidio(GtkWidget *widget, gpointer data) {
        if (vidio == 1) {
            normal_selected_item(keybord_s->vidio);
            unselected_item(keybord_s->download);
            vidio = 0;
        } else {
            selected_item(keybord_s->vidio);
            normal_selected_item(keybord_s->audio);
            normal_selected_item(keybord_s->download);
            vidio = 1;
        }
        return TRUE;
    }

    static gboolean button_clicked_audio(GtkWidget *widget, gpointer data) {
        if (vidio == -1) {
            normal_selected_item(keybord_s->audio);
            unselected_item(keybord_s->download);
            vidio = 0;
        } else {
            selected_item(keybord_s->audio);
            normal_selected_item(keybord_s->vidio);
            normal_selected_item(keybord_s->download);
            vidio = -1;
        }
        return TRUE;
    }

    static gboolean button_clicked_download(GtkWidget *widget, gpointer data) {
        const gchar *entry_text = gtk_search_entry_get_placeholder_text(GTK_SEARCH_ENTRY(list_entry_container->entry));
        if (vidio) {
            VidioDownload down;
            std::string output = down.command_cast(entry_text);
        } else {
            std::cout << "В разработке" << std::endl;
        }
        return TRUE;
    }

    void window_s() {
        // Создаем окно
        window = gtk_window_new();
        gtk_window_set_default_size(GTK_WINDOW(window), 800, 300);

        gtk_window_set_title(GTK_WINDOW(window), "Скачка видио с youtube");

        

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

        const gchar* css_data = css_1.c_str();

        // Устанавливаем CSS для фона окна
        GtkCssProvider *cssProvider = gtk_css_provider_new();
        gtk_css_provider_load_from_string(cssProvider, css_data);
        gtk_style_context_add_provider_for_display(gdk_display_get_default(), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
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
        gtk_grid_attach(GTK_GRID(grid_2), entry, 0, 0, 1, 1);
        gtk_widget_set_hexpand(entry, TRUE);
        gtk_widget_set_vexpand(entry, TRUE);

        provider = gtk_css_provider_new();
        gtk_css_provider_load_from_string(provider, "entry { background-color: rgba(0,0,0,0.1);}");
        gtk_style_context_add_provider_for_display(gtk_widget_get_display(entry), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

        list_entry_container = new Type_list_entry_container{ grid_1, grid_2, entry };
    }

    GtkWidget *create_custom_button(const gchar *label) {
        GtkWidget *button = gtk_button_new_with_label(label);
        gtk_widget_set_hexpand(button, TRUE);
        gtk_widget_set_vexpand(button, TRUE);
        return button;
    }

    void keybord() {
        GtkWidget* button_audio = create_custom_button("Аудио");
        normal_selected_item(button_audio);
        g_signal_connect(button_audio, "clicked", G_CALLBACK(button_clicked_audio), NULL);
        gtk_grid_attach(GTK_GRID(main_grid), button_audio, 1, 1, 1, 1);

        GtkWidget* button_vidio = create_custom_button("Видео");
        normal_selected_item(button_vidio);
        g_signal_connect(button_vidio, "clicked", G_CALLBACK(button_clicked_vidio), NULL);
        gtk_grid_attach(GTK_GRID(main_grid), button_vidio, 2, 1, 1, 1);

        GtkWidget* button_download = create_custom_button("Скачать");
        unselected_item(button_download);
        g_signal_connect(button_download, "clicked", G_CALLBACK(button_clicked_download), NULL);
        gtk_grid_attach(GTK_GRID(main_grid), button_download, 1, 2, 2, 1);

        keybord_s = new type_keybord{ button_audio, button_vidio, button_download };
    }

public:
    void start() {
        window_s();
        main_grid_s();
        entry();
        keybord();
        // g_signal_connect(window, "destroy", G_CALLBACK(g_main_quit), NULL);

        gtk_window_present(GTK_WINDOW(window));
    }
};

int main(int argc, char *argv[]) {
    
    StartUI start;
    start.start();

    return 0;
}

