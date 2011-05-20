#pragma once

template <class T>
NPError		TNPP_NewProc(NPMIMEType pluginType, NPP instance, uint16_t mode
					 , int16_t argc, char* argn[], char* argv[], NPSavedData* saved)
{
	return T::_class.New(pluginType, instance, mode, argc, argn, argv, saved);
}

template <class T>
NPError		TNPP_DestroyProc(NPP instance, NPSavedData** save)
{
	return T::_class.Destroy(instance, save);
}

template <class T>
NPError		TNPP_SetWindowProc(NPP instance, NPWindow* window)
{
	return T::_class.SetWindow(instance, window);
}

template <class T>
NPError		TNPP_NewStreamProc(NPP instance, NPMIMEType type, NPStream* stream, NPBool seekable, uint16_t* stype)
{
	return T::_class.NewStream(instance, type, stream, seekable, stype);
}

template <class T>
NPError		TNPP_DestroyStreamProc(NPP instance, NPStream* stream, NPReason reason)
{
	return T::_class.DestroyStream(instance, stream, reason);
}

template <class T>
int32_t		TNPP_WriteReadyProc(NPP instance, NPStream* stream)
{
	return T::_class.WriteReady(instance, stream);
}

template <class T>
int32_t		TNPP_WriteProc(NPP instance, NPStream* stream, int32_t offset, int32_t len, void* buffer)
{
	return T::_class.Write(instance, stream, offset, len, buffer);
}

template <class T>
void		TNPP_StreamAsFileProc(NPP instance, NPStream* stream, const char* fname)
{
	T::_class.StreamAsFile(instance, stream, fname);
}

template <class T>
void		TNPP_PrintProc(NPP instance, NPPrint* platformPrint)
{
	T::_class.Print(instance, platformPrint);
}

template <class T>
int16_t		TNPP_HandleEventProc(NPP instance, void* event)
{
	return T::_class.HandleEvent(instance, event);
}

template <class T>
void		TNPP_URLNotifyProc(NPP instance, const char* url, NPReason reason, void* notifyData)
{
	T::_class.URLNotify(instance, url, reason, notifyData);
}

template <class T>
NPError		TNPP_GetValueProc(NPP instance, NPPVariable variable, void *ret_value)
{
	return T::_class.GetValue(instance, variable, ret_value);
}

template <class T>
NPError		TNPP_SetValueProc(NPP instance, NPNVariable variable, void *value)
{
	return T::_class.SetValue(instance, variable, value);
}

//////////////////////////////////////////////////////////////////////////
template <class T>
class TNPPluginFuncs : public NPPluginFuncs
{
public:
	TNPPluginFuncs()
	{
		this->size = sizeof(NPPluginFuncs);
		this->version = ((NP_VERSION_MAJOR << 8) | NP_VERSION_MINOR);
		this->newp = TNPP_NewProc<T>;
		this->destroy = TNPP_DestroyProc<T>;
		this->setwindow = TNPP_SetWindowProc<T>;
		this->newstream = TNPP_NewStreamProc<T>;
		this->destroystream = TNPP_DestroyStreamProc<T>;
		this->asfile = TNPP_StreamAsFileProc<T>;
		this->writeready = TNPP_WriteReadyProc<T>;
		this->write = TNPP_WriteProc<T>;
		this->print = TNPP_PrintProc<T>;
		this->event = TNPP_HandleEventProc<T>;
		this->urlnotify = TNPP_URLNotifyProc<T>;
		this->javaClass = NULL;
		this->getvalue = TNPP_GetValueProc<T>;
		this->setvalue = TNPP_SetValueProc<T>;
	}
};

//////////////////////////////////////////////////////////////////////////
template <class T>
class TNpApp
{
public:
	TNpApp()
	{

	}

public:
	NPError		New(NPMIMEType pluginType, NPP instance, uint16_t mode
		, int16_t argc, char* argn[], char* argv[], NPSavedData* saved)
	{
		return NPERR_NO_ERROR;
	}

	NPError		Destroy(NPP instance, NPSavedData** save)
	{
		return NPERR_NO_ERROR;
	}

	NPError		SetWindow(NPP instance, NPWindow* window)
	{
		return NPERR_NO_ERROR;
	}

	NPError		NewStream(NPP instance, NPMIMEType type
		, NPStream* stream, NPBool seekable, uint16_t* stype)
	{
		return NPERR_NO_ERROR;
	}

	NPError		DestroyStream(NPP instance, NPStream* stream, NPReason reason)
	{
		return NPERR_NO_ERROR;
	}

	int32_t		WriteReady(NPP instance, NPStream* stream)
	{
		return 0;
	}

	int32_t		Write(NPP instance, NPStream* stream, int32_t offset, int32_t len, void* buffer)
	{
		return 0;
	}

	void		StreamAsFile(NPP instance, NPStream* stream, const char* fname)
	{
	}

	void		Print(NPP instance, NPPrint* platformPrint)
	{
	}

	int16_t		HandleEvent(NPP instance, void* event)
	{
		return 0;
	}

	void		URLNotify(NPP instance, const char* url, NPReason reason, void* notifyData)
	{
	}

	NPError		GetValue(NPP instance, NPPVariable variable, void *ret_value)
	{
		return NPERR_NO_ERROR;
	}

	NPError		SetValue(NPP instance, NPNVariable variable, void *value)
	{
		return NPERR_NO_ERROR;
	}
public:
	static T					_class;
	static TNPPluginFuncs<T>	PluginFuncs;
};

// App global
template <class T>
T						TNpApp<T>::_class;

template <class T>
TNPPluginFuncs<T>		TNpApp<T>::PluginFuncs;	