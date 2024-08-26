#include <gtk/gtk.h>
#include <graphene.h>

static void on_ok_button_clicked(GtkButton *button, gpointer user_data) {
    g_print("OK pressed\n");
    GtkWindow *dialog = GTK_WINDOW(user_data);
    gtk_window_destroy(dialog);
}

static void on_cancel_button_clicked(GtkButton *button, gpointer user_data) {
    g_print("Cancel pressed\n");
    GtkWindow *dialog = GTK_WINDOW(user_data);
    gtk_window_destroy(dialog);
}

static void open_modal_window(GtkWidget *parent_grid, GtkWindow *parent_window) {
    // Создание модального окна
    GtkWidget *dialog = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(dialog), "Modal Dialog");
    gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);
    gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(parent_window));
    gtk_window_set_default_size(GTK_WINDOW(dialog), 200, 100);

    // Контейнер для виджетов
    GtkWidget *content_area = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_window_set_child(GTK_WINDOW(dialog), content_area);

    // Добавление метки
    GtkWidget *label = gtk_label_new("This is a modal dialog");
    gtk_box_append(GTK_BOX(content_area), label);

    // Добавление кнопок
    GtkWidget *button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(content_area), button_box);

    GtkWidget *ok_button = gtk_button_new_with_label("OK");
    gtk_box_append(GTK_BOX(button_box), ok_button);
    g_signal_connect(ok_button, "clicked", G_CALLBACK(on_ok_button_clicked), dialog);

    GtkWidget *cancel_button = gtk_button_new_with_label("Cancel");
    gtk_box_append(GTK_BOX(button_box), cancel_button);
    g_signal_connect(cancel_button, "clicked", G_CALLBACK(on_cancel_button_clicked), dialog);

    // Получение позиции родительского grid
    graphene_point_t point = GRAPHENE_POINT_INIT(0, 0); // Инициализация начальной точки
    graphene_point_t transformed_point;

    if (gtk_widget_compute_point(parent_grid, GTK_WIDGET(parent_window), &point, &transformed_point)) {
        // Позиционирование модального окна над родительским grid
        gtk_window_move(GTK_WINDOW(dialog), static_cast<int>(transformed_point.x), static_cast<int>(transformed_point.y));
    }

    // Отображение модального окна
    gtk_widget_show(dialog);
}

static void activate(GtkApplication* app, gpointer user_data) {
    // Создание основного окна
    GtkWidget *main_window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(main_window), "Main Window");
    gtk_window_set_default_size(GTK_WINDOW(main_window), 400, 200);

    // Создание grid
    GtkWidget *grid = gtk_grid_new();
    gtk_window_set_child(GTK_WINDOW(main_window), grid);

    // Кнопка для открытия модального окна
    GtkWidget *open_button = gtk_button_new_with_label("Open Modal");
    gtk_grid_attach(GTK_GRID(grid), open_button, 0, 0, 1, 1);

    // Подключение сигнала нажатия кнопки для открытия модального окна
    g_signal_connect_data(open_button, "clicked", G_CALLBACK(open_modal_window), grid, NULL, G_CONNECT_AFTER);

    // Отображение основного окна
    gtk_widget_show(main_window);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

