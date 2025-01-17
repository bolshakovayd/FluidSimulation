////////// fluidlib //////////////////////////////////////////////////////////
// Авторы   : Баталов Семен, Зарина Максудова, Дарья Белоусова              //
// Дата     : ??.??.2020                                                    //
//////////////////////////////////////////////////////////////////////////////

#include "borders2d.h"

using namespace fluid;

////////// class WallBorderModeler2D /////////////////////////////////////////
// Описание : fluidlib2d.h.                                                 //
//////////////////////////////////////////////////////////////////////////////

////////// public ////////////////////////////////////////////////////////////

// (1) Конструктор
WallBorderModeler2D::WallBorderModeler2D()
{
    borrowX_    = 0;
    borrowY_    = 0;
    pressure2D_ = nullptr;
    speed2D_    = nullptr;
    wall2D_     = nullptr;
    interp2D_   = nullptr;
}

// (4) Инициализация всех полей
void WallBorderModeler2D::initialize(BasicTools2D& tools2D, BasicFluid2D& 
                                     fluid2D, WallBorder2D& border2D)
{
    pressure2D_ = &fluid2D.pressure2D;
    speed2D_    = &fluid2D.speed2D;
    wall2D_     = &border2D;
    interp2D_   = &tools2D.interp();
}

// (5) Производит переход в новое состояние, базируясь на текущем
void WallBorderModeler2D::compute()
{
    // Проходим по всем точкам рабочей зоны
    for (int i = 0; i < wall2D_->wall2D.getSizeX(); ++i)
    {
        for (int j = 0; j < wall2D_->wall2D.getSizeY(); ++j)
        {
            if (wall2D_->wall2D.x().area(i, j))
            {
                findBorrowDot(i, j);
                computeSpeed(i, j);
                computePressure(i, j);
            }
        }
    }
}

// (6) Установка значений по умолчанию
void WallBorderModeler2D::clear()
{
    borrowX_    = 0;
    borrowY_    = 0;
    pressure2D_ = nullptr;
    speed2D_    = nullptr;
    wall2D_     = nullptr;
    interp2D_   = nullptr;
}

////////// private ///////////////////////////////////////////////////////////

// (1) Находит координаты точки заимствования для (i, j) элемента
void WallBorderModeler2D::findBorrowDot(int i, int j)
{
    // Смещаемся на "DS" в направлении вектора нормали к поверхности
    borrowX_ = DX * i + wall2D_->wall2D.x().field(i, j) * DS;
    borrowY_ = DY * j + wall2D_->wall2D.y().field(i, j) * DS;
}

// (2) Рассчитывает скорость в (i, j) элементе
void WallBorderModeler2D::computeSpeed(int i, int j)
{
    // Проекции векторов нормали и скорости на разные направления
    Real speedX  = interp2D_->compute(speed2D_->x(), borrowX_, borrowY_);
    Real speedY  = interp2D_->compute(speed2D_->y(), borrowX_, borrowY_);
    Real normX   = wall2D_->wall2D.x().field(i, j);
    Real normY   = wall2D_->wall2D.y().field(i, j);
    Real speedPr = normX * speedX + normY * speedY;
    
    // Находим значение скорости на границе, соотв-е усл-ю прилипания
    speed2D_->x().field(i, j) = - normX * speedPr;
    speed2D_->y().field(i, j) = - normY * speedPr;
}

// (3) Рассчитывает давление в (i, j) элементе
void WallBorderModeler2D::computePressure(int i, int j)
{
    // Используя билинейную интерполяцию находим давление на границе
    pressure2D_->field(i, j) = interp2D_->compute(*(pressure2D_), 
                               borrowX_, borrowY_);
}

////////// class CavityBorderModeler2D ///////////////////////////////////////
// Описание : fluidlib2d.h.                                                 //
//////////////////////////////////////////////////////////////////////////////

////////// public ////////////////////////////////////////////////////////////

// (1) Конструктор
CavityBorderModeler2D::CavityBorderModeler2D()
{
    borrowX_    = 0;
    borrowY_    = 0;
    pressure2D_ = nullptr;
    speed2D_    = nullptr;
    cavity2D_   = nullptr;
    interp2D_   = nullptr;
}

// (4) Инициализация всех полей
void CavityBorderModeler2D::initialize(BasicTools2D& tools2D, 
                                       BasicFluid2D& fluid2D,
                                       CavityBorder2D& border2D)
{
    pressure2D_ = &fluid2D.pressure2D;
    speed2D_    = &fluid2D.speed2D;
    cavity2D_   = &border2D;
    interp2D_   = &tools2D.interp();
}

// (5) Производит переход в новое состояние, базируясь на текущем
void CavityBorderModeler2D::compute()
{
    // Проходим по всем точкам рабочей зоны
    for (int i = 0; i < cavity2D_->pressure2D.getSizeX(); ++i)
    {
        for (int j = 0; j < cavity2D_->pressure2D.getSizeY(); ++j)
        {
            if (cavity2D_->pressure2D.area(i, j))
            {
                findBorrowDot(i, j);
                computeSpeed(i, j);
                computePressure(i, j);
            }
        }
    }
}

// (6) Установка значений по умолчанию
void CavityBorderModeler2D::clear()
{
    borrowX_    = 0;
    borrowY_    = 0;
    pressure2D_ = nullptr;
    speed2D_    = nullptr;
    cavity2D_   = nullptr;
    interp2D_   = nullptr;
}

////////// private ///////////////////////////////////////////////////////////

