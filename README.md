# Shegelmetris

## RUS

��� ������� ���� �������, ���������� ���� � �������� ��������� ������� �� ����� ������ ������. ���� �������� �� C++ � ������ OpenGL ��� ���������� � ����������� �����������. �������������� � ������� ������� ���������� - [GLM](http://glm.g-truc.net/) (�������������� ���� � ������) � [cereal](http://uscilab.github.io/cereal/) (������������ ����� ��������).

### ���������

���� ��������� �������� � Visual Studio 2015 Community Edition. Solution-���� ��������� � �����.

### �������������

����� ��������� ������� 32-������ release-������, �������� ���� Game/Shegelmetris.exe. ��������� ��������� � Game/Config.xml.

### ���������

- Debug/ - �������� ���������� ��� ����������. ���� ������ ����������������� ������, ��������� ����� ���������� �����.
- Game/ - ���������������� �������� ���� � runtime-������� ���� (������ � ���� ��������). ��� ������� ���������� ����������.
- Shegelmetris/ - ���� ������� Visual Studio � ���������.
  * Shegelmetris/Src/ - �������� �����.
    * Shegelmetris/Src/ext/ - ������� ����������.
- Temp/ - ��������� ���������� ��� Visual Studio.

### �������������

- [GLM](http://glm.g-truc.net/) - �������������� ����������. �������� - [The Happy Bunny License / MIT](http://glm.g-truc.net/copying.txt).
- [cereal](http://uscilab.github.io/cereal/) - ���������� ��� ������������. �������� - [BSD](https://opensource.org/licenses/BSD-3-Clause).

### ��������

������ �������� ������������ ����������. �� ������ ������ � ��� ��, ��� ������. ����������, �������� ������ **�������������** ��� ��������� ���������� � ��������� ������� ���������.

## ENG

This is a simple Tetris clone game, written by me as a part of technical test while I was looking for a job. It was written with C++ and pure OpenGL. The only external libraries in use are [GLM](http://glm.g-truc.net/) (math library) and [cereal](http://uscilab.github.io/cereal/) (settings serialization).
This project is fully open source and in public domain. Feel free to use it however you want (just make sure to read **Credits** section about external libraries).

### Installation

The game is written fully in Visual Studio 2015 Community Edition. VS solution is located at the root directory.

### Usage

To run the compiled 32-bit release version, launch Game/Shegelmetris.exe. Game settings are in the Game/Config.xml.

### Structure

- Debug/ - output folder for the compilation. If you are going to recompile the project, look for the binaries here.
- Game/ - compiled binary file and game resources (icon and settings XML). This is the working directory of the application.
- Shegelmetris/ - VS project files and sources.
  * Shegelmetris/Src/ - source files.
    * Shegelmetris/Src/ext/ - external libraries.
- Temp/ - temporary folder for Visual Studio stuff.

### Credits

- [GLM](http://glm.g-truc.net/) - math library. Licensed under [the Happy Bunny License and MIT License](http://glm.g-truc.net/copying.txt).
- [cereal](http://uscilab.github.io/cereal/) - serialization library. Lisensed under [the BSD license](https://opensource.org/licenses/BSD-3-Clause).

### License

This project is in the public domain (or the equivalent where that is not possible). You can do anything you want with it. Please, see **Credits** for the information about external libraries.