//  g++ -g -o i main1.cpp `pkg-config --cflags --libs gtk4` -lcurl


#include <gtk/gtk.h>
#include <iostream>
#include <array>
#include <string>
#include <memory>
#include <stdexcept>
#include <cstdio>
#include <vector>
#include <ctime>
#include <thread>
#include <regex>
#include <iomanip>
#include <sstream>
#include <future>
#include <cairo.h>


std::string directory_explorer = "~/";

std::string vidio_format = "mp4";

std::string audio_format = "mp3";

std::string vidio_max_resolution = "137";

std::string audio_quality = "2";

bool playlist = false;

bool open_audio_panel = false;

bool open_video_panel = false;

bool open_info_panel = false;


// Показ оставшихся процентов      || Окно  || Кнопка аудио || Кнопка видео||Сылка на видео
GtkWidget *status_download_titlebar, *window, *button_audio, *button_video, *entry_url;
// Random fons for window
std::vector<std::string> fons_calor = { "#99FF18", "#FFF818", "#FFA918", "#FF6618", "#FF2018", "#FF1493", "#FF18C9", "#CB18FF", "#9118FF", "#5C18FF", "#1F75FE", "#00BFFF", "#18FFE5", "#00FA9A", "#00FF00", "#7FFF00", "#CEFF1D"};

struct button_name_sound_quality {
    const char *best, *good, *medium, *low, *tough;
};

struct title_box {
    const char *sound_quality, *format_audio, *max_resolution, *format_video;
};

struct button {
    const char *audio, *video, *change_background, *open_side_panel, *close_side_panel, *download_video, *download_audio, *url_youtube, *download, *ready, *explorer, *playlist;
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

struct struct_button_box_audio_or_video{
	std::string *pdata, data, new_element, number_element_box;
};

struct bottom_scrolled_window_struct{
	GtkWidget *box_widget, *label_widget;
};

bottom_scrolled_window_struct *bottom_scrolled_window;

struct name_image_url{
	std::string video_name, image_url;
};

// Функция для загрузки изображения и масштабирования с использованием Cairo
cairo_surface_t* load_and_scale_image_with_cairo(const std::string& file_path, int width, int height) {
    GError *error = nullptr;

    // Загружаем изображение как GdkPixbuf
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(file_path.c_str(), &error);
    if (!pixbuf) {
        g_printerr("Error loading image: %s\n", error->message);
        g_clear_error(&error);
        return nullptr;
    }

    // Получаем оригинальные размеры изображения
    int original_width = gdk_pixbuf_get_width(pixbuf);
    int original_height = gdk_pixbuf_get_height(pixbuf);

    // Создаем поверхность Cairo для рисования
    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
    cairo_t *cr = cairo_create(surface);

    // Устанавливаем метод интерполяции для масштабирования
    cairo_scale(cr, (double)width / original_width, (double)height / original_height);

    // Рисуем изображение с помощью Cairo
    gdk_cairo_set_source_pixbuf(cr, pixbuf, 0, 0);
    cairo_paint(cr);

    // Освобождаем ресурсы
    cairo_destroy(cr);
    g_object_unref(pixbuf);

    return surface;
}

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
	    apply_css(("." + button + " {background: rgba(0,0,0,0.3);} ." + button + ":hover {background: rgba(0,0,0,0.2);}").c_str());
    }

    static void selected_item(std::string button) {
	    apply_css(("." + button + " {background: rgba(0,0,0,0.5);} ." + button + ":hover {background: rgba(0,0,0,0.6);}").c_str());
    }
};

class LocalS{
public:
    void locale_s(const char *locale_languages = setlocale(LC_ALL, "")){
        if (strncmp(locale_languages, "ru", 2) == 0){
            button btn_info = {"Аудио", "Видео", "Смена фона", "Открыть боковую панель", "Закрыть боковую панель", "Скачать видио", "Скачать аудио", "Ссылка на видио с youtube", "Скачать!", "Готово!", "Папка?", "П.лист?"};
            error err_info = {"Вы не ввели ссылку на видио!!!", "Неверная ссылка на видио!!!", "Невыбран формат для видио"};
            title_box titb_info = {"Качество\nзвука", "Формат\nАудио", "Макс.\nразрешение", "Формат\nвидео"};
            button_name_sound_quality but_name_sound_qual_info = {"Лучшее", "Хорошее", "Средние", "Низкое", "Ужастное"};
            local = new local_struct{"Вы ничего не скачиваете!!!", "ru", "logos/local_ru.png", "Скачивальщик видио с YouTube!!!", btn_info, err_info, titb_info, but_name_sound_qual_info};
        }else{
            button btn_info = {"Audio", "   Video   ", "Change background", "Open side panel", "Close side panel", "Download video", "Download audio", "Link to video from youtube", "Download!", "Ready!", "Directory?", "P.list?"};
            error err_info = {"You have not entered a link to the video!!!", "Incorrect link to the video!!!", "No format selected for the video"};
            title_box titb_info= {"Sound\nquality", "Format\naudio", "Max\nresolution", "Format\nvideo"};
            button_name_sound_quality but_name_sound_qual_info = {"Best", "Good", "Medium", "Low", "Tough"};
            local = new local_struct{"You are not downloading anything!!!", "en", "logos/local_en.png", "YouTube video downloader!!!", btn_info, err_info, titb_info, but_name_sound_qual_info};
        }
    }
};

