/**
 * @file debugging.h
 * @author Felix Heitmann (felixjulianheitmann@gmail.com)
 * @brief Provides debugging flags for different modules
 * @version 0.1
 * @date 2022-03-13
 */

#include <Arduino.h>

template <bool flag>
inline void dbg(String const &str) {
    if constexpr (flag) {
        Serial.print(str);
    }
}

template <bool flag>
inline void dbgln(String const &str) {
    if constexpr (flag) {
        Serial.println(str);
    }
}

namespace Dbg {
constexpr bool Srv = true;
}  // namespace Dbg
