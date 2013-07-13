#include "plugin_main.h"


#define NAME        "TryNPAPI"
#define VERSION     "0.1"
#define DESCRIPTION NAME " (Mozilla SDK)"



NPNetscapeFuncs* sBrowserFuncs;

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
  switch (aVariable) {
    case NPPVpluginNameString:
      *((char**)aValue) = NAME;
      break;
    case NPPVpluginDescriptionString:
      *((char**)aValue) = DESCRIPTION;
      break;
    default:
      return NPERR_INVALID_PARAM;
      break;
  }
  return NPERR_NO_ERROR;
}

NP_EXPORT(NPError)
NP_Shutdown()
{
  return NPERR_NO_ERROR;
}

NPError
NPP_New(NPMIMEType pluginType, NPP instance, uint16_t mode, int16_t argc, char* argn[], char* argv[], NPSavedData* saved) {

  NPStream* stream;
  char* myData = "<html><head><title>Hello Stream!</title></head><body><p><strong>Rcved from simpleNPAPI plug-in!</strong></p></body></html>";
  int32_t myLength = strlen(myData) + 1;

  /* Create the stream. */
  sBrowserFuncs->newstream(instance, "text/html", "_blank", &stream);

  /* Push data into the stream. */
  sBrowserFuncs->write(instance, stream, myLength, myData);

  /* Delete the stream. */
  sBrowserFuncs->destroystream(instance, stream, NPRES_DONE);

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
  return NPERR_GENERIC_ERROR;
}

NPError
NPP_SetValue(NPP instance, NPNVariable variable, void *value) {
  return NPERR_GENERIC_ERROR;
}