class DownloaderYT{
public:
	static void download_update(GtkWidget *box, const char *percent, const char *size, const char *speed, const char *eta){


		/*
		if (percent != "100"){
			
		} else {
			gtk_button_set_label(GTK_BUTTON(status_download_titlebar), local->button_info.ready);
		}
		*/
	}
	static name_image_url* exec(const char* cmd) {
		std::array<char, 128> buffer;
		std::string result;
		FILE* pipe = popen(cmd, "r");
		if (!pipe) {
			throw std::runtime_error("popen() failed!");
		}
		try {
			while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
				result += buffer.data();
			}
		} catch (...) {
			pclose(pipe);
			throw;
		}
		pclose(pipe);
		std::string name, image_url;
		std::cout<<result<<std::endl;
		
		for (int i = size(result) - 2; true; i--){
			if (result[i] == '\n'){
				std::cout<<"jkkvnjnsdf"<<std::endl;
				image_url = result.substr(i + 1, size(result) - 2 - i);
				name = result.substr(0, i);
				break;
			}
		}
		std::cout<<"image_url    "<<image_url<<std::endl;
		
		return new name_image_url{name, image_url};
	}

	static name_image_url* get_title_video_and_url_image(std::string url) {
		std::string command = "yt-dlp --get-title --get-thumbnail --skip-download " + url;

		try {
			//   std::thread t(exec, command.c_str());
			//   name_image_url output = t.detach();
			// name_image_url output = exec(command.c_str());
			//   return output;
			std::packaged_task<name_image_url*()> task([command]{
				return exec(command.c_str());
			});
			std::future<name_image_url*> future = task.get_future();
			std::thread t(std::move(task));
			t.detach();
			return future.get(); // Блокирующий вызов, ждёт завершения задачи
		} catch (const std::exception& e) {
			std::cerr << "Error: " << e.what() << std::endl;
			return new name_image_url{"Название видео", "url image"};
		}
	}

    static void download_yt(const char *url, const char *name_video){
        std::string playlist_command, format_audio_or_video;
        if (playlist){
            playlist_command = "--yes-playlist";
        } else {
            playlist_command = "--no-playlist";
        }

        if (open_audio_panel){            
            format_audio_or_video = " --extract-audio --audio-format " + audio_format + " --audio-quality " + audio_quality;
        } else if (open_video_panel){
            format_audio_or_video = " --merge-output-format " + vidio_format + " --format " + vidio_max_resolution;
        }
        
        if (url != "") {
            std::string command = "yt-dlp --no-warnings --newline " + playlist_command + format_audio_or_video + " --output \"" + directory_explorer + "%(title)s.%(ext)s\" \"" + url + "\"";
            
            std::cout<<command<<std::endl;
            // Запуск команды для загрузки файла
            FILE* pipe = popen(command.c_str(), "r");
            if (!pipe) {
                std::cerr << "Ошибка выполнения команды." << std::endl;
                return;
            }

            // Переменные для получения информации о загрузке аудио или видео
            char buffer[256];
            std::regex regexPattern(R"(\[download\]\s+(\d+\.\d+)% of\s+(\d+\.\d+[KMG]iB) at\s+([\d\.]+[KMG]iB/s) ETA (\d+:\d+))");
	        std::smatch match;
			GtkWidget *grid_download = gtk_grid_new();
			gtk_widget_set_vexpand(grid_download, TRUE);
			gtk_widget_set_hexpand(grid_download, TRUE);
			GtkWidget *persent_label = gtk_label_new("0%");
			gtk_widget_set_vexpand(persent_label, TRUE);
			gtk_widget_set_hexpand(persent_label, TRUE);
			gtk_grid_attach(GTK_GRID(grid_download), persent_label, 0, 0, 4, 2);
			GtkWidget *scrolled_window_title_name = gtk_scrolled_window_new();
			gtk_widget_set_vexpand(scrolled_window_title_name, TRUE);
			gtk_widget_set_hexpand(scrolled_window_title_name, TRUE);
			gtk_grid_attach(GTK_GRID(grid_download), scrolled_window_title_name, 4, 0, 20, 1);
			GtkWidget *label_title_name = gtk_label_new(name_video);
			gtk_widget_set_vexpand(label_title_name, TRUE);
			gtk_widget_set_hexpand(label_title_name, TRUE);
			gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scrolled_window_title_name), label_title_name);

			GtkWidget *label_size = gtk_label_new("0");
			gtk_widget_set_vexpand(label_size, TRUE);
			gtk_widget_set_hexpand(label_size, TRUE);
			gtk_grid_attach(GTK_GRID(grid_download), label_size, 4, 1, 5, 1);
			GtkWidget *label_size_install = gtk_label_new("0");
			gtk_widget_set_vexpand(label_size_install, TRUE);
			gtk_widget_set_hexpand(label_size_install, TRUE);
			gtk_grid_attach(GTK_GRID(grid_download), label_size_install, 9, 1, 5, 1);
			GtkWidget *label_speed = gtk_label_new("0");
			gtk_widget_set_vexpand(label_speed, TRUE);
			gtk_widget_set_hexpand(label_speed, TRUE);
			gtk_grid_attach(GTK_GRID(grid_download), label_speed, 14, 1, 5, 1);
			GtkWidget *label_eta = gtk_label_new("0");
			gtk_widget_set_vexpand(label_eta, TRUE);
			gtk_widget_set_hexpand(label_eta, TRUE);
			gtk_grid_attach(GTK_GRID(grid_download), label_eta, 19, 1, 5, 1);
			gtk_box_append(GTK_BOX(bottom_scrolled_window->box_widget), grid_download);
			gtk_widget_set_visible(bottom_scrolled_window->label_widget, FALSE);
            
            // Цикл для обновления информации о загрузке аудио или видео
            while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
	            std::cout<<"edsd"<<std::endl;
                std::string line(buffer);
	            std::cout<<buffer<<std::endl;

				
                if (std::regex_search(line, match, regexPattern)) {
					//download_update(label, match[1], match[2], math[3], match[4]);
					gtk_label_set_text(GTK_LABEL(persent_label), match[1].str().c_str());
					gtk_label_set_text(GTK_LABEL(label_size), match[2].str().c_str());
					const char *match5 = match[2].str().c_str();
					std::string match51 = std::string((char*)(match5 + strlen(match5) - 3));
					std::cout<<match51<<std::endl;
					std::string match54 = std::to_string(std::stof(match5) / std::stof(match[1]) * 100);
					for (int i = 0; true; i++){
						if (match54[i] != '.'){
							match54 = match54.substr(0, i + 2);
							break;
						}
					}
					const char *match52 = (match54 + match51).c_str();
					gtk_label_set_text(GTK_LABEL(label_size_install), match52);
					gtk_label_set_text(GTK_LABEL(label_speed), match[3].str().c_str());
					gtk_label_set_text(GTK_LABEL(label_eta), match[4].str().c_str());
					/*
					const char *percent = match[1].c_str();
                    std::string size = match[2].c_str();
                    std::string speed = match[3].c_str();
                    std::string eta = match[4].c_str();

					

                    std::cout << "Процент: " << percent << "%, "
                              << "Размер: " << size << ", "
                              << "Скорость: " << speed << ", "
                              << "Оставшееся время: " << eta << std::endl;
					*/
                }
            }
            // Закрытие команды
            pclose(pipe);

        }
    }
};

