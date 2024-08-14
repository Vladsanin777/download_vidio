//  g++ -g -o i main1.cpp `pkg-config --cflags --libs gtk4` -lcurl


#include <gtk/gtk.h>
#include <iostream>
#include <string>
#include <vector>
#include <ctime>


bool open_audio_panel = false;

bool open_video_panel = false;

bool open_info_panel = false;
// Показ оставшихся процентов      || Окно  || Кнопка аудио || Кнопка видео
GtkWidget *status_download_titlebar, *window, *button_audio, *button_video;
// Random fons for window
std::vector<std::string> fons_calor = { "#99FF18", "#FFF818", "#FFA918", "#FF6618", "#FF2018", "#FF1493", "#FF18C9", "#CB18FF", "#9118FF", "#5C18FF", "#1F75FE", "#00BFFF", "#18FFE5", "#00FA9A", "#00FF00", "#7FFF00", "#CEFF1D"};

struct button_name_sound_quality {
    const char *best, *good, *medium, *low;
};

struct title_box {
    const char *sound_quality, *format_audio, *max_resolution, *format_video;
};

struct button {
    const char *audio, *video, *change_background, *open_side_panel, *close_side_panel, *download_video, *download_audio, *url_youtube, *download, *ready;
};

struct error {
    const char *not_url, *error_url_vidio, *error_none_format;
};

struct local_struct {
    const char *you_are_not_downloading_anything, *language, *logo_local_pash, *name_titlebar;
    button button_info;
    error error_info;
    title_box title_box_info;
    button_name_sound_quality button_name_sound_quality_info;
};

local_struct *local;

struct main_widgets_struct{
    GtkWidget *main_grid, *center_grid, *box_audio, *box_video, *bottom_scrolled_window_info;
};

