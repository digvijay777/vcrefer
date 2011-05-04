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
	gtk_widget_show(window);
	gtk_main();
	
	return 0;
}
