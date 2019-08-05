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

#ifndef SWORDXX_TOKENIZER_H
#define SWORDXX_TOKENIZER_H

#include <cstddef>
#include <string>
#include <string_view>
#include <utility>


namespace swordxx {
enum class SplitBehavior { KeepEmptyTokens, SkipEmptyTokens };

template <SplitBehavior splitBehavior_ = SplitBehavior::KeepEmptyTokens>
class SimpleTokenizer {

public: /* Constants: */

    static constexpr SplitBehavior const splitBehavior = splitBehavior_;

public: /* Types: */

    struct Sentinel {};

    template <typename CharT, typename SplitterT>
    class Iterator {

    public: /* Types: */

    public: /* Methods: */

        friend bool operator==(Sentinel const &, Iterator const & it) noexcept
        { return !it; }
        friend bool operator==(Iterator const & it, Sentinel const &) noexcept
        { return !it; }
        friend bool operator!=(Sentinel const &, Iterator const & it) noexcept
        { return it; }
        friend bool operator!=(Iterator const & it, Sentinel const &) noexcept
        { return it; }

        constexpr Iterator() noexcept = default;

        template <typename Splitter>
        constexpr Iterator(std::basic_string_view<CharT> sv,
                           Splitter && splitter) noexcept
            : m_sv(sv.substr(0u, sv.find(splitter)))
            , m_sizeLeftAfterToken(sv.size() - m_sv.size())
            , m_splitter(std::forward<Splitter>(splitter))
        {
            if constexpr (splitBehavior == SplitBehavior::SkipEmptyTokens)
                while (m_sv.empty() && m_sv.data())
                    incr();
        }

        constexpr Iterator(Iterator &&) noexcept = default;
        constexpr Iterator(Iterator const &) noexcept = default;

        constexpr Iterator & operator=(Iterator &&) noexcept = default;
        constexpr Iterator & operator=(Iterator const &) noexcept = default;

        constexpr std::basic_string_view<CharT> const & operator*()
                const noexcept
        { return m_sv; }

        constexpr operator bool() const noexcept
        { return m_sv.data(); }

        constexpr Iterator & operator++() noexcept {
            incr();
            return *this;
        }

        constexpr Iterator operator++(int) noexcept {
            auto r(*this);
            incr();
            return r;
        }

    private: /* Methods: */

        constexpr void incr() noexcept {
            if constexpr (splitBehavior == SplitBehavior::SkipEmptyTokens) {
                do {
                    incr_();
                } while (m_sv.empty() && m_sv.data());
            } else {
                incr_();
            }
        }

        constexpr void incr_() noexcept {
            if (m_sizeLeftAfterToken <= 0u) {
                m_sv = std::basic_string_view<CharT>();
            } else {
                auto const ss = splitterSize(m_splitter);
                std::basic_string_view<CharT> const tail(
                            m_sv.data() + m_sv.size() + ss,
                            m_sizeLeftAfterToken - ss);
                m_sv = tail.substr(0u, tail.find(m_splitter));
                m_sizeLeftAfterToken = tail.size() - m_sv.size();
            }
        }

        static constexpr std::size_t splitterSize(SplitterT const & splitter)
                noexcept
        {
            if constexpr (std::is_same_v<SplitterT, CharT>) {
                return 1u;
            } else {
                return splitter.size();
            }
        }

    private: /* Fields: */

        std::basic_string_view<CharT> m_sv;
        std::size_t m_sizeLeftAfterToken;
        SplitterT m_splitter;

    };

    template <typename CharT, typename SplitterT>
    class ResultRange {

        friend class SimpleTokenizer;

    public: /* Types: */

        using Iterator = SimpleTokenizer::Iterator<CharT, SplitterT>;

    public: /* Methods: */

        constexpr Iterator begin() const noexcept { return {m_sv, m_splitter}; }
        constexpr Sentinel end() const noexcept { return {}; }

        constexpr Iterator cbegin() const noexcept { return {m_sv, m_splitter}; }
        constexpr Sentinel cend() const noexcept { return {}; }

    private: /* Methods: */

        template <typename U, typename S>
        constexpr ResultRange(U && sv, S && splitter) noexcept
            : m_sv(std::forward<U>(sv))
            , m_splitter(std::forward<S>(splitter))
        {}

    private: /* Fields: */

        std::basic_string_view<CharT> m_sv;
        SplitterT const m_splitter;

    };

public: /* Methods: */

    template <typename CharT>
    constexpr static ResultRange<CharT, CharT>
    tokenize(std::basic_string_view<CharT> sv, CharT splitter) noexcept
    { return {sv, splitter}; }

    template <typename CharT>
    constexpr static ResultRange<CharT, CharT>
    tokenize(std::basic_string<CharT> const & str, CharT splitter) noexcept
    { return {str, splitter}; }

    template <typename CharT>
    constexpr static ResultRange<CharT, CharT>
    tokenize(CharT const * cstr, CharT splitter) noexcept
    { return {cstr, splitter}; }

    template <typename CharT, typename SplitterT>
    constexpr static ResultRange<CharT, std::basic_string_view<CharT>>
    tokenize(std::basic_string_view<CharT> sv, SplitterT && splitter) noexcept
    { return {sv, std::forward<SplitterT>(splitter)}; }

    template <typename CharT, typename SplitterT>
    constexpr static ResultRange<CharT, std::basic_string_view<CharT>>
    tokenize(std::basic_string<CharT> const & str, SplitterT && splitter)
            noexcept
    { return {str, std::forward<SplitterT>(splitter)}; }

    template <typename CharT, typename SplitterT>
    constexpr static ResultRange<CharT, std::basic_string_view<CharT>>
    tokenize(CharT const * cstr, SplitterT && splitter) noexcept
    { return {cstr, std::forward<SplitterT>(splitter)}; }

};

} /* namespace swordxx */

#endif /* SWORDXX_UTILSTR_H */