main_widgets_struct *main_widgets;

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
            button btn_info = {"Аудио", "Видео", "Смена фона", "Открыть боковую панель", "Закрыть боковую панель", "Скачать видио", "Скачать аудио", "Ссылка на видио с youtube", "Скачать!", "Готово!"};
            error err_info = {"Вы не ввели ссылку на видио!!!", "Неверная ссылка на видио!!!", "Невыбран формат для видио"};
            title_box titb_info = {"Качество\nзвука", "Формат\nАудио", "Макс.\nразрешение", "Формат\nвидео"};
            button_name_sound_quality but_name_sound_qual_info = {"Лучшее", "Хорошее", "Средние", "Низкое"};
            local = new local_struct{"Вы ничего не скачиваете!!!", "ru", "logos/local_ru.png", "Скачивальщик видио с YouTube!!!", btn_info, err_info, titb_info, but_name_sound_qual_info};
        }else{
            button btn_info = {"Audio", "Vidio", "Change background", "Open side panel", "Close side panel", "Download video", "Download audio", "Link to video from youtube", "Download!", "Ready!"};
            error err_info = {"You have not entered a link to the video!!!", "Incorrect link to the video!!!", "No format selected for the video"};
            title_box titb_info= {"Sound\nquality", "Format\naudio", "Max\nresolution", "Format\nvideo"};
            button_name_sound_quality but_name_sound_qual_info = {"Best", "Good", "Medium", "Low"};
            local = new local_struct{"You are not downloading anything!!!", "en", "logos/local_en.png", "YouTube video downloader!!!", btn_info, err_info, titb_info, but_name_sound_qual_info};
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

class PressingButton : public EditWindow {
public:
    static void hiding_audio_panel(GtkWidget *widget, bool *pdata) {
        std::cout<<"fvsdfs"<<std::endl;
        gtk_widget_set_visible(main_widgets->box_audio, *pdata ? FALSE : TRUE);
        gtk_widget_set_visible(main_widgets->box_video, FALSE);
        // int width = gtk_widget_get_width(GTK_WIDGET(window));
        // int height = gtk_widget_get_height(GTK_WIDGET(window));
        // gtk_window_set_default_size(GTK_WINDOW(window), width + 250, height);
        open_video_panel = false;
        apply_css((std::string("button.video{background-color: rgba(0, 0, 0, 0.3);} button.video:hover{background-color: rgba(0, 0, 0, 0.2);} label.video_label_box_button{color: rgb(255, 255, 255);} label.video_label_box_button:hover{color: rgb(0, 0, 0);}") + std::string(*pdata ? "button.audio{background-color: rgba(0, 0, 0, 0.3);} button.audio:hover{background-color: rgba(0, 0, 0, 0.2);} label.audio_label_box_button{color: rgb(255, 255, 255);} label.audio_label_box_button:hover{color: rgb(0, 0, 0);}" : "button.audio{background-color: rgba(0, 0, 0, 0.5);} button.audio:hover{background-color: rgba(0, 0, 0, 0.7);} label.audio_label_box_button{color: rgb(0, 0, 0);} label.audio_label_box_button:hover{color: rgb(255, 255, 255);} ")).c_str());
        *pdata = !*pdata;
    }
    static void hiding_video_panel(GtkWidget *widget, bool *pdata) {
        std::cout<<"fvsdfs2"<<std::endl;
        gtk_widget_set_visible(main_widgets->box_video, *pdata ? FALSE : TRUE);
        gtk_widget_set_visible(main_widgets->box_audio, FALSE);
        open_audio_panel = false;
        apply_css((std::string("button.audio{background-color: rgba(0, 0, 0, 0.3);} button.audio:hover{background-color: rgba(0, 0, 0, 0.2);} label.audio_label_box_button{color: rgb(255, 255, 255);} label.audio_label_box_button:hover{color: rgb(0, 0, 0);}") + std::string(*pdata ? "button.video{background-color: rgba(0, 0, 0, 0.3);} button.video:hover{background-color: rgba(0, 0, 0, 0.2);} label.video_label_box_button{color: rgb(255, 255, 255);} label.video_label_box_button:hover{color: rgb(0, 0, 0);}" : "button.video{background-color: rgba(0, 0, 0, 0.5);} button.video:hover{background-color: rgba(0, 0, 0, 0.7);} label.video_label_box_button{color: rgb(0, 0, 0);} label.video_label_box_button:hover{color: rgb(255, 255, 255);}")).c_str());
        *pdata = !*pdata;
    }
    static void hiding_bottom_panel(GtkWidget *widget, bool *pdata) {
        gtk_widget_set_visible(main_widgets->bottom_scrolled_window_info, *pdata ? FALSE : TRUE);
        *pdata = !*pdata;
    }
};

class InitStart : public LocalS, public CallbackTitlebarButton, public PressingButton{
public:
   
    // Обработчик для пользовательской кнопки
    static void on_custom_button_clicked(GtkWidget *widget, gpointer data) {
        g_print("Custom button clicked!\n");
    }
    void window_s() {
        // Создаем окно
	    window = gtk_window_new();

        gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);

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
        status_download_titlebar = gtk_button_new_with_label(local->button_info.ready);
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
        GtkWidget *main_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
        gtk_window_set_child(GTK_WINDOW(window), main_box);
        gtk_widget_add_css_class(main_box, "main_widget");
        gtk_widget_set_hexpand(main_box, TRUE);
        gtk_widget_set_vexpand(main_box, TRUE);

        GtkWidget *center_grid = gtk_grid_new();
        gtk_box_append(GTK_BOX(main_box), center_grid);
        gtk_widget_add_css_class(center_grid, "main_widget");
        gtk_widget_set_hexpand(center_grid, TRUE);
        gtk_widget_set_vexpand(center_grid, TRUE);

        GtkWidget *box_audio = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        gtk_box_append(GTK_BOX(main_box), box_audio);
        gtk_widget_add_css_class(box_audio, "main_widget");
        gtk_widget_set_hexpand(box_audio, TRUE);
        gtk_widget_set_vexpand(box_audio, TRUE);

        GtkWidget *box_video = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        gtk_box_append(GTK_BOX(main_box), box_video);
        gtk_widget_add_css_class(box_video, "main_widget");
        gtk_widget_set_hexpand(box_video, TRUE);
        gtk_widget_set_vexpand(box_video, TRUE);

        GtkWidget *bottom_scrolled_window_info = gtk_scrolled_window_new();
        gtk_grid_attach(GTK_GRID(center_grid), bottom_scrolled_window_info, 0, 4, 2, 2);
        gtk_widget_add_css_class(bottom_scrolled_window_info, "main_widget");
        gtk_widget_set_hexpand(bottom_scrolled_window_info, TRUE);
        gtk_widget_set_vexpand(bottom_scrolled_window_info, TRUE);

        apply_css("box.main_widget{min-width: 150px; margin: 5px;} grid.main_widget{min-height: 300px; min-width: 450px; margin: 5px;} scrolledwindow.main_widget{min-height: 100px; margin-top: 10px;}"); 
        main_widgets = new main_widgets_struct{main_box, center_grid, box_audio, box_video, bottom_scrolled_window_info};
    }
    void create_center_grid() {
        GtkWidget *grid = main_widgets->center_grid;

        // Ввод URL
        GtkWidget *entry_url = gtk_entry_new();
        gtk_grid_attach(GTK_GRID(grid), entry_url, 0, 0, 2, 1);
        gtk_widget_set_hexpand(entry_url, TRUE);
        gtk_widget_set_vexpand(entry_url, TRUE);
        gtk_widget_add_css_class(entry_url, "entry_url");

        // Является ли эта ссылка ссылкой на плей лист
        GtkWidget *button_is_playlist = gtk_button_new_with_label("Пелейлист?");
        gtk_grid_attach(GTK_GRID(grid), button_is_playlist, 0, 1, 1, 1);
        gtk_widget_set_hexpand(button_is_playlist, TRUE);
        gtk_widget_set_vexpand(button_is_playlist, TRUE);
        gtk_widget_add_css_class(button_is_playlist, "button_center");

        // Выбор папки
        GtkWidget *button_directory = gtk_button_new();
        gtk_grid_attach(GTK_GRID(grid), button_directory, 1, 1, 1, 1);
        gtk_widget_set_hexpand(button_directory, TRUE);
        gtk_widget_set_vexpand(button_directory, TRUE);
        gtk_widget_add_css_class(button_directory, "button_center");
        gtk_button_set_child(GTK_BUTTON(button_directory), gtk_image_new_from_file("logos/files.png"));
        
        // Выбор аудио
        button_audio = gtk_button_new();
        gtk_grid_attach(GTK_GRID(grid), button_audio, 0, 2, 1, 1);
        gtk_widget_set_hexpand(button_audio, TRUE);
        gtk_widget_set_vexpand(button_audio, TRUE);
        gtk_widget_add_css_class(button_audio, "button_center");
        gtk_widget_add_css_class(button_audio, "audio");

        g_signal_connect(button_audio, "clicked", G_CALLBACK(hiding_audio_panel), &open_audio_panel);
        GtkWidget *audio_box_button = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
        GtkWidget *audio_image_box_button = gtk_image_new_from_file("logos/audio.png");
        gtk_widget_set_hexpand(audio_image_box_button, TRUE);
        gtk_widget_set_vexpand(audio_image_box_button, TRUE);
        gtk_box_append(GTK_BOX(audio_box_button), audio_image_box_button);
        GtkWidget *audio_label_box_button = gtk_label_new(local->button_info.audio);
        gtk_widget_add_css_class(audio_label_box_button, "label_box_button");
        gtk_widget_add_css_class(audio_label_box_button, "audio_label_box_button");
        gtk_widget_set_hexpand(audio_label_box_button, TRUE);
        gtk_widget_set_vexpand(audio_label_box_button, TRUE);
        gtk_box_append(GTK_BOX(audio_box_button), audio_label_box_button);
        gtk_button_set_child(GTK_BUTTON(button_audio), audio_box_button);
        gtk_widget_set_hexpand(audio_box_button, TRUE);
        gtk_widget_set_vexpand(audio_box_button, TRUE);

        // Выбор видео
        button_video = gtk_button_new();
        gtk_grid_attach(GTK_GRID(grid), button_video, 1, 2, 1, 1);
        gtk_widget_set_hexpand(button_video, TRUE);
        gtk_widget_set_vexpand(button_video, TRUE);
        gtk_widget_add_css_class(button_video, "button_center");
        gtk_widget_add_css_class(button_video, "video");
        gtk_button_set_child(GTK_BUTTON(button_video), gtk_image_new_from_file("logos/video.png"));
        g_signal_connect(button_video, "clicked", G_CALLBACK(hiding_video_panel), &open_video_panel);
        GtkWidget *video_box_button = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
        GtkWidget *video_image_box_button = gtk_image_new_from_file("logos/video.png");
        gtk_widget_set_hexpand(video_image_box_button, TRUE);
        gtk_widget_set_vexpand(video_image_box_button, TRUE);
        gtk_box_append(GTK_BOX(video_box_button), video_image_box_button);
        GtkWidget *video_label_box_button = gtk_label_new(local->button_info.video);
        gtk_widget_add_css_class(video_label_box_button, "label_box_button");
        gtk_widget_add_css_class(video_label_box_button, "video_label_box_button");
        gtk_widget_set_hexpand(video_label_box_button, TRUE);
        gtk_widget_set_vexpand(video_label_box_button, TRUE);
        gtk_box_append(GTK_BOX(video_box_button), video_label_box_button);
        gtk_button_set_child(GTK_BUTTON(button_video), video_box_button);
        gtk_widget_set_hexpand(video_box_button, TRUE);
        gtk_widget_set_vexpand(video_box_button, TRUE);
        
        // Открытие подробной информации о загрузке
        GtkWidget *button_download_info = gtk_button_new();
        gtk_grid_attach(GTK_GRID(grid), button_download_info, 0, 3, 1, 1);
        gtk_widget_set_hexpand(button_download_info, TRUE);
        gtk_widget_set_vexpand(button_download_info, TRUE);
        gtk_widget_add_css_class(button_download_info, "button_center");

        g_signal_connect(button_download_info, "clicked", G_CALLBACK(hiding_bottom_panel), &open_info_panel);

        GtkWidget *download_info_box_button = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
        GtkWidget *download_info_image_box_button = gtk_image_new_from_file("logos/info.png");
        gtk_widget_set_hexpand(download_info_image_box_button, TRUE);
        gtk_widget_set_vexpand(download_info_image_box_button, TRUE);
        gtk_box_append(GTK_BOX(download_info_box_button), download_info_image_box_button);
        GtkWidget *download_info_label_box_button = gtk_label_new(local->button_info.ready);
        gtk_widget_add_css_class(download_info_label_box_button, "label_box_button");
        gtk_widget_set_hexpand(download_info_label_box_button, TRUE);
        gtk_widget_set_vexpand(download_info_label_box_button, TRUE);
        gtk_box_append(GTK_BOX(download_info_box_button), download_info_label_box_button);
        gtk_button_set_child(GTK_BUTTON(button_download_info), download_info_box_button);
        gtk_widget_set_hexpand(download_info_box_button, TRUE);
        gtk_widget_set_vexpand(download_info_box_button, TRUE); 

        // Кнопка для загрузки видео
        GtkWidget *button_downloader = gtk_button_new();
        gtk_grid_attach(GTK_GRID(grid), button_downloader, 1, 3, 1, 1);
        gtk_widget_set_hexpand(button_downloader, TRUE);
        gtk_widget_set_vexpand(button_downloader, TRUE);
        gtk_widget_add_css_class(button_downloader, "button_center");

        GtkWidget *download_box_button = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
        GtkWidget *download_image_box_button = gtk_image_new_from_file("logos/download.png");
        gtk_widget_set_hexpand(download_image_box_button, TRUE);
        gtk_widget_set_vexpand(download_image_box_button, TRUE);
        gtk_box_append(GTK_BOX(download_box_button), download_image_box_button);
        GtkWidget *download_label_box_button = gtk_label_new(local->button_info.download);
        gtk_widget_add_css_class(download_label_box_button, "label_box_button");
        gtk_widget_set_hexpand(download_label_box_button, TRUE);
        gtk_widget_set_vexpand(download_label_box_button, TRUE);
        gtk_box_append(GTK_BOX(download_box_button), download_label_box_button);

        gtk_button_set_child(GTK_BUTTON(button_downloader), download_box_button);
        gtk_widget_set_hexpand(download_box_button, TRUE);
        gtk_widget_set_vexpand(download_box_button, TRUE);


        apply_css("button.button_center{background-color: rgba(0, 0, 0, 0.3); border-radius: 0px;} label.label_box_button{font-size: 30px;} entry.entry_url{border-radius: 0px; background: rgba(0,0,0,0.2); border: 0px;} entry.entry_url:hover{background: rgba(0,0,0,0.1);}");
    }
    void create_bottom_scrolled_window_info() {
        GtkWidget *scrolled_window = main_widgets->bottom_scrolled_window_info;
        
        GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window), box);
        gtk_widget_set_vexpand(box, TRUE);
        gtk_widget_add_css_class(box, "bottom_box");

        GtkWidget *label = gtk_label_new(local->you_are_not_downloading_anything);
        gtk_widget_add_css_class(label, "label_you_are_not_downloading_anything");
        std::cout<<local->you_are_not_downloading_anything<<std::endl;
        apply_css("label.label_you_are_not_downloading_anything{ background-color: rgba(0, 0, 0, 0.3); font-size: 25px; color: rgb(255, 255, 255);} label.label_you_are_not_downloading_anything:hover{ background-color: rgba(0, 0, 0, 0.3); color: rgb(0, 0, 0);}");
        gtk_widget_set_vexpand(label, TRUE);
        gtk_widget_set_hexpand(label, TRUE);
        gtk_box_append(GTK_BOX(box), label);
        gtk_widget_set_visible(scrolled_window, FALSE);
        // apply_css("box.bottom_box{background: rgba(0, 0, 0, 0.1);}");
    }
    void create_left_scrolled_window_audio(){
        GtkWidget *box = main_widgets->box_audio;

        GtkWidget *box_audio_sound_quality = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        gtk_box_append(GTK_BOX(box), box_audio_sound_quality);

        GtkWidget *label_audio_sound_quality = gtk_label_new(local->title_box_info.sound_quality);
        gtk_box_append(GTK_BOX(box_audio_sound_quality), label_audio_sound_quality);
        gtk_widget_add_css_class(label_audio_sound_quality, "label_box_audio_video");

        GtkWidget *scrolled_window_audio_sound_quality = gtk_scrolled_window_new();

        gtk_widget_set_vexpand(scrolled_window_audio_sound_quality, TRUE);

        gtk_box_append(GTK_BOX(box_audio_sound_quality), scrolled_window_audio_sound_quality);

        GtkWidget *box_audio_sound_quality_nested = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        
        gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window_audio_sound_quality), box_audio_sound_quality_nested);

        GtkWidget *button_audio_sound_quality_best = gtk_button_new_with_label(local->button_name_sound_quality_info.best);
        gtk_widget_add_css_class(button_audio_sound_quality_best, "button_box_audio_video");
        gtk_box_append(GTK_BOX(box_audio_sound_quality_nested), button_audio_sound_quality_best);
        gtk_widget_set_vexpand(button_audio_sound_quality_best, TRUE);

        GtkWidget *button_audio_sound_quality_good = gtk_button_new_with_label(local->button_name_sound_quality_info.good);
        gtk_widget_add_css_class(button_audio_sound_quality_good, "button_box_audio_video");
        gtk_box_append(GTK_BOX(box_audio_sound_quality_nested), button_audio_sound_quality_good);
        gtk_widget_set_vexpand(button_audio_sound_quality_good, TRUE);

        GtkWidget *button_audio_sound_quality_medium = gtk_button_new_with_label(local->button_name_sound_quality_info.medium);
        gtk_widget_add_css_class(button_audio_sound_quality_medium, "button_box_audio_video");
        gtk_box_append(GTK_BOX(box_audio_sound_quality_nested), button_audio_sound_quality_medium);
        gtk_widget_set_vexpand(button_audio_sound_quality_medium, TRUE);

        GtkWidget *button_audio_sound_quality_low = gtk_button_new_with_label(local->button_name_sound_quality_info.low);
        gtk_widget_add_css_class(button_audio_sound_quality_low, "button_box_audio_video");
        gtk_box_append(GTK_BOX(box_audio_sound_quality_nested), button_audio_sound_quality_low);
        gtk_widget_set_vexpand(button_audio_sound_quality_low, TRUE);

        // Выбор формата аудио
        GtkWidget *box_audio_format_audio = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        gtk_box_append(GTK_BOX(box), box_audio_format_audio);

        GtkWidget *label_audio_format_audio = gtk_label_new(local->title_box_info.format_audio);
        gtk_box_append(GTK_BOX(box_audio_format_audio), label_audio_format_audio);
        gtk_widget_add_css_class(label_audio_format_audio, "label_box_audio_video");

        GtkWidget *scrolled_window_audio_format_audio = gtk_scrolled_window_new();
        gtk_widget_set_vexpand(scrolled_window_audio_format_audio, TRUE);

        gtk_box_append(GTK_BOX(box_audio_format_audio), scrolled_window_audio_format_audio);

        GtkWidget *box_audio_format_audio_nested = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

        gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window_audio_format_audio), box_audio_format_audio_nested);

        GtkWidget *button_audio_format_audio_AAC = gtk_button_new_with_label("AAC");
        gtk_widget_add_css_class(button_audio_format_audio_AAC, "button_box_audio_video");
        gtk_box_append(GTK_BOX(box_audio_format_audio_nested), button_audio_format_audio_AAC);
        gtk_widget_set_vexpand(button_audio_format_audio_AAC, TRUE);

        GtkWidget *button_audio_format_audio_OGG = gtk_button_new_with_label("OGG");
        gtk_widget_add_css_class(button_audio_format_audio_OGG, "button_box_audio_video");
        gtk_box_append(GTK_BOX(box_audio_format_audio_nested), button_audio_format_audio_OGG);
        gtk_widget_set_vexpand(button_audio_format_audio_OGG, TRUE);

        GtkWidget *button_audio_format_audio_MP3 = gtk_button_new_with_label("MP3");
        gtk_widget_add_css_class(button_audio_format_audio_MP3, "button_box_audio_video");
        gtk_box_append(GTK_BOX(box_audio_format_audio_nested), button_audio_format_audio_MP3);
        gtk_widget_set_vexpand(button_audio_format_audio_MP3, TRUE);

        GtkWidget *button_audio_format_audio_OPUS = gtk_button_new_with_label("OPUS");
        gtk_widget_add_css_class(button_audio_format_audio_OPUS, "button_box_audio_video");
        gtk_box_append(GTK_BOX(box_audio_format_audio_nested), button_audio_format_audio_OPUS);
        gtk_widget_set_vexpand(button_audio_format_audio_OPUS, TRUE);

        GtkWidget *button_audio_format_audio_WAV = gtk_button_new_with_label("WAV");
        gtk_widget_add_css_class(button_audio_format_audio_WAV, "button_box_audio_video");
        gtk_box_append(GTK_BOX(box_audio_format_audio_nested), button_audio_format_audio_WAV);
        gtk_widget_set_vexpand(button_audio_format_audio_WAV, TRUE);


        gtk_widget_set_visible(box, FALSE);
    }
    void create_right_scrolled_window_video(){
        GtkWidget *box = main_widgets->box_video;
        
        GtkWidget *box_video_max_resolution = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        gtk_box_append(GTK_BOX(box), box_video_max_resolution);

        GtkWidget *label_video_max_resolution = gtk_label_new(local->title_box_info.max_resolution);
        gtk_box_append(GTK_BOX(box_video_max_resolution), label_video_max_resolution);
        gtk_widget_add_css_class(label_video_max_resolution, "label_box_audio_video");

        GtkWidget *scrolled_window_video_max_resolution = gtk_scrolled_window_new();
        
        gtk_widget_set_vexpand(scrolled_window_video_max_resolution, TRUE);

        gtk_box_append(GTK_BOX(box_video_max_resolution), scrolled_window_video_max_resolution);

        GtkWidget *box_video_max_resolution_nested = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        
        gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window_video_max_resolution), box_video_max_resolution_nested);

        GtkWidget *button_video_max_resolution_4K = gtk_button_new_with_label("4K");
        gtk_widget_add_css_class(button_video_max_resolution_4K, "button_box_audio_video");
        gtk_box_append(GTK_BOX(box_video_max_resolution_nested), button_video_max_resolution_4K);
        gtk_widget_set_vexpand(button_video_max_resolution_4K, TRUE);

        GtkWidget *button_video_max_resolution_1440p = gtk_button_new_with_label("1440p");
        gtk_widget_add_css_class(button_video_max_resolution_1440p, "button_box_audio_video");
        gtk_box_append(GTK_BOX(box_video_max_resolution_nested), button_video_max_resolution_1440p);
        gtk_widget_set_vexpand(button_video_max_resolution_1440p, TRUE);

        GtkWidget *button_video_max_resolution_1080p = gtk_button_new_with_label("1080p");
        gtk_widget_add_css_class(button_video_max_resolution_1080p, "button_box_audio_video");
        gtk_box_append(GTK_BOX(box_video_max_resolution_nested), button_video_max_resolution_1080p);
        gtk_widget_set_vexpand(button_video_max_resolution_1080p, TRUE);

        GtkWidget *button_video_max_resolution_720p = gtk_button_new_with_label("720p");
        gtk_widget_add_css_class(button_video_max_resolution_720p, "button_box_audio_video");
        gtk_box_append(GTK_BOX(box_video_max_resolution_nested), button_video_max_resolution_720p);
        gtk_widget_set_vexpand(button_video_max_resolution_720p, TRUE);

        GtkWidget *button_video_max_resolution_480p = gtk_button_new_with_label("480p");
        gtk_widget_add_css_class(button_video_max_resolution_480p, "button_box_audio_video");
        gtk_box_append(GTK_BOX(box_video_max_resolution_nested), button_video_max_resolution_480p);
        gtk_widget_set_vexpand(button_video_max_resolution_480p, TRUE);

        GtkWidget *button_video_max_resolution_360p = gtk_button_new_with_label("360p");
        gtk_widget_add_css_class(button_video_max_resolution_360p, "button_box_audio_video");
        gtk_box_append(GTK_BOX(box_video_max_resolution_nested), button_video_max_resolution_360p);
        gtk_widget_set_vexpand(button_video_max_resolution_360p, TRUE);

        GtkWidget *button_video_max_resolution_240p = gtk_button_new_with_label("240p");
        gtk_widget_add_css_class(button_video_max_resolution_240p, "button_box_audio_video");
        gtk_box_append(GTK_BOX(box_video_max_resolution_nested), button_video_max_resolution_240p);
        gtk_widget_set_vexpand(button_video_max_resolution_240p, TRUE);

        GtkWidget *button_video_max_resolution_144p = gtk_button_new_with_label("144p");
        gtk_widget_add_css_class(button_video_max_resolution_144p, "button_box_audio_video");
        gtk_box_append(GTK_BOX(box_video_max_resolution_nested), button_video_max_resolution_144p);
        gtk_widget_set_vexpand(button_video_max_resolution_144p, TRUE);

        // Выбор формата видео
        GtkWidget *box_video_format_video = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        gtk_box_append(GTK_BOX(box), box_video_format_video);

        GtkWidget *label_video_format_video = gtk_label_new(local->title_box_info.format_video);
        gtk_box_append(GTK_BOX(box_video_format_video), label_video_format_video);
        
        gtk_widget_add_css_class(label_video_format_video, "label_box_audio_video");

        GtkWidget *scrolled_window_video_format_video = gtk_scrolled_window_new();
        gtk_widget_set_vexpand(scrolled_window_video_format_video, TRUE);

        gtk_box_append(GTK_BOX(box_video_format_video), scrolled_window_video_format_video);

        GtkWidget *box_video_format_video_nested = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

        gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window_video_format_video), box_video_format_video_nested);

        GtkWidget *button_video_format_video_3GP = gtk_button_new_with_label("3GP");
        gtk_widget_add_css_class(button_video_format_video_3GP, "button_box_audio_video");
        gtk_box_append(GTK_BOX(box_video_format_video_nested), button_video_format_video_3GP);
        gtk_widget_set_vexpand(button_video_format_video_3GP, TRUE);

        GtkWidget *button_video_format_video_FLV = gtk_button_new_with_label("FLV");
        gtk_widget_add_css_class(button_video_format_video_FLV, "button_box_audio_video");
        gtk_box_append(GTK_BOX(box_video_format_video_nested), button_video_format_video_FLV);
        gtk_widget_set_vexpand(button_video_format_video_FLV, TRUE);

        GtkWidget *button_video_format_video_MP4 = gtk_button_new_with_label("MP4");
        gtk_widget_add_css_class(button_video_format_video_MP4, "button_box_audio_video");
        gtk_box_append(GTK_BOX(box_video_format_video_nested), button_video_format_video_MP4);
        gtk_widget_set_vexpand(button_video_format_video_MP4, TRUE);

        GtkWidget *button_video_format_video_WEBM = gtk_button_new_with_label("WEBM");
        gtk_widget_add_css_class(button_video_format_video_WEBM, "button_box_audio_video");
        gtk_box_append(GTK_BOX(box_video_format_video_nested), button_video_format_video_WEBM);
        gtk_widget_set_vexpand(button_video_format_video_WEBM, TRUE);

        GtkWidget *button_video_format_video_MKV = gtk_button_new_with_label("MKV");
        gtk_widget_add_css_class(button_video_format_video_MKV, "button_box_audio_video");
        gtk_box_append(GTK_BOX(box_video_format_video_nested), button_video_format_video_MKV);
        gtk_widget_set_vexpand(button_video_format_video_MKV, TRUE);


        gtk_widget_set_visible(box, FALSE);
    }
    void box_audio_vidio_style_start(){
        apply_css("button.button_box_audio_video{border-radius: 0px; font-size: 30px;} label.label_box_audio_video{background-color: rgba(0, 0, 0, 0.2); color: rgb(255, 255, 255); font-size: 22px;} label.label_box_audio_video:hover{background-color: rgba(0, 0, 0, 0.1); color: rgb(0, 0, 0);}");
    }
    void button_style_start(){
        apply_css("button { background: rgba(0,0,0,0.3); color: rgb(255,255,255); border: 0px; text-shadow: none; box-shadow: none; transition: background 0.3s ease;} button:active { background: rgba(0,0,0,0);} button:hover {background: rgba(0,0,0,0.2); color: rgb(0, 0, 0);}");
    }
    void start() {
        locale_s();
        std::cout<<"kl iuh"<<std::endl;
        window_s();
        cretion_titlebar();
        cretion_main_grids();
        create_center_grid();
        button_style_start();
        create_bottom_scrolled_window_info();
        create_left_scrolled_window_audio();
        create_right_scrolled_window_video();
        box_audio_vidio_style_start();
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

