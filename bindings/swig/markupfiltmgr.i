%{
#include <markupfiltmgr.h>
using namespace sword;
%}


class MarkupFilterMgr : public EncodingFilterMgr {
public:
        MarkupFilterMgr();
        virtual ~MarkupFilterMgr();

%extend {
        int Markup(int markup) {
                 return self->Markup(char(markup));
        }
}


        virtual void AddRenderFilters(SWModule *module, ConfigEntMap &section);
};

%{
#include <markupfiltmgr.h>
using namespace sword;
%}


class MarkupFilterMgr : public EncodingFilterMgr {
public:
        MarkupFilterMgr();
        virtual ~MarkupFilterMgr();

%extend {
        int Markup(int markup) {
                 return self->Markup(char(markup));
        }
}


        virtual void AddRenderFilters(SWModule *module, ConfigEntMap &section);
};

%{
#include <markupfiltmgr.h>
using namespace sword;
%}


class MarkupFilterMgr : public EncodingFilterMgr {
public:
        MarkupFilterMgr();
        virtual ~MarkupFilterMgr();

%extend {
        int Markup(int markup) {
                 return self->Markup(char(markup));
        }
}


        virtual void AddRenderFilters(SWModule *module, ConfigEntMap &section);
};

%{
#include <markupfiltmgr.h>
using namespace sword;
%}


class MarkupFilterMgr : public EncodingFilterMgr {
public:
        MarkupFilterMgr();
        virtual ~MarkupFilterMgr();

%extend {
        int Markup(int markup) {
                 return self->Markup(char(markup));
        }
}


        virtual void AddRenderFilters(SWModule *module, ConfigEntMap &section);
};

%{
#include <markupfiltmgr.h>
using namespace sword;
%}


class MarkupFilterMgr : public EncodingFilterMgr {
public:
        MarkupFilterMgr();
        virtual ~MarkupFilterMgr();

%extend {
        int Markup(int markup) {
                 return self->Markup(char(markup));
        }
}


        virtual void AddRenderFilters(SWModule *module, ConfigEntMap &section);
};

%{
#include <markupfiltmgr.h>
using namespace sword;
%}


class MarkupFilterMgr : public EncodingFilterMgr {
public:
        MarkupFilterMgr();
        virtual ~MarkupFilterMgr();

%extend {
        int Markup(int markup) {
                 return self->Markup(char(markup));
        }
}


        virtual void AddRenderFilters(SWModule *module, ConfigEntMap &section);
};

