//  g++ -g -o i main1.cpp `pkg-config --cflags --libs gtk4` -lcurl


#include <gtk/gtk.h>
#include <iostream>
#include <string>
#include <vector>
#include <ctime>

// Показ оставшихся процентов
GtkWidget *status_download_titlebar;
// Окно
GtkWidget *window;
// Random fons for window
std::vector<std::string> fons_calor = { "#99FF18", "#FFF818", "#FFA918", "#FF6618", "#FF2018", "#FF1493", "#FF18C9", "#CB18FF", "#9118FF", "#5C18FF", "#1F75FE", "#00BFFF", "#18FFE5", "#00FA9A", "#00FF00", "#7FFF00", "#CEFF1D"};


struct button {
    const char *audio, *vidio, *change_background, *open_side_panel, *close_side_panel, *download_video, *download_audio, *url_youtube, *download, *ready;
};

struct error {
    const char *not_url, *error_url_vidio, *error_none_format;
};

struct local_struct {
    const char *language, *logo_local_pash, *name_titlebar;
    button button_info;
    error error_info;
};

local_struct *local;

struct main_grids_struct{
    GtkWidget *main_grid, *center_grid, *left_grid_audio, *right_grid_vidio, *bottom_grid_info;
};

main_grids_struct *main_grids;

class EditWindow {
public:
    static void apply_css(const char* css) {
        GtkCssProvider* provider = gtk_css_provider_new();
        gtk_css_provider_load_from_string(provider, css);
        gtk_style_context_add_provider_for_display(gtk_widget_get_display(window), GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
        g_object_unref(provider);
    }
    static void window_coloring(GtkWidget *widget = NULL, gpointer data = NULL) {
        int randomNumber_1, randomNumber_2;
        randomNumber_1 = std::rand() % fons_calor.size();
        do {
            randomNumber_2 = std::rand() % fons_calor.size();
        } while (randomNumber_2 == randomNumber_1);
        apply_css(("window { background: linear-gradient(to bottom right, " + fons_calor[randomNumber_1] + ", " + fons_calor[randomNumber_2] + ");}").c_str());
    }
    static void normal_selected_item(std::string button) {
	    apply_css(("." + button + " {background: rgba(0,0,0,0.3);} ." + button + ":hover {background: rgba(0,0,0,0.2);}").c_str());}

    static void selected_item(std::string button) {
	    apply_css(("." + button + " {background: rgba(0,0,0,0.5);} ." + button + ":hover {background: rgba(0,0,0,0.6);}").c_str());
    }
};

class LocalS{
public:
    void locale_s(const char *locale_languages = setlocale(LC_ALL, "")){
        if (strncmp(locale_languages, "ru", 2) == 0){
            button btn_info = {"Аудио", "Видио", "Смена фона", "Открыть боковую панель", "Закрыть боковую панель", "Скачать видио", "Скачать аудио", "Ссылка на видио с youtube", "Скачать", "Готово!"};
            error err_info = {"Вы не ввели ссылку на видио!!!", "Неверная ссылка на видио!!!", "Невыбран формат для видио"};
            local = new local_struct{"ru", "logos/local_ru.png", "Скачивальщик видио с YouTube!!!", btn_info, err_info};
        }else{
            button btn_info = {"Audio", "Vidio", "Change background", "Open side panel", "Close side panel", "Download video", "Download audio", "Link to video from youtube", "Download", "Ready!"};
            error err_info = {"You have not entered a link to the video!!!", "Incorrect link to the video!!!", "No format selected for the video"};
            local = new local_struct{"en", "logos/local_en.png", "YouTube video downloader!!!", btn_info, err_info};
        }
    }
};

class CallbackTitlebarButton{
public:
    // Обработчик для кнопки закрытия окна
    static void on_close_button_clicked(GtkWidget *widget, gpointer window) {
        gtk_window_close(GTK_WINDOW(window));
    }

    // Обработчик для кнопки сворачивания окна
    static void on_minimize_button_clicked(GtkWidget *widget, gpointer window) {
        gtk_window_minimize(GTK_WINDOW(window));
    }

    // Обработчик для кнопки разворачивания/восстановления окна
    static void on_maximize_button_clicked(GtkWidget *widget, gpointer window) {
        if (gtk_window_is_maximized(GTK_WINDOW(window))) {
            gtk_window_unmaximize(GTK_WINDOW(window));
        } else {
            gtk_window_maximize(GTK_WINDOW(window));
        }
    }
};

class InitStart : public EditWindow, public LocalS, public CallbackTitlebarButton{
public:
   
