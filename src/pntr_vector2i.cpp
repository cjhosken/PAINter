#include "pntr_vector2i.h"
// Basic constructor methods. either initialize as zero or the 1-2 input variables.
PNTR_Vector2I::PNTR_Vector2I() : x(0), y(0) {};
PNTR_Vector2I::PNTR_Vector2I(int i) : x(i), y(i) {};
PNTR_Vector2I::PNTR_Vector2I(int i, int j) : x(i), y(j) {};