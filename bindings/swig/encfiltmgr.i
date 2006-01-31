%{
#include "encfiltmgr.h"
using namespace sword;
%}


class EncodingFilterMgr : public SWFilterMgr {
public:
        EncodingFilterMgr ();

        virtual ~EncodingFilterMgr();

%extend {

        int Encoding(int enc) {
             return self->Encoding(char(enc));
        }
}

        

        virtual void AddRawFilters(SWModule *module, ConfigEntMap &section);

        virtual void AddEncodingFilters(SWModule *module, ConfigEntMap &section);
};
%{
#include "encfiltmgr.h"
using namespace sword;
%}


class EncodingFilterMgr : public SWFilterMgr {
public:
        EncodingFilterMgr ();

        virtual ~EncodingFilterMgr();

%extend {

        int Encoding(int enc) {
             return self->Encoding(char(enc));
        }
}

        

        virtual void AddRawFilters(SWModule *module, ConfigEntMap &section);

        virtual void AddEncodingFilters(SWModule *module, ConfigEntMap &section);
};
%{
#include "encfiltmgr.h"
using namespace sword;
%}


class EncodingFilterMgr : public SWFilterMgr {
public:
        EncodingFilterMgr ();

        virtual ~EncodingFilterMgr();

%extend {

        int Encoding(int enc) {
             return self->Encoding(char(enc));
        }
}

        

        virtual void AddRawFilters(SWModule *module, ConfigEntMap &section);

        virtual void AddEncodingFilters(SWModule *module, ConfigEntMap &section);
};
%{
#include "encfiltmgr.h"
using namespace sword;
%}


class EncodingFilterMgr : public SWFilterMgr {
public:
        EncodingFilterMgr ();

        virtual ~EncodingFilterMgr();

%extend {

        int Encoding(int enc) {
             return self->Encoding(char(enc));
        }
}

        

        virtual void AddRawFilters(SWModule *module, ConfigEntMap &section);

        virtual void AddEncodingFilters(SWModule *module, ConfigEntMap &section);
};
%{
#include "encfiltmgr.h"
using namespace sword;
%}


class EncodingFilterMgr : public SWFilterMgr {
public:
        EncodingFilterMgr ();

        virtual ~EncodingFilterMgr();

%extend {

        int Encoding(int enc) {
             return self->Encoding(char(enc));
        }
}

        

        virtual void AddRawFilters(SWModule *module, ConfigEntMap &section);

        virtual void AddEncodingFilters(SWModule *module, ConfigEntMap &section);
};
%{
#include "encfiltmgr.h"
using namespace sword;
%}


class EncodingFilterMgr : public SWFilterMgr {
public:
        EncodingFilterMgr ();

        virtual ~EncodingFilterMgr();

%extend {

        int Encoding(int enc) {
             return self->Encoding(char(enc));
        }
}

        

        virtual void AddRawFilters(SWModule *module, ConfigEntMap &section);

        virtual void AddEncodingFilters(SWModule *module, ConfigEntMap &section);
};
%{
#include "encfiltmgr.h"
using namespace sword;
%}


class EncodingFilterMgr : public SWFilterMgr {
public:
        EncodingFilterMgr ();

        virtual ~EncodingFilterMgr();

%extend {

        int Encoding(int enc) {
             return self->Encoding(char(enc));
        }
}

        

        virtual void AddRawFilters(SWModule *module, ConfigEntMap &section);

        virtual void AddEncodingFilters(SWModule *module, ConfigEntMap &section);
};