    // Обработчик для пользовательской кнопки
    static void on_custom_button_clicked(GtkWidget *widget, gpointer data) {
        g_print("Custom button clicked!\n");
    }
    void window_s() {
        // Создаем окно
	    window = gtk_window_new();

        gtk_window_set_default_size(GTK_WINDOW(window), 900, 600);

        std::cout<<"kl"<<std::endl;
        // Инициализируем генератор случайных чисел текущим временем
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        std::cout<<"kl1"<<std::endl;
        // Раскрасска окна
        window_coloring();
    }
        
    void cretion_titlebar(){
        // Создание пользовательского заголовка
        GtkWidget *header_bar = gtk_header_bar_new();
        GtkWidget *label_titlebar = gtk_label_new(local->name_titlebar);

        // Установка заголовка и скрытие кнопки закрытия
        gtk_header_bar_set_title_widget(GTK_HEADER_BAR(header_bar), label_titlebar);
        gtk_header_bar_set_show_title_buttons(GTK_HEADER_BAR(header_bar), FALSE);

        

        // Создание кнопки закрытия и добавление ее в заголовок
        GtkWidget *close_button = gtk_button_new_from_icon_name("window-close-symbolic");
        gtk_widget_add_css_class(close_button, "window-main-button");
        g_signal_connect(close_button, "clicked", G_CALLBACK(on_close_button_clicked), window);
        gtk_header_bar_pack_end(GTK_HEADER_BAR(header_bar), close_button);
        
        // Создание кнопки разворачивания/восстановления и добавление ее в заголовок
        GtkWidget *maximize_button = gtk_button_new_from_icon_name("window-maximize-symbolic");
        gtk_widget_set_margin_end(maximize_button, 6);
        gtk_widget_add_css_class(maximize_button, "window-main-button");
        g_signal_connect(maximize_button, "clicked", G_CALLBACK(on_maximize_button_clicked), window);
        gtk_header_bar_pack_end(GTK_HEADER_BAR(header_bar), maximize_button);

        // Создание кнопки сворачивания и добавление ее в заголовок
        GtkWidget *minimize_button = gtk_button_new_from_icon_name("window-minimize-symbolic");
        gtk_widget_add_css_class(minimize_button, "window-main-button");
        gtk_widget_set_margin_end(minimize_button, 6);
        g_signal_connect(minimize_button, "clicked", G_CALLBACK(on_minimize_button_clicked), window);
        gtk_header_bar_pack_end(GTK_HEADER_BAR(header_bar), minimize_button);
        
        apply_css(R"( 
            button.window-main-button {
                border-radius: 35px; /* Делает кнопку круглой */
                background-color: rgba(0, 0, 0, 0.1); /* Убирает фон */
            }

            button.window-main-button:hover {
                background-color: rgba(255, 255, 255, 0.1); /* Добавляет легкий эффект при наведении */
            })");
        status_download_titlebar = gtk_button_new_with_label("Готово!");
        gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), status_download_titlebar);

        GtkWidget *language_button = gtk_button_new_with_label(local->language);
        gtk_header_bar_pack_start(GTK_HEADER_BAR(header_bar), language_button);

        
        // Создание пользовательской кнопки и добавление ее в заголовок
        GtkWidget *coloring_window_button = gtk_button_new();
        gtk_button_set_child(GTK_BUTTON(coloring_window_button),gtk_image_new_from_file("logos/coloring_window.png"));
        
        gtk_widget_set_margin_end(coloring_window_button, 40);
        g_signal_connect(coloring_window_button, "clicked", G_CALLBACK(window_coloring), NULL);
        gtk_header_bar_pack_end(GTK_HEADER_BAR(header_bar), coloring_window_button);

        // Установка пользовательского заголовка для окна
        gtk_window_set_titlebar(GTK_WINDOW(window), header_bar);