class CallbackTitlebarButton : public EditWindow{
public:
    // Функция-обработчик для события изменения размеров окна
    static void on_window_height_changed(GObject *object = G_OBJECT(window), GParamSpec *pspec = NULL, gpointer data = NULL) {
        int height, width;
        gtk_window_get_default_size(GTK_WINDOW(object), &width, &height);
        std::cout << "New window size: " << height << std::endl;
        if (height <= 600 || width <= 840){
            apply_css("button.box_audio_video{font-size: 18px;} label.label_box_audio_video{font-size: 22px;} label.label_box_button{font-size: 30px;}");
        }else if (width >= 980){
            apply_css("button.box_audio_video{font-size: 45px;} label.label_box_audio_video{font-size: 25px;} label.label_box_button{font-size: 55px;}");
        }else{
            apply_css("button.box_audio_video{font-size: 30px;} label.label_box_audio_video{font-size: 22px;} label.label_box_button{font-size: 45px;}");
        }
    }
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
            on_window_height_changed();
        } else {
            apply_css("button.box_audio_video{font-size: 45px;} label.label_box_audio_video{font-size: 40px;} label.label_box_button{font-size: 45px;}");
            gtk_window_maximize(GTK_WINDOW(window));
        }
    }
};

class PressingButton : public CallbackTitlebarButton {
public:
    static void new_value_for_variables(GtkWidget *widget, gpointer user_data) {
    	struct_button_box_audio_or_video *data = (struct_button_box_audio_or_video *)user_data;
		*data->pdata = data->data;
		std::cout<<data->data<<std::endl;
		std::cout<<"kgddsjjcv"<<std::endl;
        apply_css(("button.box_audio_video_" + data->number_element_box + "{background-color: rgba(0, 0, 0, 0.3); color: rgb(255, 255, 255);} button.box_audio_video_" + data->number_element_box + ":hover{background-color: rgba(0, 0, 0, 0.2); color: rgb(0, 0, 0);} button." + data->new_element + "_box_audio_video_" + data->number_element_box + "{background-color: rgba(0, 0, 0, 0.5); color: rgb(0, 0, 0);} button." + data->new_element + "_box_audio_video_" + data->number_element_box + ":hover{background-color: rgba(0, 0, 0, 0.7); color: rgb(255, 255, 255);}").c_str());
    }
	

