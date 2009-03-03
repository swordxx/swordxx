%{
#include "swlocale.h"
%}

%ignore sword::SWLocale::operator+=;
%ignore sword::SWLocale::getBooks;

%include "swlocale.h"

/*%extend sword::SWLocale {
        int getNumBookGroupings() {
                return 2;
        }

        int getNumBooks(int grouping) {
                char *numbooks;
                struct sbook **books;

                self->getBooks(&numbooks, &books);

                return numbooks[grouping];
        }

        sbook getBook(int grouping, int booknum) {
                char *numbooks;
                struct sbook **books;
                self->getBooks(&numbooks, &books);

                return books[grouping][booknum];
        }
};*/
