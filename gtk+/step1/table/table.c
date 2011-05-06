#if defined(WIN32)
#include <Windows.h>
#include <locale.h>
#endif
#include <gtk/gtk.h>


int main(int argc, char* argv[])
{
	GtkWidget*			window;
	GtkWidget*			table;
	GtkWidget*			button;
	GtkWidget*			frame;

#if defined(WIN32)
	setlocale(LC_ALL, "zh_CN.UTF-8");
#endif
	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "格状容器");
	gtk_window_set_default_size(GTK_WINDOW(window), 200, 300);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	g_signal_connect(G_OBJECT(window), "destroy"
		, G_CALLBACK(gtk_main_quit), NULL);
	gtk_container_set_border_width(GTK_CONTAINER(window), 20);

	frame = gtk_frame_new("请注意下列按钮的排列");
	gtk_container_add(GTK_CONTAINER(window), frame);

	table = gtk_table_new(4, 4, FALSE);
	gtk_container_set_border_width(GTK_CONTAINER(table), 10);
	gtk_table_set_row_spacings(GTK_TABLE(table), 5);
	gtk_table_set_col_spacings(GTK_TABLE(table), 5);
	gtk_container_add(GTK_CONTAINER(frame), table);

	button = gtk_button_new();
	gtk_table_attach(GTK_TABLE(table), button, 0, 1, 0, 1, GTK_FILL
		, GTK_FILL, 0, 0);

	button = gtk_button_new();
	gtk_table_attach(GTK_TABLE(table), button, 1, 3, 1, 3, GTK_FILL
		, GTK_FILL, 0, 0);

	button = gtk_button_new();
	gtk_table_attach_defaults(GTK_TABLE(table), button, 0, 1, 1, 3);

	button = gtk_button_new();
	gtk_table_attach_defaults(GTK_TABLE(table), button, 1, 3, 0, 1);

	button = gtk_button_new();
	gtk_table_attach_defaults(GTK_TABLE(table), button, 0, 4, 3, 4);

	button = gtk_button_new();
	gtk_table_attach_defaults(GTK_TABLE(table), button, 3, 4, 0, 3);

	gtk_widget_show_all(window);
	gtk_main();
    return 0;
}



















