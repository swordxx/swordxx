#include "soapH.h"
#include <iostream.h>

char server[] = "http://localhost:18083";

int main(int argc, char **argv) {
	xsd__int mgr;
	xsd__int noop;

	if (soap_call_sword__SWMgr_new(server, NULL, mgr)) {
		soap_print_fault(stderr);
		soap_print_fault_location(stderr);
		exit(-1);
	}

	xsd__int modIt;
	if (soap_call_sword__SWMgr_getModulesIterator(server, NULL, mgr, modIt)) {
		soap_print_fault(stderr);
		soap_print_fault_location(stderr);
		exit(-1);
	}

	xsd__int module;
	do {
		if (soap_call_sword__ModList_iterator_val(server, NULL, modIt, module)) {
			soap_print_fault(stderr);
			soap_print_fault_location(stderr);
			exit(-1);
		}


		if (module) {
			char *name;
			char *desc;
			if (soap_call_sword__SWModule_getName(server, NULL, module, name)) {
				soap_print_fault(stderr);
				soap_print_fault_location(stderr);
				exit(-1);
			}
			if (soap_call_sword__SWModule_getDescription(server, NULL, module, desc)) {
				soap_print_fault(stderr);
				soap_print_fault_location(stderr);
				exit(-1);
			}

			cout << "[" << name << "] " << desc << endl;

			if (soap_call_sword__ModList_iterator_next(server, NULL, modIt, noop)) {
				soap_print_fault(stderr);
				soap_print_fault_location(stderr);
				exit(-1);
			}
		}

		
	} while (module);

	if (soap_call_sword__SWMgr_delete(server, NULL, mgr, noop)) {
		soap_print_fault(stderr);
		soap_print_fault_location(stderr);
		exit(-1);
	}
	char *text;
	if (soap_call_sword__Quick_getModuleRenderText(server, NULL, "MHC", "jas1:19", text)) {
		soap_print_fault(stderr);
		soap_print_fault_location(stderr);
		exit(-1);
	}
	cout << text << endl;

	printf("success: %d\n", mgr);
	soap_free();
	return 0;
}

#include "sword.nsmap"
