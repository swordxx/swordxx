#include <QApplication>
#include <QTextEdit>
#include <QString>
#include <swmgr.h>
#include <swmodule.h>
#include <stdio.h>

using namespace sword;

int main(int argc, char **argv) {

	if (argc != 3) {
		fprintf(stderr, "\nusage: %s <modname> <\"lookup key\">\n"
							 "\tExample: %s KJV \"James 1:19\"\n\n", argv[0], argv[0]);
		exit(-1);
	}

	QApplication app(argc, argv);

	QString modName = argv[1];
	QString key = argv[2];

	SWMgr library;
	SWModule *mod = library.getModule(modName);

	QTextEdit textEdit;
	if (mod) {
		mod->setKey(key);
		textEdit.setText(mod->renderText());
	}
	else textEdit.setText("module not found");

	textEdit.show();

	return app.exec();
}
