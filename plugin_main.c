#include "plugin_main.h"
#include "stdio.h"
#include "string.h"

#define NAME        "TryNPAPI"
#define VERSION     "0.1"
#define DESCRIPTION NAME " (Mozilla SDK)"

static const char *METHOD = "send";



NPNetscapeFuncs* sBrowserFuncs;
NPP currentInstance;

bool plugin_has_method(NPObject *obj, NPIdentifier methodName);
bool plugin_invoke(NPObject *obj, NPIdentifier methodName, const NPVariant *args, uint32_t argCount, NPVariant *result);
bool plugin_invoke_default(NPObject *npobj, const NPVariant *args, uint32_t argCount, NPVariant *result);

static struct NPClass scriptablePluginClass = {
    NP_CLASS_STRUCT_VERSION,
    NULL,
    NULL,
    NULL,
    plugin_has_method,
    plugin_invoke,
    plugin_invoke_default,
    NULL,
    NULL,
    NULL,
    NULL,
};

bool plugin_has_method(NPObject *obj, NPIdentifier methodName)
{
    NPUTF8 *name = sBrowserFuncs->utf8fromidentifier(methodName);
    bool result = strcmp(name, METHOD) == 0;
    sBrowserFuncs->memfree(name);
    return result;
}



bool plugin_invoke(NPObject *obj, NPIdentifier methodName, const NPVariant *args, uint32_t argCount, NPVariant *result)
{
    bool ret = true;
    NPUTF8 *name = sBrowserFuncs->utf8fromidentifier(methodName);
    //if(strcmp(name, METHOD) == 0) {

    NPString str = NPVARIANT_TO_STRING(args[0]);
    //char path[100];
    char *p = malloc(sizeof(char)*100);
    p = str.UTF8Characters;

    FILE *fp;
    fp = fopen(p,"r");
    NPStream* stream;
    char myData[100000];
    fgets(myData,100000,fp);
    fclose(fp);

    char *head = "<html><head><title>";
    char *middle = "</title></head><body><p><strong>";
    char *tail = "</strong></p></body></html>";
    int32_t myLength = strlen(head) + strlen(middle) + strlen(tail) + strlen(myData) + strlen(p) + 1;
    char *toSend = malloc(sizeof(char)*myLength);

    strcpy(toSend, head);
    strcat(toSend, p);
    strcat(toSend, middle);
    strcat(toSend, myData);
    strcat(toSend, tail);


    sBrowserFuncs->newstream(currentInstance, "text/html", "_blank", &stream);
    sBrowserFuncs->write(currentInstance, stream, myLength, toSend);
    //else
    //  sBrowserFuncs->write(currentInstance, stream, (strlen(error) + 1), error);
    //}

    sBrowserFuncs->memfree(name);

    return ret;
}


bool plugin_invoke_default(NPObject *npobj, const NPVariant *args, uint32_t argCount, NPVariant *result)
{
    return false;
}




NP_EXPORT(NPError)
NP_Initialize(NPNetscapeFuncs* bFuncs, NPPluginFuncs* pFuncs)
{
  //Get function table from browser
  sBrowserFuncs = bFuncs;

  //Set the function for browser
  pFuncs->newp = NPP_New;
  pFuncs->destroy = NPP_Destroy;
  pFuncs->setwindow = NPP_SetWindow;
  pFuncs->newstream = NPP_NewStream;
  pFuncs->destroystream = NPP_DestroyStream;
  pFuncs->asfile = NPP_StreamAsFile;
  pFuncs->writeready = NPP_WriteReady;
  pFuncs->write = NPP_Write;
  pFuncs->print = NPP_Print;
  pFuncs->event = NPP_HandleEvent;
  pFuncs->urlnotify = NPP_URLNotify;
  pFuncs->getvalue = NPP_GetValue;
  pFuncs->setvalue = NPP_SetValue;

  return NPERR_NO_ERROR;
}