        // Установка параметров titlebar
        apply_css(" .titlebar { background-color: rgba(0,0,0,0.2); color: rgb(255,255,255); background-image: none; background-blend-mode: overlay; } ");
    }
    void cretion_main_grids() {
        GtkWidget *main_grid = gtk_grid_new();
        gtk_window_set_child(GTK_WINDOW(window), main_grid);
        gtk_widget_add_css_class(main_grid, "main_grid");
        apply_css(".main_grid{margin: 5px;}");
        gtk_widget_set_hexpand(main_grid, TRUE);
        gtk_widget_set_vexpand(main_grid, TRUE);

        GtkWidget *center_grid = gtk_grid_new();
        gtk_grid_attach(GTK_GRID(main_grid), center_grid, 2, 0, 4, 3);
        gtk_widget_add_css_class(center_grid, "main_grids");
        gtk_widget_set_hexpand(center_grid, TRUE);
        gtk_widget_set_vexpand(center_grid, TRUE);
        GtkWidget *left_grid_audio = gtk_grid_new();
        gtk_grid_attach(GTK_GRID(main_grid), left_grid_audio, 0, 0, 2, 5);
        gtk_widget_add_css_class(left_grid_audio, "main_grids");
        gtk_widget_set_hexpand(left_grid_audio, TRUE);
        gtk_widget_set_vexpand(left_grid_audio, TRUE);
        GtkWidget *right_grid_vidio = gtk_grid_new();
        gtk_grid_attach(GTK_GRID(main_grid), right_grid_vidio, 6, 0, 2, 5);
        gtk_widget_add_css_class(right_grid_vidio, "main_grids");
        gtk_widget_set_hexpand(right_grid_vidio, TRUE);
        gtk_widget_set_vexpand(right_grid_vidio, TRUE);
        GtkWidget *bottom_grid_info = gtk_grid_new();
        gtk_grid_attach(GTK_GRID(main_grid), bottom_grid_info, 2, 3, 4, 2);
        gtk_widget_add_css_class(bottom_grid_info, "main_grids");
        gtk_widget_set_hexpand(bottom_grid_info, TRUE);
        gtk_widget_set_vexpand(bottom_grid_info, TRUE);
        apply_css("grid.main_grids {margin: 5px;}");
        main_grids = new main_grids_struct{main_grid, center_grid, left_grid_audio, right_grid_vidio, bottom_grid_info};
    }
    void create_center_grid() {
        GtkWidget *grid = main_grids->center_grid;

        // Ввод URL
        GtkWidget *entry_url = gtk_entry_new();
        gtk_grid_attach(GTK_GRID(grid), entry_url, 1, 0, 12, 1);
        gtk_widget_set_hexpand(entry_url, TRUE);
        gtk_widget_set_vexpand(entry_url, TRUE);
        gtk_widget_add_css_class(entry_url, "entry_url");

        // Выбор папки
        GtkWidget *button_directory = gtk_button_new();
        gtk_grid_attach(GTK_GRID(grid), button_directory, 13, 0, 3, 1);
        gtk_widget_set_hexpand(button_directory, TRUE);
        gtk_widget_set_vexpand(button_directory, TRUE);
        gtk_widget_add_css_class(button_directory, "button_center");
        gtk_button_set_child(GTK_BUTTON(button_directory), gtk_image_new_from_file("logos/files.png"));

        // Открытие настроек аудио
        GtkWidget *button_settings_audio = gtk_button_new();
        gtk_grid_attach(GTK_GRID(grid), button_settings_audio, 1, 1, 2, 1);
        gtk_widget_set_hexpand(button_settings_audio, TRUE);
        gtk_widget_set_vexpand(button_settings_audio, TRUE);
        gtk_widget_add_css_class(button_settings_audio, "button_center");
        gtk_button_set_child(GTK_BUTTON(button_settings_audio), gtk_image_new_from_file("logos/right_arrow.png"));

        // Выбор аудио
        GtkWidget *button_audio = gtk_button_new();
        gtk_grid_attach(GTK_GRID(grid), button_audio, 3, 1, 5, 1);
        gtk_widget_set_hexpand(button_audio, TRUE);
        gtk_widget_set_vexpand(button_audio, TRUE);
        gtk_widget_add_css_class(button_audio, "button_center");
        gtk_button_set_child(GTK_BUTTON(button_audio), gtk_image_new_from_file("logos/audio.png"));

        // Выбор видео
        GtkWidget *button_video = gtk_button_new();
        gtk_grid_attach(GTK_GRID(grid), button_video, 8, 1, 5, 1);
        gtk_widget_set_hexpand(button_video, TRUE);
        gtk_widget_set_vexpand(button_video, TRUE);
        gtk_widget_add_css_class(button_video, "button_center");
        gtk_button_set_child(GTK_BUTTON(button_video), gtk_image_new_from_file("logos/video.png"));

        // Открытие настроек видео
        GtkWidget *button_settings_video = gtk_button_new();
        gtk_grid_attach(GTK_GRID(grid), button_settings_video, 13, 1, 3, 1);
        gtk_widget_set_hexpand(button_settings_video, TRUE);
        gtk_widget_set_vexpand(button_settings_video, TRUE);
        gtk_widget_add_css_class(button_settings_video, "button_center");
        gtk_button_set_child(GTK_BUTTON(button_settings_video), gtk_image_new_from_file("logos/left_arrow.png"));

        // Открытие подробной информации о загрузке
        GtkWidget *button_download_info = gtk_button_new();
        gtk_grid_attach(GTK_GRID(grid), button_download_info, 1, 2, 6, 1);
        gtk_widget_set_hexpand(button_download_info, TRUE);
        gtk_widget_set_vexpand(button_download_info, TRUE);
        gtk_widget_add_css_class(button_download_info, "button_center");

        GtkWidget *download_info_box_button = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
        GtkWidget *download_info_image_box_button = gtk_image_new_from_file("logos/info.png");
        gtk_widget_set_hexpand(download_info_image_box_button, TRUE);
        gtk_widget_set_vexpand(download_info_image_box_button, TRUE);
        gtk_box_append(GTK_BOX(download_info_box_button), download_info_image_box_button);
        GtkWidget *download_info_label_box_button = gtk_label_new(local->button_info.ready);
        gtk_widget_add_css_class(download_info_label_box_button, "download_info_label_box_button");
        gtk_widget_set_hexpand(download_info_label_box_button, TRUE);
        gtk_widget_set_vexpand(download_info_label_box_button, TRUE);
        gtk_box_append(GTK_BOX(download_info_box_button), download_info_label_box_button);

        gtk_button_set_child(GTK_BUTTON(button_download_info), download_info_box_button);
        gtk_widget_set_hexpand(download_info_box_button, TRUE);
        gtk_widget_set_vexpand(download_info_box_button, TRUE); 

        // Кнопка для загрузки видео
        GtkWidget *button_downloader = gtk_button_new();
        gtk_grid_attach(GTK_GRID(grid), button_downloader, 7, 2, 9, 1);
        gtk_widget_set_hexpand(button_downloader, TRUE);
        gtk_widget_set_vexpand(button_downloader, TRUE);
        gtk_widget_add_css_class(button_downloader, "button_center");

        GtkWidget *download_box_button = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
        GtkWidget *download_image_box_button = gtk_image_new_from_file("logos/download.png");
        gtk_widget_set_hexpand(download_image_box_button, TRUE);
        gtk_widget_set_vexpand(download_image_box_button, TRUE);
        gtk_box_append(GTK_BOX(download_box_button), download_image_box_button);
        GtkWidget *download_label_box_button = gtk_label_new(local->button_info.download);
        gtk_widget_add_css_class(download_label_box_button, "download_label_box_button");
        gtk_widget_set_hexpand(download_label_box_button, TRUE);
        gtk_widget_set_vexpand(download_label_box_button, TRUE);
        gtk_box_append(GTK_BOX(download_box_button), download_label_box_button);

        gtk_button_set_child(GTK_BUTTON(button_downloader), download_box_button);
        gtk_widget_set_hexpand(download_box_button, TRUE);
        gtk_widget_set_vexpand(download_box_button, TRUE);


        apply_css("button.button_center{background-color: rgba(0, 0, 0, 0.3); border-radius: 0px;} label.download_label_box_button{font-size: 30px;} label.download_info_label_box_button{font-size: 30px;}");
    }

    void button_style_start(){
        apply_css("button { background: rgba(0,0,0,0.3); color: rgb(255,255,255); border: 3px solid rgba(0,0,0,0); text-shadow: none; box-shadow: none; transition: background 0.3s ease;} button:active { background: rgba(0,0,0,0);} button:hover {background: rgba(0,0,0,0.2);}");
    }
    void start() {
        locale_s();
        std::cout<<"kl iuh"<<std::endl;
        window_s();
        cretion_titlebar();
        cretion_main_grids();
        create_center_grid();
        button_style_start();
    }
};



int main(){
    gtk_init();
    std::cout<<"dsf"<<std::endl;
    InitStart start;
    start.start();

    // Создание основного цикла
    GMainLoop *loop = g_main_loop_new(NULL, FALSE);

    // Показ окна
    gtk_window_present(GTK_WINDOW(window));

    g_main_loop_run(loop);
    // Освобождение ресурсов
    g_main_loop_unref(loop);
    std::cout<<"gh"<<std::endl;
    return 0;
}

