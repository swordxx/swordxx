#include "soapH.h"
#include <flatapi.h>


int sword__ModList_iterator_next(SWHANDLE hmmi, int &noop) {
	ModList_iterator_next(hmmi);
	return SOAP_OK;
}

int sword__ModList_iterator_val(SWHANDLE hmmi, SWHANDLE &hmodule) {
	hmodule = ModList_iterator_val(hmmi);
	return SOAP_OK;
}

int sword__SWMgr_new(SWHANDLE &retVal) {
	retVal = SWMgr_new();
	return SOAP_OK;
}

int sword__SWMgr_delete(SWHANDLE hmgr, int &noop) {
	SWMgr_delete(hmgr);
	return SOAP_OK;
}

int sword__SWMgr_getModulesIterator(SWHANDLE hmgr, SWHANDLE &hmodIterator) {
	hmodIterator = SWMgr_getModulesIterator(hmgr);
	return SOAP_OK;
}

int sword__SWMgr_getModuleByName(SWHANDLE hmgr, char *name, SWHANDLE &hmodule) {
	hmodule = SWMgr_getModuleByName(hmodule, name);
	return SOAP_OK;
}



int sword__SWModule_getName(SWHANDLE hmodule, char **name) {
	*name = SWModule_getName(hmodule);
	return SOAP_OK;
}

int sword__SWModule_getDescription(SWHANDLE hmodule, char **description) {
	*description = SWModule_getDescription(hmodule);
	return SOAP_OK;
}




main() {
    int m, s;
     m = soap_bind("localhost", 18083, 100);
	if (m < 0) {
		soap_print_fault(stderr);
		exit(-1);
	}
	fprintf(stderr, "Socket connection successful: master socket = %d\n", m);
	for (int i = 1; ; i++) {
		s = soap_accept();
		if (s < 0) {
			soap_print_fault(stderr);
			exit(-1);
		}
		fprintf(stderr, "%d: accepted connection from IP = %d.%d.%d.%d socket = %d", i, (soap_ip<<24)&0xFF, (soap_ip<<16)&0xFF, (soap_ip<<8)&0xFF, soap_ip&0xFF, s);
		soap_serve(); // process RPC skeletons
		fprintf(stderr, "request served\n");
		soap_end(); // clean up everything and close socket
	}
}

#include "sword.nsmap"

