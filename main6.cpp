#include <gtk/gtk.h>
#include <curl/curl.h>
#include <vector>
#include <iostream>

// Структура для хранения загруженных данных
struct MemoryStruct {
    std::vector<unsigned char> data;
};

// Callback функция для записи загруженных данных
static size_t WriteMemoryCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t total_size = size * nmemb;
    MemoryStruct* mem = static_cast<MemoryStruct*>(userp);
    unsigned char* data = static_cast<unsigned char*>(contents);

    // Добавляем загруженные данные в вектор
    mem->data.insert(mem->data.end(), data, data + total_size);

    return total_size;
}

// Функция для загрузки изображения с помощью libcurl
bool download_image(const std::string& url, MemoryStruct& chunk) {
    CURL* curl_handle;
    CURLcode res;

    // Инициализация curl
    curl_global_init(CURL_GLOBAL_ALL);
    curl_handle = curl_easy_init();

    if (curl_handle) {
        curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void*)&chunk);
        curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

        // Выполняем запрос
        res = curl_easy_perform(curl_handle);

        // Проверяем ошибки
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            curl_easy_cleanup(curl_handle);
            return false;
        }

        // Очистка
        curl_easy_cleanup(curl_handle);
    }

    curl_global_cleanup();
    return true;
}

// Функция для создания главного окна приложения и отображения изображения
static void activate(GtkApplication* app, gpointer user_data) {
    // URL изображения
    std::string url = "https://i.ytimg.com/vi/zhE4Uq9Ayc8/maxresdefault.jpg";
    MemoryStruct chunk;

    // Загружаем изображение
    if (!download_image(url, chunk)) {
        std::cerr << "Failed to download image." << std::endl;
        return;
    }

    // Создаем буфер GdkPixbuf из загруженных данных
    GdkPixbufLoader* loader = gdk_pixbuf_loader_new();
    if (!gdk_pixbuf_loader_write(loader, chunk.data.data(), chunk.data.size(), NULL)) {
        std::cerr << "Failed to load image data into GdkPixbufLoader." << std::endl;
        g_object_unref(loader);
        return;
    }

    GdkPixbuf* pixbuf = gdk_pixbuf_loader_get_pixbuf(loader);
    gdk_pixbuf_loader_close(loader, NULL);

    // Преобразуем GdkPixbuf в GdkTexture
    GdkTexture* texture = gdk_texture_new_for_pixbuf(pixbuf);

    // Создаем окно GTK
    GtkWidget* window = gtk_application_window_new(app);
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);
    gtk_window_set_title(GTK_WINDOW(window), "Image Viewer");

    // Создаем GtkImage из GdkTexture
    GtkWidget* image = gtk_image_new_from_paintable(GDK_PAINTABLE(texture));

    // Задаем минимальный размер изображения (например, 100x100 пикселей)
    gtk_image_set_pixel_size(GTK_IMAGE(image), 100);

    // Добавляем изображение в окно
    gtk_window_set_child(GTK_WINDOW(window), image);

    // Устанавливаем минимальный размер окна
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);
    gtk_widget_set_size_request(window, 100, 100);

    // Делаем окно видимым
    gtk_widget_set_visible(window, TRUE);

    // Освобождаем ресурсы
    g_object_unref(texture);
    g_object_unref(loader);
}

int main(int argc, char** argv) {
    GtkApplication* app = gtk_application_new("com.example.imageviewer", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