    // Функция обновления UI в главном потоке
	static gboolean update_ui(gpointer data) {
		name_image_url* name_image_url_s = static_cast<name_image_url*>(data);
		std::cout<<"bjksv\t"<<name_image_url_s->image_url<<std::endl;

		GtkWidget *window_video = gtk_window_new();
		GtkWidget *box_video_window = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
		gtk_window_set_child(GTK_WINDOW(window_video), box_video_window);

		GtkWidget *button_preview = gtk_button_new();
		gtk_box_append(GTK_BOX(box_video_window), button_preview);
		/*
		GtkWidget *image_preview = gtk_image_new_from_file(name_image_url_s->image_url.c_str());
        gtk_widget_set_hexpand(image_preview, TRUE);
        gtk_widget_set_vexpand(image_preview, TRUE);
		gtk_button_set_child(GTK_BUTTON(button_preview), image_preview);
		*/
		// Загрузить и масштабировать изображение с помощью Cairo
		// Загрузить и масштабировать изображение с помощью Cairo
		cairo_surface_t *surface = load_and_scale_image_with_cairo(name_image_url_s->image_url, 635, 360);

		if (surface) {
			// Создать GtkPicture с использованием поверхности Cairo
			GtkPicture *picture = GTK_PICTURE(gtk_picture_new_for_surface(surface));

			// Освобождаем поверхность Cairo после использования
			cairo_surface_destroy(surface);

			// Создаем GtkWidget из GtkPicture
			GtkWidget *image_preview = GTK_WIDGET(picture);

			// Устанавливаем параметры расширения
			gtk_widget_set_hexpand(image_preview, TRUE);
			gtk_widget_set_vexpand(image_preview, TRUE);
			gtk_box_append(GTK_BOX(box_video_window), image_preview);
		} else {
			g_printerr("Failed to create scaled image using Cairo.\n");
		}

		gtk_box_append(GTK_BOX(box_video_window), gtk_label_new(name_image_url_s->video_name.c_str()));
		GtkWidget *box_video_window_button = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
		gtk_box_append(GTK_BOX(box_video_window), box_video_window_button);
		GtkWidget *button_video_download = gtk_button_new();
		gtk_box_append(GTK_BOX(box_video_window_button), button_video_download);
		GtkWidget *button_video_cancel = gtk_button_new();
		gtk_box_append(GTK_BOX(box_video_window_button), button_video_cancel);
		gtk_window_present(GTK_WINDOW(window_video));

		// Освобождаем данные
		delete name_image_url_s;

		return FALSE; // Возвращаем FALSE, чтобы функция не вызывалась снова
	}

	static std::string download_image(std::string url_image){
		std::string name_file, name_file_file_extension, file_extension;
		std::cout<<"vffffji"<<url_image<<std::endl;
		int last_slash = 0;
		for (int i = size(url_image) - 1; true; i--){
			std::cout<<url_image[i]<<std::endl;
			if (url_image[i] == '/'){
				if (last_slash == 0){
					last_slash = i;
				}else{
					
					for (int ii = size(url_image) - 1; true; ii--){
						std::cout<<"l\t"<<ii<<std::endl;
						if (url_image[ii] == '.'){
							std::cout<<"l\t"<<ii<<std::endl;
							file_extension = url_image.substr(ii, size(url_image) - ii);
							break;
						}
					}
					name_file = url_image.substr(i + 1, last_slash - i - 1);
					name_file_file_extension = name_file + file_extension;
					break;
				}
			}
		}
		std::string pash_file = "previews/" + name_file + file_extension;
		std::string command = "wget --output-document=" + pash_file + " \"" + url_image + "\"";
		std::cout<<command<<std::endl;
		FILE* pipe = popen(command.c_str(), "r");
		if (!pipe) {
			std::cerr << "Ошибка выполнения команды." << std::endl;
		}
		// Закрытие команды
        pclose(pipe);

		std::string name_file_png = name_file + ".png";
		command = "ffmpeg -i " + pash_file + " previews/" + name_file_png;
		std::cout<<command<<std::endl;
		FILE* pipe1 = popen(command.c_str(), "r");
		if (!pipe1) {
			std::cerr << "Ошибка выполнения команды." << std::endl;
		}
		// Закрытие команды
        pclose(pipe1);
		
		
		return "previews/" + name_file_png;
	}

