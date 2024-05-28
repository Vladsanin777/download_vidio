//  g++ -g -o d main.cpp `pkg-config --cflags --libs gtk+-3.0` -lcurl

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
#include <future>
#include <thread>

// Struct for holding GTK widget pointers
struct Type_list_entry_container {
    GtkWidget *grid_1, *grid_2, *entry;
};

// Global variables
Type_list_entry_container *list_entry_container = nullptr;
GtkWidget *window = nullptr;
GtkWidget *main_grid = nullptr;

class VidioDownload {
private:
    std::string exec(const std::string &cmd) {
        std::array<char, 128> buffer;
        std::string result;
        std::shared_ptr<FILE> pipe(popen(cmd.c_str(), "r"), pclose);
        if (!pipe) throw std::runtime_error("popen() failed!");
        while (fgets(buffer.data(), 128, pipe.get()) != nullptr) {
            result += buffer.data();
        }
        int returnCode = pclose(pipe.get());
        if (returnCode != 0) {
            throw std::runtime_error("Command failed with return code: " + std::to_string(returnCode));
        }
        return result;
    }
public:
    std::future<std::string> command_cast_async(const std::string &video_url) {
        return std::async(std::launch::async, &VidioDownload::exec, this, 
                          "yt-dlp -f bestvideo+bestaudio --merge-output-format mp4 -o \"%(title)s.%(ext)s\" \"" + video_url + "\"");
    }
};

class StartUI {
private:
    static void button_clicked(GtkWidget *widget, gpointer data) {
        const gchar *text = gtk_button_get_label(GTK_BUTTON(widget));
        GtkWidget *entry = (GtkWidget *)data;
        const gchar *entry_text = gtk_entry_get_text(GTK_ENTRY(entry));
        std::string entry_string(entry_text);

        if (std::string(text) == "Аудио") {
            std::cout << "Audio download selected" << std::endl;
        } else if (std::string(text) == "Видео") {
            auto down = std::make_shared<VidioDownload>();
            auto future = down->command_cast_async(entry_string);
            std::thread([down, future = std::move(future)]() mutable {
                try {
                    std::string output = future.get();
                    std::cout << "Download completed: " << output << std::endl;
                } catch (const std::exception &e) {
                    std::cerr << "Error: " << e.what() << std::endl;
                }
            }).detach();
        }

        gtk_entry_set_text(GTK_ENTRY(entry), entry_string.c_str());
    }

    void window_s() {
        window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(window), "Download Video from YouTube");
        gtk_widget_set_size_request(window, 800, 300);

        std::srand(static_cast<unsigned int>(std::time(nullptr)));

        std::vector<std::string> fons_calor = {"#99FF18", "#FFF818", "#FFA918", "#FF6618", "#FF2018", "#FF1493", "#FF18C9", "#CB18FF", "#9118FF", "#5C18FF", "#1F75FE", "#00BFFF", "#18FFE5", "#00FA9A", "#00FF00", "#7FFF00", "#CEFF1D"};

        int randomNumber_1, randomNumber_2;
        randomNumber_1 = std::rand() % fons_calor.size();
        do {
            randomNumber_2 = std::rand() % fons_calor.size();
        } while (randomNumber_2 == randomNumber_1);

        std::string css_1 = "window { background: linear-gradient(to bottom right, " + fons_calor[randomNumber_1] + ", " + fons_calor[randomNumber_2] + ");}";
        const gchar* css_data = css_1.c_str();

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
        gtk_widget_set_hexpand(grid_1, TRUE);
        gtk_widget_set_vexpand(grid_1, TRUE);

        GtkCssProvider *provider = gtk_css_provider_new();
        gtk_css_provider_load_from_data(provider, "grid { border-bottom: 10px solid rgba(0, 0, 0, 0); }", -1, NULL);
        GtkStyleContext *context = gtk_widget_get_style_context(grid_1);
        gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

        GtkWidget *grid_2 = gtk_grid_new();
        gtk_grid_attach(GTK_GRID(grid_1), grid_2, 0, 0, 1, 1);
        gtk_widget_set_hexpand(grid_2, TRUE);
        gtk_widget_set_vexpand(grid_2, TRUE);

        GtkWidget* entry = gtk_entry_new();
        gtk_entry_set_alignment(GTK_ENTRY(entry), 1);
        gtk_entry_set_text(GTK_ENTRY(entry), "0");
        gtk_editable_set_editable(GTK_EDITABLE(entry), TRUE);
        gtk_grid_attach(GTK_GRID(grid_2), entry, 0, 0, 1, 1);
        gtk_widget_set_hexpand(entry, TRUE);
        gtk_widget_set_vexpand(entry, TRUE);

        provider = gtk_css_provider_new();
        gtk_css_provider_load_from_data(provider, "entry { background-color: rgba(0,0,0,0.1);}", -1, NULL);
        gtk_style_context_add_provider(gtk_widget_get_style_context(entry), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

        list_entry_container = new Type_list_entry_container{grid_1, grid_2, entry};
    }

    GtkWidget *create_custom_button(const gchar *label) {
        GtkWidget *button = gtk_button_new_with_label(label);
        gtk_widget_set_hexpand(button, TRUE);
        gtk_widget_set_vexpand(button, TRUE);
        gtk_widget_set_name(button, "custom-button");

        GtkCssProvider *provider = gtk_css_provider_new();
        gtk_css_provider_load_from_data(provider, R"(
        button {
            background: rgba(0,0,0,0.3);
            border-radius: 0;
            color: rgb(255,255,255);
            border: 3px solid rgba(0,0,0,0);
            text-shadow: none;
            box-shadow: none;
        }
        button:hover {
            background: rgba(0,0,0,0.1);
        }
        button:active {
            background: rgba(0,0,0,0);
        }
        button:focus {
            background: rgba(0,0,0,0.3);
        }
        )",
        -1, NULL);
        gtk_style_context_add_provider(gtk_widget_get_style_context(button), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

        return button;
    }

    void keybord() {
        const gchar *button_labels[] = {
            "Аудио", "Видео"
        };

        for (int i = 0; i < 2; ++i) {
            GtkWidget* button = create_custom_button(button_labels[i]);
            g_signal_connect(button, "clicked", G_CALLBACK(button_clicked), list_entry_container->entry);
            gtk_grid_attach(GTK_GRID(main_grid), button, i % 2, i / 2 + 1, 1, 1);
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

