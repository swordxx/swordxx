/******************************************************************************
 *
 *  mkfastmod.cpp -
 *
 * $Id$
 *
 * Copyright 2000-2013 CrossWire Bible Society (http://www.crosswire.org)
 *    CrossWire Bible Society
 *    P. O. Box 2528
 *    Tempe, AZ  85280-2528
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

#ifdef _MSC_VER
    #pragma warning( disable: 4251 )
#endif

#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <swordxx/swmgr.h>
#include <swordxx/swmodule.h>


int main(int argc, char * argv[]) {
    auto const outErr =
            [argv]() noexcept -> std::ostream &
            { return std::cerr << argv[0u] << ": "; };
    if (argc != 2) {
        outErr() << "Usage: " << argv[0u] << " <modname>" << std::endl;
        return EXIT_FAILURE;
    }

    swordxx::SWMgr manager;
    auto const it(manager.modules().find(argv[1]));
    if (it == manager.modules().end()) {
        outErr() << "Could not find module [" << argv[1u]
                 << "].  Available modules:\n";
        for (auto const & rp : manager.modules()) {
            auto const & module = *rp.second;
            std::cerr << '[' << module.getName() << "s]\t - "
                      << module.getDescription() << '\n';
        }
        std::cerr << std::flush;
        return EXIT_FAILURE;
    }

    auto & module = *it->second;
    std::cout << "Deleting any existing framework..." << std::flush;
    module.deleteSearchFramework();
    std::cout << "done\nBuilding framework, please wait..." << std::endl;
    std::cout << "[0================================5"
                 "0==============================100]" << std::endl;

    constexpr static std::size_t const progressBarLength = 70u;
    auto const percentUpdate =
            [](char percent, void * userData) {
                assert(percent >= 0);
                assert(percent <= 100);
                std::size_t newLength;
                if (percent <= 0) {
                    newLength = 0u;
                } else if (percent >= 100) {
                    newLength = progressBarLength;
                } else {
                    float const ratio = static_cast<float>(percent) / 100.0f;
                    newLength =
                            static_cast<std::size_t>(ratio * progressBarLength);
                }
                std::size_t & printed = *static_cast<std::size_t *>(userData);
                std::size_t const p = printed;
                if (p < newLength) {
                    std::size_t toPrint = newLength - p;
                    do {
                        std::cout << '=';
                    } while (--toPrint);
                    std::cout << std::flush;
                    printed = newLength;
                }
            };

    std::size_t progressPrinted = 0u;
    auto const error =
            module.createSearchFramework(percentUpdate, &progressPrinted);
    std::cout << std::endl;
    if (error) {
        outErr() << "Couldn't create search framework (permissions?)"
                 << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
