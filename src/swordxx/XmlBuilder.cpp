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

#include "XmlBuilder.h"

#include <cassert>
#include <ostream>
#include <type_traits>
#include <utility>
#include "max_v.h"
#include "utilxml.h"


namespace swordxx {

using namespace std::literals::string_view_literals;

XmlBuilder::XmlBuilder()
    : m_sizeOfOpenTagNames(0u)
    , m_inStartTag(false)
{}

XmlBuilder::XmlBuilder(XmlBuilder && move) noexcept
    : m_contents(std::move(move.m_contents))
    , m_openTagNameLocations(std::move(move.m_openTagNameLocations))
    , m_sizeOfOpenTagNames(move.m_sizeOfOpenTagNames)
    , m_inStartTag(move.m_inStartTag)
{
    // Moving from std::string left it in an unspecified state, clear it:
    move.m_contents.clear();

    move.m_sizeOfOpenTagNames = 0u;
    move.m_inStartTag = false;
}

XmlBuilder::XmlBuilder(XmlBuilder const &) = default;

XmlBuilder::XmlBuilder(std::string_view elemName)
    : m_contents(1u, '<')
    , m_sizeOfOpenTagNames(elemName.size())
    , m_inStartTag(true)
{
    static_assert(max_v<decltype(elemName.size())>
                  <= max_v<std::string::size_type>, "");

    assert(!elemName.empty());
    m_contents += elemName;
    m_openTagNameLocations.emplace_back(TagNameLocation{1u, elemName.size()});
}

XmlBuilder & XmlBuilder::operator=(XmlBuilder && move) noexcept {
    m_contents = std::move(move.m_contents);
    // Moving from std::string left it in an unspecified state, clear it:
    move.m_contents.clear();

    m_openTagNameLocations = std::move(move.m_openTagNameLocations);

    m_sizeOfOpenTagNames = move.m_sizeOfOpenTagNames;
    move.m_sizeOfOpenTagNames = 0u;

    m_inStartTag = move.m_inStartTag;
    move.m_inStartTag = false;
    return *this;
}

XmlBuilder & XmlBuilder::operator=(XmlBuilder const &) = default;

XmlBuilder & XmlBuilder::e(std::string_view elemName) {
    auto const oldSize = m_contents.size();
    if (m_inStartTag) {
        m_contents += "><"sv;
    } else {
        m_contents += '<';
    }
    try {
        auto const pos = m_contents.size();
        m_contents += elemName;
        m_openTagNameLocations.emplace_back(
                    TagNameLocation{pos, elemName.size()});
    } catch (...) {
        m_contents.resize(oldSize);
        throw;
    }
    m_sizeOfOpenTagNames += elemName.size();
    m_inStartTag = true;
    return *this;
}

XmlBuilder & XmlBuilder::a(std::string_view attrName,
                           std::string_view attrValue)
{
    assert(m_inStartTag);
    auto const oldSize = m_contents.size();
    m_contents += ' ';
    try {
        m_contents += attrName;
        appendEqualsXmlAttributeValue(m_contents, attrValue);
    } catch (...) {
        m_contents.resize(oldSize);
        throw;
    }
    return *this;
}

XmlBuilder & XmlBuilder::c(std::string_view charData) {
    auto const oldSize = m_contents.size();
    if (m_inStartTag)
        m_contents += '>';
    try {
        for (;;) {
            auto const pos = charData.find_first_of("<&>"sv);
            if (pos == std::string::npos)
                break;
            if (pos > 0u)
                m_contents += charData.substr(0u, pos);
            switch (charData[pos]) {
            case '<': m_contents += "&lt;"sv; break;
            case '&': m_contents += "&amp;"sv; break;
            case '>': m_contents += "&gt;"sv; break;
            }
            charData.remove_prefix(pos + 1u);
        }
        m_contents += charData;
    } catch (...) {
        m_contents.resize(oldSize);
        throw;
    }
    m_inStartTag = false;
    return *this;
}

XmlBuilder & XmlBuilder::up() {
    if (m_inStartTag) {
        m_contents += "/>"sv;
        assert(!m_openTagNameLocations.empty());
        m_inStartTag = false;
        m_sizeOfOpenTagNames -= m_openTagNameLocations.back().size;
    } else {
        if (m_openTagNameLocations.empty())
            return *this;
        auto const oldSize = m_contents.size();
        m_contents += "</"sv;
        auto const & loc = m_openTagNameLocations.back();
        std::string_view const lastTagNameView(
                m_contents.data() + loc.pos,
                loc.size);
        try {
            m_contents +=  lastTagNameView;
            m_contents += '>';
        } catch (...) {
            m_contents.resize(oldSize);
            throw;
        }
        m_sizeOfOpenTagNames -= lastTagNameView.size();
    }
    m_openTagNameLocations.pop_back();
    return *this;
}

XmlBuilder & XmlBuilder::finish() {
    auto const oldSize = m_contents.size();
    auto const predictedSize = oldSize + predictedSizeToFinish();
    m_contents.reserve(predictedSize);
    try {
        finishTo([this](auto && v)
                 { m_contents += std::forward<decltype(v)>(v); });
        assert(m_contents.size() == predictedSize);
    } catch (...) {
        m_contents.resize(oldSize);
        throw;
    }
    m_openTagNameLocations.clear();
    m_sizeOfOpenTagNames = 0u;
    m_inStartTag = false;
    return *this;
}

std::string & XmlBuilder::finishTo(std::string & outputString) {
    auto const predictedSize = outputString.size() + predictedSizeToFinish();
    outputString.reserve(predictedSize);
    finishTo([&outputString](auto && v)
             { outputString += std::forward<decltype(v)>(v); });
    return outputString;
}

std::ostream & XmlBuilder::finishTo(std::ostream & outputStream) {
    finishTo([&outputStream](auto && v)
             { outputStream << std::forward<decltype(v)>(v); });
    return outputStream;
}

std::string::size_type XmlBuilder::predictedSizeToFinish() const noexcept {
    auto locSizeLeft = m_openTagNameLocations.size();
    auto sizeOfOpenTagNames = m_sizeOfOpenTagNames;
    std::string::size_type r;
    if (m_inStartTag) {
        assert(locSizeLeft);
        if (!--locSizeLeft)
            return 2u; // Just the closing "/>"
        sizeOfOpenTagNames -= m_openTagNameLocations.back().size;
        r = 4u; // Closing for start tag and beginning for end tag ("/></"):
    } else {
        if (!locSizeLeft)
            return 0u;
        r = 2u; // Beginning for end tag ("</"):
    }
    r += sizeOfOpenTagNames; // Sizes of all end tag names

    // Sizes of all "></" between pairs of end tags:
    r += 3u * (locSizeLeft - 1u);

    r += 1u; // ending '>';
    return r;
}

std::string XmlBuilder::toString() const {
    auto const predictedSize = m_contents.size() + predictedSizeToFinish();
    std::string r;
    r.reserve(predictedSize);
    r.append(m_contents);
    finishTo([&r](auto && v) { r += std::forward<decltype(v)>(v); });
    assert(r.size() == predictedSize);
    return r;
}

} /* namespace swordxx */
