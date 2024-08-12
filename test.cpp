#include <gtk/gtk.h>
#include <cairo.h>
#include <cmath>

// g++ -g -o t test.cpp `pkg-config --cflags --libs gtk4` -lcurl



static void on_snapshot(GtkDrawingArea *drawing_area, GtkSnapshot *snapshot, gpointer user_data) {
    double percentage = *reinterpret_cast<double*>(user_data);

    int width = gtk_widget_get_width(GTK_WIDGET(drawing_area));
    int height = gtk_widget_get_height(GTK_WIDGET(drawing_area));

    int radius = fmin(width, height) / 2 - 10;
    int center_x = width / 2;
    int center_y = height / 2;
    double start_angle = -M_PI / 2;
    double sweep_angle = 2 * M_PI * percentage;

    GtkRenderNode *node = gtk_render_node_new();
    GskPath *path = gsk_path_new();
    gsk_path_move_to(path, center_x, center_y);
    gsk_path_arc(path, center_x, center_y, radius, start_angle, start_angle + sweep_angle);
    gsk_path_close(path);

    GskColor color = {0, 0, 1, 1}; // Blue color
    gsk_render_node_set_color(node, &color);
    gsk_render_node_set_path(node, path);

    gtk_snapshot_push_node(snapshot, node);
    gtk_snapshot_paint_node(snapshot, node);

    g_object_unref(node);
    g_object_unref(path);
}

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Simple Drawing Example");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);

    GtkWidget *drawing_area = gtk_drawing_area_new();
    gtk_window_set_child(GTK_WINDOW(window), drawing_area);

    double percentage = 0.75; // 75%
    g_signal_connect(drawing_area, "snapshot", G_CALLBACK(on_snapshot), &percentage);

    gtk_widget_set_visible(window, TRUE);
    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    app = gtk_application_new("com.example.SimpleDrawingExample", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