	// Фоновая функция для скачивания информации
	static void downloader_youtube_s(GtkWidget *widget, gpointer *data) {
		std::cout << "fdgss" << std::endl;
		const char *entry_text_c_str = gtk_editable_get_text(GTK_EDITABLE(GTK_ENTRY(entry_url)));

		std::string entry_text = entry_text_c_str ? std::string(entry_text_c_str) : "";
		name_image_url *name_image_url_s = DownloaderYT::get_title_video_and_url_image(entry_text);

		name_image_url_s->image_url = download_image(std::string(name_image_url_s->image_url));
		/*
		// Подготовка данных для обновления UI
		name_image_url* name_image_url_s = new name_image_url();
		name_image_url_s->image_url = name_image_url_s.image_url;
		name_image_url_s->video_name = name_image_url_s.video_name;
		*/
		// Передаем функцию обновления UI в главный поток
		g_idle_add(update_ui, name_image_url_s);

		/*
		std::thread t(DownloaderYT::download_yt, entry_text_c_str, name_image_url_s.video_name.c_str());
		t.detach();
		*/
	}

	static void downloader_youtube(GtkWidget *widget, gpointer *data){
		std::thread t(downloader_youtube_s, widget, data);
		t.detach();
	}

    static void is_playlist(GtkWidget *widget, bool *pdata){
        if (*pdata) {
            apply_css("button.playlist{background-color: rgba(0, 0, 0, 0.3); color: rgb(255, 255, 255);} button.playlist:hover{background-color: rgba(0, 0, 0, 0.2); color: rgb(0, 0, 0);}");
        }else{
            apply_css("button.playlist{background-color: rgba(0, 0, 0, 0.5); color: rgb(0, 0, 0);} button.playlist:hover{background-color: rgba(0, 0, 0, 0.7); color: rgb(255, 255, 255);}");
        }
        *pdata = !*pdata;
    }
    
    static void open_explorer_thread(GtkWidget *widget, bool *data){
		if (directory_explorer == "~/") {
        	std::string command = "zenity --file-selection --directory 2>/dev/null";
        	char buffer[1024];
    
        	// Открытие диалогового окна и получение выбранного пути
        	FILE* pipe = popen(command.c_str(), "r");
    
    
        	// Чтение пути, который вернул Zenity
        	if (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
				std::string selected_path(buffer);
				apply_css("button.explorer{background-color: rgba(0, 0, 0, 0.5); color: rgb(0, 0, 0);} button.explorer:hover{background-color: rgba(0, 0, 0, 0.7); color: rgb(255, 255, 255);}");
			
				// Удаляем символ новой строки, если он есть
				if (!selected_path.empty() && selected_path.back() == '\n') {
					selected_path.pop_back();
				}
			
				directory_explorer = selected_path;
			}
		
			pclose(pipe);
			std::cout<<directory_explorer<<std::endl;
		}else{
			directory_explorer = "~/";
			apply_css("button.explorer{background-color: rgba(0, 0, 0, 0.3); color: rgb(255, 255, 255);} button.explorer:hover{background-color: rgba(0, 0, 0, 0.2); color: rgb(0, 0, 0);}");
		}
    }
    static void open_explorer(GtkWidget *widget, bool *pdata){
        std::thread t(open_explorer_thread, widget, pdata);
        t.detach();  // Отсоединяем поток
    }
    static void hiding_audio_panel(GtkWidget *widget, bool *pdata) {
        std::cout<<"fvsdfs"<<std::endl;
        
        int width_window = gtk_widget_get_width(window);
        int height_window = gtk_widget_get_height(window);
        std::cout<<"width_window "<<width_window<<"\theight_window "<<height_window<<std::endl;
        std::cout<<"*pdata\t"<<*pdata<<std::endl;
        if (open_video_panel) {
            std::cout<<"DVFDVDF"<<std::endl;
            gtk_widget_set_visible(main_widgets->box_video, FALSE);
            open_video_panel = false;
            width_window = std::round(width_window / 3 * 2);
        }
        apply_css((std::string("button.video{background-color: rgba(0, 0, 0, 0.3); color: rgb(255, 255, 255);} button.video:hover{background-color: rgba(0, 0, 0, 0.2); color: rgb(0, 0, 0);}") + std::string(*pdata ? "button.audio{background-color: rgba(0, 0, 0, 0.3); color: rgb(255, 255, 255);} button.audio:hover{background-color: rgba(0, 0, 0, 0.2); color: rgb(0, 0, 0);}" : "button.audio{background-color: rgba(0, 0, 0, 0.5); color: rgb(0, 0, 0);} button.audio:hover{background-color: rgba(0, 0, 0, 0.7); color: rgb(255, 255, 255);}")).c_str());
        std::cout<<"width_window "<<width_window<<"\theight_window "<<height_window<<std::endl;
        // gtk_window_set_default_size(GTK_WINDOW(window), *pdata ? width_window / 3 * 2 : width_window * 1.5, height_window);
        std::cout<<width_window<<"\t"<<*pdata<<std::endl;
        std::cout<<(*pdata ? std::round(width_window / 3 * 2) : width_window * 1.5)<<std::endl;
        
        gtk_widget_set_visible(main_widgets->box_audio, *pdata ? FALSE : TRUE);
        *pdata = !*pdata;
    }
    static void hiding_video_panel(GtkWidget *widget, bool *pdata) {
        std::cout<<"fvsdfs2"<<std::endl;
        int width_window = gtk_widget_get_width(window);
        int height_window = gtk_widget_get_height(window);
        std::cout<<"width_window "<<width_window<<"\theight_window "<<height_window<<std::endl;
        std::cout<<"*pdata\t"<<*pdata<<std::endl;
        if (open_audio_panel) {
            std::cout<<"hfjffkjfj"<<std::endl;
            gtk_widget_set_visible(main_widgets->box_audio, FALSE);
            open_audio_panel = false;
            width_window = std::round(width_window / 3 * 2);
        }
        apply_css((std::string("button.audio{background-color: rgba(0, 0, 0, 0.3); color: rgb(255, 255, 255);} button.audio:hover{background-color: rgba(0, 0, 0, 0.2); color: rgb(0, 0, 0);}") + std::string(*pdata ? "button.video{background-color: rgba(0, 0, 0, 0.3); color: rgb(255, 255, 255);} button.video:hover{background-color: rgba(0, 0, 0, 0.2); color: rgb(0, 0, 0);}" : "button.video{background-color: rgba(0, 0, 0, 0.5); color: rgb(0, 0, 0);} button.video:hover{background-color: rgba(0, 0, 0, 0.7); color: rgb(255, 255, 255);} ")).c_str());
        std::cout<<"width_window "<<width_window<<"\theight_window "<<height_window<<std::endl;
        // gtk_window_set_default_size(GTK_WINDOW(window), *pdata ? std::round(width_window / 3 * 2) : width_window * 1.5, height_window);
        gtk_widget_set_visible(main_widgets->box_video, *pdata ? FALSE : TRUE);
        *pdata = !*pdata;
    }
    
