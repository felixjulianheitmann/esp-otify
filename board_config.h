/**
 * @file board_config.h
 * @author Felix Heitmann (felixjulianheitmann@gmail.com)
 * @brief This file contains basic configuration constants for the board
 * @version 0.1
 * @date 2022-03-13
 */

#pragma once

namespace board {

// Led output GPIO on the ESP-01 module
constexpr int Led_Pin = 2;
constexpr bool Led_Active_High = false;

}  // namespace board
