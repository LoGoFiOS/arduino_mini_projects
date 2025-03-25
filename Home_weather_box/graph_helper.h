// #pragma once
// #include <GyverOLED.h>

// // Глобальные переменные — можно заменить на аргументы, если нужно
// int ox = 0;
// int oy = 0;

// // Используем int вместо double для экономии памяти и скорости
// inline void drawCO2GraphLive(
//   GyverOLED<SSD1306_128x64, OLED_NO_BUFFER>& d,
//   int x, int y,
//   int gx, int gy,
//   int w, int h,
//   int xlo, int xhi, int xinc,
//   int ylo, int yhi, int yinc,
//   uint8_t dig,
//   const char* title,
//   bool& redraw
// ) {
//   if (redraw) {
//     redraw = false;
//     d.clear();
//     d.setScale(1);
//     d.setCursorXY(2, 2);
//     d.invertText(true);
//     d.print(title);
//     d.invertText(false);

//     // ось Y
//     for (int i = ylo; i <= yhi; i += yinc) {
//       int ty = map(i, ylo, yhi, gy + h, gy);
//       if (i == 0)
//         d.line(gx, ty, gx + w, ty);  // основная линия
//       else
//         d.line(gx - 2, ty, gx, ty);  // короткие тики

//       d.setCursorXY(0, ty - 4);
//       d.print(i);
//     }

//     // ось X
//     for (int i = xlo; i <= xhi; i += xinc) {
//       int tx = map(i, xlo, xhi, gx, gx + w);
//       if (i == 0)
//         d.line(tx, gy, tx, gy - h);  // главная вертикаль
//       else
//         d.line(tx, gy, tx, gy + 3);  // тики

//       d.setCursorXY(tx - 4, gy + 6);
//       d.print(i);
//     }
//   }

//   // трансформация координат
//   int sx = map(x, xlo, xhi, gx, gx + w);
//   int sy = map(y, ylo, yhi, gy + h, gy);

//   d.line(ox, oy, sx, sy);
//   d.line(ox, oy - 1, sx, sy - 1);

//   ox = sx;
//   oy = sy;

//   d.update();  // вывод на экран
// }