NP_EXPORT(char*)
NP_GetPluginVersion()
{
  return (char*)VERSION;
}


NP_EXPORT(const char*)
NP_GetMIMEDescription()
{
  return "application/simpleNPAPI:bsc:simple plugin";
}

NP_EXPORT(NPError)
NP_GetValue(void* future, NPPVariable aVariable, void* aValue) {

  return NPP_GetValue(future,aVariable,aValue);
}

NP_EXPORT(NPError)
NP_Shutdown()
{
  return NPERR_NO_ERROR;
}

NPError
NPP_New(NPMIMEType pluginType, NPP instance, uint16_t mode, int16_t argc, char* argn[], char* argv[], NPSavedData* saved) {


  instance->pdata = sBrowserFuncs->createobject(instance, &scriptablePluginClass);
  currentInstance = instance;

  //NPStream* stream;
      //char* myData = "<html><head><title>Hello Stream!</title></head><body><p><strong>Rcved from simpleNPAPI plug-in!</strong></p></body></html>";
      //int32_t myLength = strlen(myData) + 1;

      //sBrowserFuncs->newstream(instance, "text/html", "_blank", &stream);
      //sBrowserFuncs->write(instance, stream, myLength, myData);

  //sBrowserFuncs->destroystream(instance, stream, NPRES_DONE);

  return NPERR_NO_ERROR;
}

NPError
NPP_Destroy(NPP instance, NPSavedData** save) {
  return NPERR_NO_ERROR;
}

NPError
NPP_SetWindow(NPP instance, NPWindow* window) {
  return NPERR_NO_ERROR;
}

NPError
NPP_NewStream(NPP instance, NPMIMEType type, NPStream* stream, NPBool seekable, uint16_t* stype) {
  return NPERR_GENERIC_ERROR;
}

NPError
NPP_DestroyStream(NPP instance, NPStream* stream, NPReason reason) {
  sBrowserFuncs->destroystream(instance, stream, NPRES_DONE);
  return NPERR_GENERIC_ERROR;
}

int32_t
NPP_WriteReady(NPP instance, NPStream* stream) {
  return 0;
}

int32_t
NPP_Write(NPP instance, NPStream* stream, int32_t offset, int32_t len, void* buffer) {
  return 0;
}

void
NPP_StreamAsFile(NPP instance, NPStream* stream, const char* fname) {
}

void
NPP_Print(NPP instance, NPPrint* platformPrint) {
}

int16_t
NPP_HandleEvent(NPP instance, void* event) {
  return 0;
}

void
NPP_URLNotify(NPP instance, const char* URL, NPReason reason, void* notifyData) {

}

NPError
NPP_GetValue(NPP instance, NPPVariable variable, void *value) {
  NPObject *pluginInstance = NULL;
  NPError ret = NPERR_NO_ERROR;

  if(instance)
      pluginInstance = instance->pdata;
  switch(variable) {
    case NPPVpluginNameString:
      *((char **)value) = NAME;
      break;
    case NPPVpluginDescriptionString:
      *((char **)value) = DESCRIPTION;
      break;
    case NPPVpluginScriptableIID:
    case NPPVpluginScriptableInstance:
        /* XPCOM scripting, obsolete */
      ret = NPERR_GENERIC_ERROR;
      break;
    case NPPVpluginScriptableNPObject:
        // If we didn't create any plugin instance, we create it.
      if (pluginInstance) {
        sBrowserFuncs->retainobject(pluginInstance);
        *((NPObject **)value) = pluginInstance;
        }else{
          ret = NPERR_GENERIC_ERROR;
        }
        break;
    default:
      ret = NPERR_INVALID_PARAM;
    }
    return ret;
  return NPERR_GENERIC_ERROR;
}

NPError
NPP_SetValue(NPP instance, NPNVariable variable, void *value) {

  return NPERR_GENERIC_ERROR;
}

