#include <gtk/gtk.h>

gint	count		= 1;

void on_button_clicked(GtkWidget* buttun, gpointer userdata);

// #ifdef WIN32
#if 0
#include <windows.h>
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int		argc	= __argc;
	char**	argv	= __argv;
#else
int main(int argc, char* argv[])
{
#endif
	GtkWidget*		window;
	GtkWidget*		button;
	
	gtk_init(&argc, &argv);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	// 1.2 完善的窗口
	g_signal_connect(G_OBJECT(window), "delete_event",
	                 G_CALLBACK(gtk_main_quit), NULL);
	gtk_window_set_title (GTK_WINDOW(window), "一个功能完善的窗口");
	gtk_window_set_default_size (GTK_WINDOW(window), 500, 300);
	gtk_window_set_position (GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	// 1.3 Hello World
	gtk_container_set_border_width(GTK_CONTAINER(window), 40);
	button = gtk_button_new_with_label("按此按钮会在终端上显示一行信息");
	g_signal_connect(G_OBJECT(button), "clicked",
		G_CALLBACK(on_button_clicked), (gpointer)count);
	gtk_container_add(GTK_CONTAINER(window), button);

	gtk_widget_show(button);
	// 退出窗体	
	gtk_widget_show(window);
	gtk_main();
	
	return 0;
}

// 1.3 Hello World
void on_button_clicked(GtkWidget* buttun, gpointer userdata)
{
	g_print("你好， 这是Hello功能的测试.");
	g_print("%d\n", (gint)userdata);
	count = count + 1;
}