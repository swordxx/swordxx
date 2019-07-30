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

#include <swordxx/roman.h>

#include "TestAssert.h"


static_assert(
        std::is_same_v<
            decltype(swordxx::parseRomanNumeral(
                         std::declval<std::string_view>())),
            std::optional<std::uintmax_t>
        >, "");

#define ERROR std::optional<std::uintmax_t>()
#define TEST(str, expected) \
    TEST_ASSERT(swordxx::parseRomanNumeral(str) == (expected))
#define TESTSV(str, expected) TEST(str ## sv, expected)

void testRegulars(std::size_t const from, std::size_t const to) {
    {
        std::string romanStr;
        for (std::size_t i = from; i < to; ++i, romanStr.clear()) {
            auto n = i;
            auto const construct =
                    [&n, &romanStr](auto const value, auto const str) {
                        while (n >= value) {
                            n -= value;
                            romanStr += str;
                        }
                    };
            construct(1000u, 'M');
            construct( 900u, "CM");
            construct( 500u, 'D');
            construct( 400u, "CD");
            construct( 100u, 'C');
            construct(  90u, "XC");
            construct(  50u, 'L');
            construct(  40u, "XL");
            construct(  10u, 'X');
            construct(   9u, "IX");
            construct(   5u, 'V');
            construct(   4u, "IV");
            construct(   1u, 'I');
            TEST(romanStr, i);
        }
    }
}

