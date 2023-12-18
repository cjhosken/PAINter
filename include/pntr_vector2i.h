#ifndef PNTR_VECTOR2I_H
#define PNTR_VECTOR2I_H

// PNTR_Vector2I is similar to a 2d coordinate. I made a class for it as I use alot of 2d coordinates throughout the application.
class PNTR_Vector2I {
    public:
        int x, y;

    PNTR_Vector2I();
    PNTR_Vector2I(int i);
    PNTR_Vector2I(int i, int j);
};

#endif