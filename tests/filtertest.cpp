#include <iostream.h>
#include <thmlhtmlhref.h>

#define MAXBUF 30000
int main(int argc, char **argv) {
    ThMLHTMLHREF filter;
    char *buf = new char [ MAXBUF ];
    memset(buf, 0, MAXBUF);
    strcpy(buf, "This is a verse reference: <scripRef>James 1:19</scripRef>");
    filter.ProcessText(buf, MAXBUF - 1);

    cout << buf << "\n";

    return 0;
}