int main() {
    using namespace std::literals::string_view_literals;

    testRegulars(0, 5000u);

    // Test one span:
    TESTSV("", 0u);
    TESTSV("Y", ERROR);
    TESTSV("I", 1u);
    TESTSV("II", 2u);
    TESTSV("III", 3u);
    TESTSV("IIII", 4u);
    TESTSV("IIIII", 5u);
    TESTSV("IIIIII", 6u);
    TESTSV("IIIIIII", 7u);
    TESTSV("IIIIIIII", 8u);
    TESTSV("IIIIIIIII", 9u);
    TESTSV("IIIIIIIIII", 10u);
    TESTSV("IIIIIIIIIII", 11u);
    TESTSV("IIIIIIIIIIII", 12u);
    TESTSV("V", 5u);
    TESTSV("VV", ERROR);
    TESTSV("X", 10u);
    TESTSV("XX", 20u);
    TESTSV("XXX", 30u);
    TESTSV("XXXX", 40u);
    TESTSV("XXXXX", 50u);
    TESTSV("XXXXXX", 60u);
    TESTSV("XXXXXXX", 70u);
    TESTSV("XXXXXXXX", 80u);
    TESTSV("XXXXXXXXX", 90u);
    TESTSV("XXXXXXXXXX", 100u);
    TESTSV("XXXXXXXXXXX", 110u);
    TESTSV("XXXXXXXXXXXX", 120u);
    TESTSV("L", 50u);
    TESTSV("LL", ERROR);
    TESTSV("C", 100u);
    TESTSV("CC", 200u);
    TESTSV("CCC", 300u);
    TESTSV("CCCC", 400u);
    TESTSV("CCCCC", 500u);
    TESTSV("CCCCCC", 600u);
    TESTSV("CCCCCCC", 700u);
    TESTSV("CCCCCCCC", 800u);
    TESTSV("CCCCCCCCC", 900u);
    TESTSV("CCCCCCCCCC", 1000u);
    TESTSV("CCCCCCCCCCC", 1100u);
    TESTSV("CCCCCCCCCCCC", 1200u);
    TESTSV("D", 500u);
    TESTSV("DD", ERROR);
    TESTSV("M", 1000u);
    TESTSV("MM", 2000u);
    TESTSV("MMM", 3000u);
    TESTSV("MMMM", 4000u);
    TESTSV("MMMMM", 5000u);
    TESTSV("MMMMMM", 6000u);
    TESTSV("MMMMMMM", 7000u);
    TESTSV("MMMMMMMM", 8000u);
    TESTSV("MMMMMMMMM", 9000u);
    TESTSV("MMMMMMMMMM", 10000u);
    TESTSV("MMMMMMMMMMM", 11000u);
    TESTSV("MMMMMMMMMMMM", 12000u);


    // Corner cases:
    // "vx" and the like:
    TESTSV("VX", ERROR);
    TESTSV("XVX", ERROR);
    TESTSV("CVX", ERROR);
    TESTSV("VVX", ERROR);
    TESTSV("XVVX", ERROR);
    TESTSV("CVVX", ERROR);
    TESTSV("LC", ERROR);
    TESTSV("CLC", ERROR);
    TESTSV("MLC", ERROR);
    TESTSV("LLC", ERROR);
    TESTSV("CLLC", ERROR);
    TESTSV("MLLC", ERROR);
    TESTSV("DM", ERROR);
    TESTSV("MDM", ERROR);
    TESTSV("DDM", ERROR);
    TESTSV("MDDM", ERROR);
    // "iiiiiiiiiix" and the like:
    TESTSV("IIV", 3u);
    TESTSV("IIIV", 2u);
    TESTSV("IIIIV", 1u);
    TESTSV("IIIIIV", ERROR);
    TESTSV("XXIIV", 23u);
    TESTSV("XXIIIV", 22u);
    TESTSV("XXIIIIV", 21u);
    TESTSV("XXIIIIIV", ERROR);
    TESTSV("LIIV", 53u);
    TESTSV("LIIIV", 52u);
    TESTSV("LIIIIV", 51u);
    TESTSV("LIIIIIV", ERROR);
    TESTSV("IIX", 8u);
    TESTSV("IIIX", 7u);
    TESTSV("IIIIX", 6u);
    TESTSV("IIIIIX", 5u);
    TESTSV("IIIIIIX", 4u);
    TESTSV("IIIIIIIX", 3u);
    TESTSV("IIIIIIIIX", 2u);
    TESTSV("IIIIIIIIIX", 1u);
    TESTSV("IIIIIIIIIIX", ERROR);
    TESTSV("XXIIX", 28u);
    TESTSV("XXIIIX", 27u);
    TESTSV("XXIIIIX", 26u);
    TESTSV("XXIIIIIX", 25u);
    TESTSV("XXIIIIIIX", 24u);
    TESTSV("XXIIIIIIIX", 23u);
    TESTSV("XXIIIIIIIIX", 22u);
    TESTSV("XXIIIIIIIIIX", 21u);
    TESTSV("XXIIIIIIIIIIX", ERROR);
    TESTSV("LIIX", 58u);
    TESTSV("LIIIX", 57u);
    TESTSV("LIIIIX", 56u);
    TESTSV("LIIIIIX", 55u);
    TESTSV("LIIIIIIX", 54u);
    TESTSV("LIIIIIIIX", 53u);
    TESTSV("LIIIIIIIIX", 52u);
    TESTSV("LIIIIIIIIIX", 51u);
    TESTSV("LIIIIIIIIIIX", ERROR);
    // "vx" and the like:
    TESTSV("VX", ERROR);
    TESTSV("VL", 45u);
    TESTSV("VC", 95u);
    TESTSV("VD", 495u);
    TESTSV("VM", 995u);
    TESTSV("LC", ERROR);
    TESTSV("LD", 450u);
    TESTSV("LM", 950u);
    TESTSV("DM", ERROR);
    // "ixi" and the like:
    TESTSV("IVI", ERROR);
    TESTSV("IXI", ERROR);
    TESTSV("IXV", ERROR);
    TESTSV("ILI", ERROR);
    TESTSV("ILV", ERROR);
    TESTSV("ILX", ERROR);
    TESTSV("ICI", ERROR);
    TESTSV("ICV", ERROR);
    TESTSV("ICX", ERROR);
    TESTSV("ICL", ERROR);
    TESTSV("IDI", ERROR);
    TESTSV("IDV", ERROR);
    TESTSV("IDX", ERROR);
    TESTSV("IDL", ERROR);
    TESTSV("IDC", ERROR);
    TESTSV("IMI", ERROR);
    TESTSV("IMV", ERROR);
    TESTSV("IMX", ERROR);
    TESTSV("IML", ERROR);
    TESTSV("IMC", ERROR);
    TESTSV("IMD", ERROR);
    TESTSV("VVI", ERROR); // V
    TESTSV("VXI", ERROR);
    TESTSV("VXV", ERROR);
    TESTSV("VLI", 46u);
    TESTSV("VLV", ERROR);
    TESTSV("VLX", ERROR);
    TESTSV("VCI", 96u);
    TESTSV("VCV", ERROR);
    TESTSV("VCX", ERROR);
    TESTSV("VCL", ERROR);
    TESTSV("VDI", 496u);
    TESTSV("VDV", ERROR);
    TESTSV("VDX", ERROR);
    TESTSV("VDL", ERROR);
    TESTSV("VDC", ERROR);
    TESTSV("VMI", 996u);
    TESTSV("VMV", ERROR);
    TESTSV("VMX", ERROR);
    TESTSV("VML", ERROR);
    TESTSV("VMC", ERROR);
    TESTSV("VMD", ERROR);
    TESTSV("XLI", 41u); // X
    TESTSV("XLV", 45u);
    TESTSV("XLX", ERROR);
    TESTSV("XCI", 91u);
    TESTSV("XCV", 95u);
    TESTSV("XCX", ERROR);
    TESTSV("XCL", ERROR);
    TESTSV("XDI", 491u);
    TESTSV("XDV", 495u);
    TESTSV("XDX", ERROR);
    TESTSV("XDL", ERROR);
    TESTSV("XDC", ERROR);
    TESTSV("XMI", 991u);
    TESTSV("XMV", 995u);
    TESTSV("XMX", ERROR);
    TESTSV("XML", ERROR);
    TESTSV("XMC", ERROR);
    TESTSV("XMD", ERROR);
    TESTSV("LLI", ERROR); // L
    TESTSV("LLV", ERROR);
    TESTSV("LLX", ERROR);
    TESTSV("LCI", ERROR);
    TESTSV("LCV", ERROR);
    TESTSV("LCX", ERROR);
    TESTSV("LCL", ERROR);
    TESTSV("LDI", 451u);
    TESTSV("LDV", 455u);
    TESTSV("LDX", 460u);
    TESTSV("LDL", ERROR);
    TESTSV("LDC", ERROR);
    TESTSV("LMI", 951u);
    TESTSV("LMV", 955u);
    TESTSV("LMX", 960u);
    TESTSV("LML", ERROR);
    TESTSV("LMC", ERROR);
    TESTSV("LLD", ERROR);
    TESTSV("CDI", 401u); // C
    TESTSV("CDV", 405u);
    TESTSV("CDX", 410u);
    TESTSV("CDL", 450u);
    TESTSV("CDC", ERROR);
    TESTSV("CMI", 901u);
    TESTSV("CMV", 905u);
    TESTSV("CMX", 910u);
    TESTSV("CML", 950u);
    TESTSV("CMC", ERROR);
    TESTSV("CLD", ERROR);

    // "ixix" and the like:
    TESTSV("IXIX", ERROR);
    TESTSV("ICIX", ERROR);
    TESTSV("IXIC", ERROR);
    TESTSV("XCXC", ERROR);
    TESTSV("XMXC", ERROR);
    TESTSV("XCXM", ERROR);

    testRegulars(5000u, 100001u);
}
