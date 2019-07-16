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

#ifndef SWORDXX_SHAREREF_H
#define SWORDXX_SHAREREF_H

#include <memory>
#include <utility>


namespace swordxx {

/**
   \brief Converts a T & to a std::shared_ptr<T>.
   \param[in] ref The reference to convert.
   \returns a std::shared_ptr to the given reference.
   \warning This does not in any way guarantee that the lifetime of the object
            referred to will extend to the lifetime of the returned shared-
            ownership pointer. It is up to the caller to ensure that the object
            pointed to is used only when the given reference is still valid.
*/
template <typename T>
inline std::shared_ptr<T> shareRef(T & ref) noexcept {
    std::shared_ptr<void> aliasingTrick;
    return {std::move(aliasingTrick), &ref};
}

} /* namespace swordxx */

#endif /* SWORDXX_SHAREREF_H */
