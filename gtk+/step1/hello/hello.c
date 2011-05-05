#include <gtk/gtk.h>

#ifdef WIN32
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
	
	gtk_init(&argc, &argv);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	// 1.2 完善的窗口
	g_signal_connect(G_OBJECT(window), "delete_event",
	                 G_CALLBACK(gtk_main_quit), NULL);
	gtk_window_set_title (GTK_WINDOW(window), "一个功能完善的窗口");
	gtk_window_set_default_size (GTK_WINDOW(window), 500, 300);
	gtk_window_set_position (GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	// 退出窗体	
	gtk_widget_show(window);
	gtk_main();
	
	return 0;
}
