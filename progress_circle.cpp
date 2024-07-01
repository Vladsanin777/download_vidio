#include <gtk/gtk.h>
#include <cmath>
#include <iostream>

// Радиус окружности
#define RADIUS 100

// Функция для рисования окружности
static gboolean draw_callback(GtkDrawingArea *widget, cairo_t *cr, gpointer data) {
    // Получаем ширину и высоту области рисования
    // int width = gtk_widget_get_width(widget);
    // int height = gtk_widget_get_height(widget);

    // Центрируем окружность по центру области рисования
    double x = RADIUS;
    double y = RADIUS;

    // Рисуем окружность
    cairo_set_source_rgb(cr, 0, 0, 1); // Синий цвет (RGB: 0, 0, 1)
    cairo_arc(cr, x, y, RADIUS, 0, 2 * G_PI); // Рисуем окружность
    cairo_fill(cr); // Заливаем окружность

    return GDK_EVENT_STOP;
}
// Функция завершения основного цикла
static void on_window_destroy(GtkWidget *widget, gpointer data) {
    g_main_loop_quit((GMainLoop *)data);
}


int main() {
    // Инициализация GTK
    gtk_init();

    // Создание основного окна
    GtkWidget *window = gtk_window_new();
    gtk_window_set_title(GTK_WINDOW(window), "Синяя окружность");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);

    // Создание области рисования (Drawing Area)
    GtkWidget *drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(drawing_area, 2 * RADIUS, 2 * RADIUS);

    // Обработчик события отрисовки для области рисования
    g_signal_connect(GTK_DRAWING_AREA(drawing_area), "draw", G_CALLBACK(draw_callback), NULL);

    // Установка области рисования в окно
    gtk_window_set_child(GTK_WINDOW(window), GTK_DRAWING_AREA(drawing_area));

    // Создание основного цикла
    GMainLoop *loop = g_main_loop_new(NULL, FALSE);

	// Установка сигнала для закрытия окна
    g_signal_connect(window, "destroy", G_CALLBACK(on_window_destroy), loop);

    // Показ окна
    gtk_window_present(GTK_WINDOW(window));

    g_main_loop_run(loop);

    // Освобождение ресурсов
    g_main_loop_unref(loop);

    return 0;
}

