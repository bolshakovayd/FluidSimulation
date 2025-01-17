Библиотека файлового ввода\вывода данных модели.

##Задание:
Требуется перегрузить операторы "<<", ">>" для чтения и записи в 
бинарный файл следующих объектов библиотеки "FluidLib":

> basic.h
    1) struct Point

> components2d.h
    1) class SField2D
    2) class VField2D

> fluids2d.h
    1) struct BasicFluid2D
    2) struct Fluid2D

> borders2d.h
    1) struct WallBorder2D
    2) struct CavityBorder2D
    3) struct GravityBorder2D
    4) struct SpeedBorder2D
    7) struct Border2D

> fluidlib2d.h
    1) struct Model2D

##Замечание:
    Естественно писать последовательно, в отдельном пространстве имен "fio". 
Не обязательно перегружать именно эти операторы, можно написать несколько 
классов, методы которых будут предоставлять нужный функционал (Можно и так, 
и так. Как удобнее).
    Приветствуется соблюдение правил оформления кода (См. 
Documents\CodeStyleCPP.txt). Описание всех классов и структур есть в файлах. 
Возможны малые изменения в расположении файлов, но это уже вряд ли.

##Расположение:
    Библиотека должна содержаться в папке "Libraries\FioLib". Она должна 
включать три подпапки: "headers", "source", "objects", и текстовый файл 
"readme.txt". Содержимое всех папок очевидно, например, в папке "objects" 
находятся все объектные файлы. Если требуется изменить структуру папок, то 
это можно сделать.

##Ссылки:
1) Репозиторий Зарины: https://github.com/ZarinaMaks/FluidSimulation
2) Мой репозиторий:    https://github.com/st076569/FluidSimulation

##Литература:
    Вся полезная литература пока находится в папке "Literature".
