<div id="top"></div>


<!-- PROJECT SHIELDS -->
[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![Apache License][license-shield]][license-url]



<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://github.com/0verEngineer/cuteInjector">
    <img src="resources/icon.png" alt="Logo">
  </a>

  <h3 align="center">cuteInjector</h3>

  <p align="center">
    Simple dll injector built with QT
    <br />
    <br />
    <a href="https://github.com/0verEngineer/cuteInjector/issues">Report Bug</a>
    ·
    <a href="https://github.com/0verEngineer/cuteInjector/issues">Request Feature</a>
  </p>
</div>



<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

- The Injector and the x86Bridge are two different CMake Projects because of the different architecture
- The x86Bridge is only used to get the address of LoadLibraryA in a x86 Process but that will change in the future


<p align="right">(<a href="#top">back to top</a>)</p>



### Built With

- [QT](https://qt.io/)
- [nlohmann json](https://github.com/nlohmann/json)

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- GETTING STARTED -->
## Getting Started

### Prerequisites
- QT

### Visual Studio
- Install the QT Extension
- Open the Directory with Visual Studio
- Change the CMakeSettings.json accordingly. The CMAKE_PREFIX_PATH should be the only one you need to set.

### CLion Setup
- Set the CMAKE_PREFIX_PATH in the Debug and Release Profiles of the CMake Settings. There is an Environment field.

### CMAKE_PREFIX_PATH
- Is the Path of the QT Installation that should be used. For Example: `C:\Qt\6.3.0\msvc2019_64`

### Release dependencies
- run `windeployqt.exe --release P:\ath\to\dir\with\cuteInjector.exe`



<p align="right">(<a href="#top">back to top</a>)</p>



<!-- ROADMAP -->
## Roadmap

- [x] LoadLibrary + CreateThread Injection
- [x] Process Icons
- [ ] (File-)Logging, Log-Window
- [ ] Rethink x86, x64 situation
- [ ] Add more injection methods
    - [ ] Manual Mapping
    - [ ] LdrLoadDll
- [ ] Add more thread-creation methods
    - [ ] Kernel callback


<p align="right">(<a href="#top">back to top</a>)</p>



<!-- LICENSE -->
## License

Distributed under the Apache License 2.0. See `LICENSE` for more information.

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- CONTACT -->
## Contact

Julian Hackinger - dev@hackinger.io

Project Link: [https://github.com/0verEngineer/cuteInjector](https://github.com/0verEngineer/cuteInjector)

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- MARKDOWN LINKS & IMAGES -->
[contributors-shield]: https://img.shields.io/github/contributors/0verEngineer/cuteInjector.svg?style=for-the-badge
[contributors-url]: https://github.com/0verEngineer/cuteInjector/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/0verEngineer/cuteInjector.svg?style=for-the-badge
[forks-url]: https://github.com/0verEngineer/cuteInjector/network/members
[stars-shield]: https://img.shields.io/github/stars/0verEngineer/cuteInjector.svg?style=for-the-badge
[stars-url]: https://github.com/0verEngineer/cuteInjector/stargazers
[issues-shield]: https://img.shields.io/github/issues/0verEngineer/cuteInjector.svg?style=for-the-badge
[issues-url]: https://github.com/0verEngineer/cuteInjector/issues
[license-shield]: https://img.shields.io/github/license/0verEngineer/cuteInjector.svg?style=for-the-badge
[license-url]: https://github.com/0verEngineer/cuteInjector/blob/master/LICENSE.txt
