#include <gtk/gtk.h>
#include <adwaita.h>

#include "ui/identifier.h"
#include "ui/signal.h"

#define APPLICATION_ID nullptr

static void activate(GtkApplication* app, gpointer user_data);

static GtkBuilder* ui_builder;

int main (int argc, char** argv) {
    adw_init();
    GtkApplication* app = gtk_application_new(APPLICATION_ID, G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    int status = g_application_run(G_APPLICATION (app), argc, argv);

    g_object_unref(app);
    g_object_unref(ui_builder);

    return status;
}

static void activate(GtkApplication* app, gpointer user_data) {
    ui_builder = gtk_builder_new_from_file(UI_MAIN_WINDOW_STACK);
    
    GtkBuilderScope* scope = gtk_builder_get_scope(ui_builder);
    gtk_builder_cscope_add_callback(scope, main_menu_start_random_simulation_clicked);

    GtkWindow* window = GTK_WINDOW(gtk_builder_get_object(ui_builder, ID_WINDOW));
    gtk_window_set_application(window, app);

    GtkStack* view_stack = GTK_STACK(gtk_builder_get_object(ui_builder, ID_STACK_VIEW_CONTAINER));
    GtkWidget* main_menu = GTK_WIDGET(gtk_builder_get_object(ui_builder, ID_MAIN_MENU_VIEW));

    GtkButton* quit_button = GTK_BUTTON(gtk_builder_get_object(ui_builder, ID_MAIN_MENU_BUTTON_QUIT));
    g_signal_connect_swapped(quit_button, "clicked", G_CALLBACK(gtk_window_destroy), window);
    
    gtk_stack_set_visible_child(view_stack, main_menu);

    gtk_window_present(window);
}