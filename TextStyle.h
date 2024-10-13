#pragma once

enum class TextStyle {
    Default         = 0,
    Bold            = 1,
    Faint           = 2, // более темный / бледный
    Italic          = 3,
    Underline       = 4, // подчеркинаие
    SlowBlink       = 5, // медленно мигающий
    RapidBlink      = 6, // быстро мигающий
    Inverse         = 7, // swap(fontColor, backgoundColor)
    Сonceal         = 8, // Невидимый
    СrossedOut      = 9, // Перечеркнутый

    DoubleUnderline = 21,// Двойное подчеркивание
};