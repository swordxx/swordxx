/******************************************************************************
 *
 * Copyright Jaak Ristioja
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation version 2.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 */

#ifndef SWORDXX_XMLBUILDER_H
#define SWORDXX_XMLBUILDER_H

#include <iosfwd>
#include <string>
#include <string_view>
#include <vector>


namespace swordxx {

class XmlBuilder {

private: /* Types: */

    struct TagNameLocation {
        std::string::size_type pos;
        std::string::size_type size;
    };

public: /* Methods: */

    XmlBuilder();
    XmlBuilder(XmlBuilder &&) noexcept;
    XmlBuilder(XmlBuilder const &);

    XmlBuilder(std::string_view elemName);

    XmlBuilder & operator=(XmlBuilder &&) noexcept;
    XmlBuilder & operator=(XmlBuilder const &);

    bool inContent() const noexcept { return !m_inStartTag; }
    bool isComplete() const noexcept { return m_openTagNameLocations.empty(); }

    XmlBuilder & e(std::string_view elemName);
    XmlBuilder & a(std::string_view attrName, std::string_view attrValue);
    XmlBuilder & c(std::string_view charData);
    XmlBuilder & up();
    XmlBuilder & finish();

    std::string::size_type predictedSizeToFinish() const noexcept;

    std::string & finishTo(std::string & outputString);
    std::ostream & finishTo(std::ostream & outputStream);

    template <typename OutputFunc>
    void finishTo(OutputFunc output) const
            noexcept(noexcept(output(std::string_view()))
                     && noexcept(output(char())))
    {
        using namespace std::literals::string_view_literals;
        auto locSizeLeft = m_openTagNameLocations.size();
        if (m_inStartTag) {
            if (!--locSizeLeft) {
                output("/>"sv);
                return;
            }
            output("/></"sv);
        } else {
            if (!locSizeLeft)
                return;
            output("</"sv);
        }
        for (;; output("></"sv)) {
            auto const & loc = m_openTagNameLocations[locSizeLeft - 1u];
            output(std::string_view(m_contents.data() + loc.pos, loc.size));
            if (!--locSizeLeft)
                break;
        }
        output('>');
    }

    std::string const & asString() const noexcept { return m_contents; }
    std::string toString() const;

private: /* Fields: */

    /// The (potentially partial) output already generated:
    std::string m_contents;

    /** A vector of string views to names of <tags> already inserted, but not
        yet closed, in order of insertion: */
    std::vector<TagNameLocation> m_openTagNameLocations;

    /** The accumulative size of all views in m_tagNameLocations: */
    std::string::size_type m_sizeOfOpenTagNames;

    /** Whether m_contents ends at a position where attributes can be added to
        currently open start tag, e.g. ending with "<smth attr='name'": */
    bool m_inStartTag;

}; /* class XmlBuilder */

} /* namespace swordxx */

#endif /* SWORDXX_XMLBUILDER_H */
