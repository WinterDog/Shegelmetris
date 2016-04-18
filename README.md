# Shegelmetris

## RUS

Это простой клон Тетриса, написанный мной в качестве тестового задания во время поиска работы. Игра написана на C++ и чистом OpenGL без расширений и графических фреймворков. Использованные в проекте внешние библиотеки - [GLM](http://glm.g-truc.net/) (математические типы и методы) и [cereal](http://uscilab.github.io/cereal/) (сериализация файла настроек).

### Установка

Игра полностью написана в Visual Studio 2015 Community Edition. Solution-файл находится в корне.

### Использование

Чтобы запустить готовую 32-битную release-версию, откройте файл Game/Shegelmetris.exe. Настройки прописаны в Game/Config.xml.

### Структура

- Debug/ - выходная директория для компиляции. Если решите перекомпилировать проект, бинарники будут находиться здесь.
- Game/ - скомпилированный бинарный файл и runtime-ресурсы игры (иконка и файл настроек). Это рабочая директория приложения.
- Shegelmetris/ - файл проекта Visual Studio и исходники.
  * Shegelmetris/Src/ - исходные файлы.
    * Shegelmetris/Src/ext/ - внешние библиотеки.
- Temp/ - временная директория для Visual Studio.

### Благодарности

- [GLM](http://glm.g-truc.net/) - математическая библиотека. Лицензия - [The Happy Bunny License / MIT](http://glm.g-truc.net/copying.txt).
- [cereal](http://uscilab.github.io/cereal/) - библиотека для сериализации. Лицензия - [BSD](https://opensource.org/licenses/BSD-3-Clause).

### Лицензия

Проект является общественным достоянием. Вы можете делать с ним всё, что хотите. Пожалуйста, прочтите раздел **Благодарности** для получения информации о лицензиях внешних библиотек.

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