// (1) Находит координаты точки заимствования для (i, j) элемента
void CavityBorderModeler2D::findBorrowDot(int i, int j)
{
    // Смещаемся на "DS" в направлении вектора нормали к поверхности
    borrowX_ = DX * i + cavity2D_->border2D.x().field(i, j) * DS;
    borrowY_ = DY * j + cavity2D_->border2D.y().field(i, j) * DS;
}

// (2) Рассчитывает скорость в (i, j) элементе
void CavityBorderModeler2D::computeSpeed(int i, int j)
{
    // Проекции векторов нормали и скорости на разные направления
    Real speedX  = interp2D_->compute(speed2D_->x(), borrowX_, borrowY_);
    Real speedY  = interp2D_->compute(speed2D_->y(), borrowX_, borrowY_);
    
    // Находим значение скорости на границе, соответствующее условию 
    // отсутствия какой-либо преграды
    speed2D_->x().field(i, j) = speedX;
    speed2D_->y().field(i, j) = speedY;
}

// (3) Рассчитывает давление в (i, j) элементе
void CavityBorderModeler2D::computePressure(int i, int j)
{
    // Исполюзуя имеющиеся данные, обновляем элементы поля давления
    pressure2D_->field(i, j) = cavity2D_->pressure2D.field(i, j);
}

////////// struct GravityBorder2D ////////////////////////////////////////////
// Описание : fluidlib2d.h.                                                 //
//////////////////////////////////////////////////////////////////////////////

// (1) Конструктор
GravityBorder2D::GravityBorder2D()
{
    gX = 0;
    gY = 0;
}

////////// class GravityBorderModeler2D //////////////////////////////////////
// Описание : fluidlib2d.h.                                                 //
//////////////////////////////////////////////////////////////////////////////

// (1) Конструктор
GravityBorderModeler2D::GravityBorderModeler2D()
{
    force2D_   = nullptr;
    gravity2D_ = nullptr;
}

// (4) Инициализация всех полей
void GravityBorderModeler2D::initialize(BasicFluid2D& fluid2D, 
                                        GravityBorder2D& border2D)
{
    force2D_   = &fluid2D.force2D;
    gravity2D_ = &border2D;
}

// (5) Производит переход в новое состояние, базируясь на текущем
void GravityBorderModeler2D::compute()
{
    // Проходим по всем точкам рабочей зоны
    for (int i = 0; i < force2D_->getSizeX(); ++i)
    {
        for (int j = 0; j < force2D_->getSizeY(); ++j)
        {
            if (force2D_->x().area(i, j))
            {
                // Обновляем значение внешней силы в точке (i, j)
                force2D_->x().field(i, j) = gravity2D_->gX;
                force2D_->y().field(i, j) = gravity2D_->gY;
            }
        }
    }
}

// (6) Установка значений по умолчанию
void GravityBorderModeler2D::clear()
{
    force2D_   = nullptr;
    gravity2D_ = nullptr;
}

////////// class SpeedBorderModeler2D //////////////////////////////////////
// Описание : fluidlib2d.h.                                                 //
//////////////////////////////////////////////////////////////////////////////

// (1) Конструктор
SpeedBorderModeler2D::SpeedBorderModeler2D()
{
    speed2D_     = nullptr;
    wallSpeed2D_ = nullptr;
}

// (4) Инициализация всех полей
void SpeedBorderModeler2D::initialize(BasicFluid2D& fluid2D, 
                                      SpeedBorder2D& border2D)
{
    speed2D_     = &fluid2D.speed2D;
    wallSpeed2D_ = &border2D;
}

// (5) Производит переход в новое состояние, базируясь на текущем
void SpeedBorderModeler2D::compute()
{
    // Ссылаемся на поле скоростей стенки
    VField2D& wS2D = wallSpeed2D_->wallSpeed2D;
    
    // Проходим по всем точкам рабочей зоны
    for (int i = 0; i < wS2D.getSizeX(); ++i)
    {
        for (int j = 0; j < wS2D.getSizeY(); ++j)
        {
            if (wS2D.x().area(i, j))
            {
                // Обновляем значение скорости жидкости в точке (i, j)
                speed2D_->x().field(i, j) += wS2D.x().field(i, j);
                speed2D_->y().field(i, j) += wS2D.y().field(i, j);
            }
        }
    }
}

// (6) Установка значений по умолчанию
void SpeedBorderModeler2D::clear()
{
    speed2D_     = nullptr;
    wallSpeed2D_ = nullptr;
}

////////// class BorderModeler2D /////////////////////////////////////////////
// Описание : fluidlib2d.h.                                                 //
//////////////////////////////////////////////////////////////////////////////

// (4) Инициализация всех полей
void BorderModeler2D::initialize(BasicTools2D& tools2D, Fluid2D& fluid2D, 
                                 Border2D& border2D)
{
    wall2D_.initialize(tools2D, fluid2D.basic2D, border2D.wall2D);
    speed2D_.initialize(fluid2D.basic2D, border2D.speed2D);
    cavity2D_.initialize(tools2D, fluid2D.basic2D, border2D.cavity2D);
    gravity2D_.initialize(fluid2D.basic2D, border2D.gravity2D);
}

// (5) Производит переход в новое состояние, базируясь на текущем
void BorderModeler2D::compute()
{
    // Порядок имеет значение
    wall2D_.compute();
    cavity2D_.compute();
    speed2D_.compute();
    gravity2D_.compute();
}

// (6) Установка значений по умолчанию
void BorderModeler2D::clear()
{
    wall2D_.clear();
    cavity2D_.clear();
    speed2D_.clear();
    gravity2D_.clear();
}