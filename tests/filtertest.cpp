#include <iostream.h>
#include <thmlhtmlhref.h>

int main(int argc, char **argv) {
    ThMLHTMLHREF filter;
    char *buf = new char [ 4096 ];
    strcpy(buf, "This is a verse reference: <scripRef>James 1:19</scripRef>");
    filter.ProcessText(buf);

    cout << buf << "\n";

    return 0;
}