    static void hiding_bottom_panel(GtkWidget *widget, bool *pdata) {
        gtk_widget_set_visible(main_widgets->bottom_scrolled_window_info, *pdata ? FALSE : TRUE);
        *pdata = !*pdata;
    }
};

class InitStart : public LocalS, public PressingButton{
public:
   
    // Обработчик для пользовательской кнопки
    static void on_custom_button_clicked(GtkWidget *widget, gpointer data) {
        g_print("Custom button clicked!\n");
    }
    void window_s() {
		
        // Создаем окно
	    window = gtk_window_new();

        g_signal_connect(window, "notify::default-height", G_CALLBACK(on_window_height_changed), nullptr);
		g_signal_connect(window, "notify::default-width", G_CALLBACK(on_window_height_changed), nullptr);
        gtk_window_set_default_size(GTK_WINDOW(window), 630, 480);

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
                border-radius: 35px; 
                background-color: rgba(0, 0, 0, 0.1);  
            }

            button.window-main-button:hover {
                background-color: rgba(255, 255, 255, 0.1);
            })"
        );
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
        // gtk_widget_set_size_request(box_audio, 150, 300);
        gtk_widget_set_hexpand(box_audio, TRUE);
        gtk_widget_set_vexpand(box_audio, TRUE);

        GtkWidget *box_video = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
        gtk_box_append(GTK_BOX(main_box), box_video);
        gtk_widget_add_css_class(box_video, "main_widget");
        // gtk_widget_set_size_request(box_video, 150, 300);
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
        entry_url = gtk_entry_new();
        gtk_grid_attach(GTK_GRID(grid), entry_url, 0, 0, 2, 1);
        gtk_widget_set_hexpand(entry_url, TRUE);
        gtk_widget_set_vexpand(entry_url, TRUE);
        gtk_widget_add_css_class(entry_url, "entry_url");

        
        // Является ли эта ссылка ссылкой на плей лист
        GtkWidget *button_playlist = gtk_button_new_with_label("Пелейлист?");
        gtk_grid_attach(GTK_GRID(grid), button_playlist, 0, 1, 1, 1);
        gtk_widget_set_hexpand(button_playlist, TRUE);
        gtk_widget_set_vexpand(button_playlist, TRUE);
        gtk_widget_add_css_class(button_playlist, "button_center");
        gtk_widget_add_css_class(button_playlist, "playlist");

        g_signal_connect(button_playlist, "clicked", G_CALLBACK(is_playlist), &playlist);
        GtkWidget *playlist_box_button = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
        GtkWidget *playlist_image_box_button = gtk_image_new_from_file("logos/playlist.png");
        gtk_widget_set_hexpand(playlist_image_box_button, TRUE);
        gtk_widget_set_vexpand(playlist_image_box_button, TRUE);
        gtk_box_append(GTK_BOX(playlist_box_button), playlist_image_box_button);
        GtkWidget *playlist_label_box_button = gtk_label_new(local->button_info.playlist);
        gtk_widget_add_css_class(playlist_label_box_button, "label_box_button");
        gtk_widget_add_css_class(playlist_label_box_button, "playlist_label_box_button");
        gtk_widget_set_hexpand(playlist_label_box_button, TRUE);
        gtk_widget_set_vexpand(playlist_label_box_button, TRUE);
        gtk_box_append(GTK_BOX(playlist_box_button), playlist_label_box_button);
        gtk_button_set_child(GTK_BUTTON(button_playlist), playlist_box_button);
        gtk_widget_set_hexpand(playlist_box_button, TRUE);
        gtk_widget_set_vexpand(playlist_box_button, TRUE);

        // Выбор папки
        GtkWidget *button_explorer = gtk_button_new();
        gtk_grid_attach(GTK_GRID(grid), button_explorer, 1, 1, 1, 1);
        gtk_widget_set_hexpand(button_explorer, TRUE);
        gtk_widget_set_vexpand(button_explorer, TRUE);
        gtk_widget_add_css_class(button_explorer, "button_center");
        gtk_widget_add_css_class(button_explorer, "explorer");

        g_signal_connect(button_explorer, "clicked", G_CALLBACK(open_explorer), &directory_explorer);
        GtkWidget *explorer_box_button = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
        GtkWidget *explorer_image_box_button = gtk_image_new_from_file("logos/explorer.png");
        gtk_widget_set_hexpand(explorer_image_box_button, TRUE);
        gtk_widget_set_vexpand(explorer_image_box_button, TRUE);
        gtk_box_append(GTK_BOX(explorer_box_button), explorer_image_box_button);
        GtkWidget *explorer_label_box_button = gtk_label_new(local->button_info.explorer);
        gtk_widget_add_css_class(explorer_label_box_button, "label_box_button");
        gtk_widget_add_css_class(explorer_label_box_button, "explorer_label_box_button");
        gtk_widget_set_hexpand(explorer_label_box_button, TRUE);
        gtk_widget_set_vexpand(explorer_label_box_button, TRUE);
        gtk_box_append(GTK_BOX(explorer_box_button), explorer_label_box_button);
        gtk_button_set_child(GTK_BUTTON(button_explorer), explorer_box_button);
        gtk_widget_set_hexpand(explorer_box_button, TRUE);
        gtk_widget_set_vexpand(explorer_box_button, TRUE);
        
        
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

		g_signal_connect(button_downloader, "clicked", G_CALLBACK(downloader_youtube), NULL);

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


        apply_css("button.button_center{background-color: rgba(0, 0, 0, 0.3); border-radius: 0px;} entry.entry_url{border-radius: 0px; background: rgba(0,0,0,0.2); border: 0px;} entry.entry_url:hover{background: rgba(0,0,0,0.1);}");
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
		bottom_scrolled_window = new bottom_scrolled_window_struct{box, label};
    }
    void cretion_button_box_audio_or_vidio(std::string number_element_css, std::string param_button_css, const char *label_button, GtkWidget *box, std::string *pdata, std::string data){
        GtkWidget *button = gtk_button_new_with_label(label_button);
        gtk_widget_add_css_class(button, ("box_audio_video"));
        gtk_widget_add_css_class(button, ("box_audio_video_" + number_element_css).c_str());
        gtk_widget_add_css_class(button, (param_button_css + "_box_audio_video_" + number_element_css).c_str());
		struct_button_box_audio_or_video *data_struct = new struct_button_box_audio_or_video{pdata, data, param_button_css, number_element_css};
		g_signal_connect(button, "clicked", G_CALLBACK(new_value_for_variables), data_struct);
        gtk_box_append(GTK_BOX(box), button);
        gtk_widget_set_vexpand(button, TRUE);
    }
    void create_scrolled_window_audio(){
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
        // Button Best
        cretion_button_box_audio_or_vidio("1", "best", local->button_name_sound_quality_info.best, box_audio_sound_quality_nested, &audio_quality, "0");
        // Button good
        cretion_button_box_audio_or_vidio("1", "good", local->button_name_sound_quality_info.good, box_audio_sound_quality_nested, &audio_quality, "2");
        // Button medium
        cretion_button_box_audio_or_vidio("1", "medium", local->button_name_sound_quality_info.medium, box_audio_sound_quality_nested, &audio_quality, "5"); 
        // Button low
        cretion_button_box_audio_or_vidio("1", "low", local->button_name_sound_quality_info.low, box_audio_sound_quality_nested, &audio_quality, "7");
		// Button tough
		cretion_button_box_audio_or_vidio("1", "tough", local->button_name_sound_quality_info.tough, box_audio_sound_quality_nested, &audio_quality, "9");

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
        // Button format aac
        cretion_button_box_audio_or_vidio("2", "acc", "ACC", box_audio_format_audio_nested, &audio_format, "acc");
        // Button format ogg
        cretion_button_box_audio_or_vidio("2", "ogg", "OGG", box_audio_format_audio_nested, &audio_format, "ogg");


        // Button format mp3
        cretion_button_box_audio_or_vidio("2", "mp3", "MP3", box_audio_format_audio_nested, &audio_format, "mp3");
        // Button format opus
        cretion_button_box_audio_or_vidio("2", "opus", "OPUS", box_audio_format_audio_nested, &audio_format, "opus");
        // Button format wav
        cretion_button_box_audio_or_vidio("2", "wav", "WAV", box_audio_format_audio_nested, &audio_format, "wav");

        // Invisible All box audio
        gtk_widget_set_visible(box, FALSE);
    }
    void create_scrolled_window_video(){
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
		// Button resolution 4k
        cretion_button_box_audio_or_vidio("3", "r8k", "8K", box_video_max_resolution_nested, &vidio_max_resolution, "571");
        // Button resolution 4k
        cretion_button_box_audio_or_vidio("3", "r4k", "4K", box_video_max_resolution_nested, &vidio_max_resolution, "313");
        // Button resolution 1440p
        cretion_button_box_audio_or_vidio("3", "r1440p", "1440p", box_video_max_resolution_nested, &vidio_max_resolution, "217");
        // Button resolution 1080p
        cretion_button_box_audio_or_vidio("3", "r1080p", "1080p", box_video_max_resolution_nested, &vidio_max_resolution, "137");
        // Button resolution 720p
        cretion_button_box_audio_or_vidio("3", "r720p", "720p", box_video_max_resolution_nested, &vidio_max_resolution, "136");
        // Button resolution 480p
        cretion_button_box_audio_or_vidio("3", "r480p", "480p", box_video_max_resolution_nested, &vidio_max_resolution, "135");
        // Button resolution 360p
        cretion_button_box_audio_or_vidio("3", "r360p", "360p", box_video_max_resolution_nested, &vidio_max_resolution, "134");
        // Button resolution 240p
        cretion_button_box_audio_or_vidio("3", "r240p", "240p", box_video_max_resolution_nested, &vidio_max_resolution, "133");
		// Button resolution 160p
        cretion_button_box_audio_or_vidio("3", "r160p", "160p", box_video_max_resolution_nested, &vidio_max_resolution, "17");
        // Button resolution 144p
        cretion_button_box_audio_or_vidio("3", "r144p", "144p", box_video_max_resolution_nested, &vidio_max_resolution, "160");

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
        // Button format 3gp
        cretion_button_box_audio_or_vidio("4", "r3gp", "3GP", box_video_format_video_nested, &vidio_format, "3gp");
        // Button format flv
        cretion_button_box_audio_or_vidio("4", "flv", "FLV", box_video_format_video_nested, &vidio_format, "flv");
        // Button format mp4
        cretion_button_box_audio_or_vidio("4", "mp4", "MP4", box_video_format_video_nested, &vidio_format, "mp4");
        // Button format webm
        cretion_button_box_audio_or_vidio("4", "webm", "WEBM", box_video_format_video_nested, &vidio_format, "webm");
        // Button format mkv
        cretion_button_box_audio_or_vidio("4", "mkv", "MKV", box_video_format_video_nested, &vidio_format, "mkv");
        // Invisible all box video
        gtk_widget_set_visible(box, FALSE);
    }
    void button_style_start(){
        apply_css("button { background: rgba(0,0,0,0.3); color: rgb(255,255,255); border: 0px; text-shadow: none; box-shadow: none; transition: background 0.3s ease;} button:active { background: rgba(0,0,0,0);} button:hover {background: rgba(0,0,0,0.2); color: rgb(0, 0, 0);}");
    }
    void box_audio_video_default(){
        apply_css("button.box_audio_video{border-radius: 0px;} label.label_box_audio_video{background-color: rgba(0, 0, 0, 0.2); color: rgb(255, 255, 255);} label.label_box_audio_video:hover{background-color: rgba(0, 0, 0, 0.1); color: rgb(0, 0, 0);} button.good_box_audio_video_1{background-color: rgba(0, 0, 0, 0.5); color: rgb(0, 0, 0);} button.good_box_audio_video_1:hover{background-color: rgba(0, 0, 0, 0.7); color: rgb(255, 255, 255);} button.mp3_box_audio_video_2{background-color: rgba(0, 0, 0, 0.5); color: rgb(0, 0, 0);} button.mp3_box_audio_video_2:hover{background-color: rgba(0, 0, 0, 0.7); color: rgb(255, 255, 255);} button.r1080p_box_audio_video_3{background-color: rgba(0, 0, 0, 0.5); color: rgb(0, 0, 0);} button.r1080p_box_audio_video_3:hover{background-color: rgba(0, 0, 0, 0.7); color: rgb(255, 255, 255);} button.mp4_box_audio_video_4{background-color: rgba(0, 0, 0, 0.5); color: rgb(0, 0, 0);} button.mp4_box_audio_video_4:hover{background-color: rgba(0, 0, 0, 0.7); color: rgb(255, 255, 255);}");
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
        create_scrolled_window_audio();
        create_scrolled_window_video();
        box_audio_video_default();
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

