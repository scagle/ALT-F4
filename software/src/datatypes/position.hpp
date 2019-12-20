#pragma once

enum PositionType
{
    ROW_COL,
    X_Y
};

struct Position
{
    int a;
    int b;
    PositionType t;

    Position getXY() { return (t == ROW_COL) ? Position{ b, a, ROW_COL } 
        : Position{ a, b, ROW_COL }; }
    Position getRC() { return (t == X_Y)     ? Position{ b, a, X_Y }     
        : Position{ a, b, X_Y }; }
};
