# This repository is archived and not maintained
# Development is continued on [SoCMake](https://github.com/HEP-SoC/SoCMake) repository

<!-- Improved compatibility of back to top link: See: https://github.com/Risto97/VeriSC/pull/73 -->
<a name="readme-top"></a>
<!--
*** Thanks for checking out the Best-README-Template. If you have a suggestion
*** that would make this better, please fork the repo and create a pull request
*** or simply open an issue with the tag "enhancement".
*** Don't forget to give the project a star!
*** Thanks again! Now go create something AMAZING! :D
-->



<!-- PROJECT SHIELDS -->
<!--
*** I'm using markdown "reference style" links for readability.
*** Reference links are enclosed in brackets [ ] instead of parentheses ( ).
*** See the bottom of this document for the declaration of the reference variables
*** for contributors-url, forks-url, etc. This is an optional, concise syntax you may use.
*** https://www.markdownguide.org/basic-syntax/#reference-style-links
-->
[![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![MIT License][license-shield]][license-url]
<!-- [![LinkedIn][linkedin-shield]][linkedin-url] -->



<!-- PROJECT LOGO -->
<br />
<div align="center">
  <!-- <a href="https://github.com/Risto97/VeriSC"> -->
  <!--   <img src="images/logo.png" alt="Logo" width="80" height="80"> -->
  <!-- </a> -->

  <h3 align="center">VeriSC</h3>

  <p align="center">
    SystemC-UVM verification framework and CMake based HW build system
    <br />
    <!-- <a href="https://github.com/Risto97/VeriSC"><strong>Explore the docs »</strong></a> -->
    <br />
    <br />
    <!-- <a href="https://github.com/Risto97/VeriSC">View Demo</a> -->
    <!-- · -->
    <a href="https://github.com/Risto97/VeriSC/issues">Report Bug</a>
    ·
    <a href="https://github.com/Risto97/VeriSC/issues">Request Feature</a>
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

[![Product Name Screen Shot][product-screenshot]](https://example.com)

VeriSC provides an easy way to verify your hardware with SystemC, with an option to use SystemC-UVM and additional libraries to enhance your verification environment.
It is possible to simulate your RTL code with Verilator or Synopsys VCS simulators.

The project is composed of multiple parts:
* CMake build system for RTL and/or SystemC simulation, provides a structure ways for organizing your RTL projects and testbenches with CMake recipes, making IP blocks easy to package and distribute.
* It allows proper SystemC verification of RTL code, similar to what SystemVerilog can provide. It supports UVM enviroments, Constraint Randomization, Coverage, RTL simulation using external libraries and tools. All that at no cost using only open source and free tools and libraries, with an optional support for commercial RTL simulators.
* Several Universal Verification Components (UVCs) are provided for standard interconnect protocols (APB, I2C, Picorv32 NMI... tbc)

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- ### Built With -->
<!--  -->
<!-- This section should list any major frameworks/libraries used to bootstrap your project. Leave any add-ons/plugins for the acknowledgements section. Here are a few examples. -->
<!--  -->
<!-- * [![Next][Next.js]][Next-url] -->
<!-- * [![React][React.js]][React-url] -->
<!-- * [![Vue][Vue.js]][Vue-url] -->
<!-- * [![Angular][Angular.io]][Angular-url] -->
<!-- * [![Svelte][Svelte.dev]][Svelte-url] -->
<!-- * [![Laravel][Laravel.com]][Laravel-url] -->
<!-- * [![Bootstrap][Bootstrap.com]][Bootstrap-url] -->
<!-- * [![JQuery][JQuery.com]][JQuery-url] -->
<!--  -->
<!-- <p align="right">(<a href="#readme-top">back to top</a>)</p> -->
<!--  -->
<!--  -->
<!--  -->
<!-- GETTING STARTED -->
## Getting Started

VeriSC will build all of the needed dependencies automatically, there are only 2 dependencies that will not be built: 
* Relatively recent version of C compiler that can compile GCC 12.2
* CMake version 25.1 or greater
* Autotools

### Prerequisites

CMake can also be installed by running the script scripts/install_cmake.sh
After which CMake will be installed and source.sh will be generated to add CMake to path. TODO change

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Installation

In order to install VeriSC you need to follow these steps:

1. Clone the repo
   ```sh
   git clone https://github.com/Risto97/VeriSC.git
   ```
2. Make a build directory
   ```sh
   cd VeriSC
   mkdir build
   ```
3. Invoke CMake to configure the build, provide CMAKE_INSTALL_PREFIX=\<path\> pointing to the path you want VeriSC installed, optional variables are VCS_HOME=\<path-to-vcs\> that points to your VCS installation directory and CMAKE_CXX_STANDARD=[11,14,17,20] specifying the C++ standard in which to compile the libraries
   ```sh
   cmake -DCMAKE_INSTALL_PREFIX=/tools/verisc -DVCS_HOME=/path/to/vcs ../
   ```
4. After CMake has finished without errors, you can build and install the project, running with -j [number of jobs] will speed up the installation
    ```sh
    make -j16 install
    ```
5. It is required to set the enviroment variable to point to the installation directory of VeriSC, TODO change VERISC_HOME -> VERISC_HOME
    ```sh
    export VERISC_HOME=/tools/verisc
    ```

Now the VeriSC is installed with all of the libraries and tools

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- USAGE EXAMPLES -->
## Examples

There are few examples that are provided in the repository.
They are placed in examples directory.

All the examples can be run by following these steps

1. CD into the example directory and create a build directory
    ```sh
    cd examples/nmi2apb
    mkdir build
    cd build
    ```
2. Configure the project with CMake
    ```sh
    cmake ../
    ```
3. Run the simulation with Verilator or VCS, with make run use -j [number of jobs] to accellerate building
    ```sh
    make -j16 run  # Verilator
    make vrun # VCS
    ```

#### nmi2apb

Example to show how to verify a simple RTL module with SystemC-UVM environment using constrained randomization and functional coverage. The RTL can be simulated with Verilator or VCS

#### simple_sc_verilog

It is a simplest example on how to simulate RTL together with SystemC testbench, but without UVM and other libraries

<!-- _For more examples, please refer to the [Documentation](https://example.com)_ -->

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- ROADMAP -->
## Roadmap

- [X] Move Verilating in build phase from configure phase
- [ ] Finalize CRAVE support
    - [ ] Allow different constraint solvers, currently Z3 and CUDD
- [ ] Add Additional Examples
    - [ ] No verilog example
    - [ ] Multiple verilog top level connected with SystemC
    - [ ] CRAVE example
- [ ] VeriSC installation as CMake Package
- [ ] Support Verilator version 5 and greater
    - [ ] Support C++ 20
- [ ] Integrate [SC enhance](https://github.com/verificationcontractor/sc_enhance) for better forks in SystemC
- [ ] Clang support
- [ ] Xcelium support

See the [open issues](https://github.com/Risto97/VeriSC/issues) for a full list of proposed features (and known issues).

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- CONTRIBUTING -->
<!-- ## Contributing -->
<!--  -->
<!-- Contributions are what make the open source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**. -->
<!--  -->
<!-- If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue with the tag "enhancement". -->
<!-- Don't forget to give the project a star! Thanks again! -->
<!--  -->
<!-- 1. Fork the Project -->
<!-- 2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`) -->
<!-- 3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`) -->
<!-- 4. Push to the Branch (`git push origin feature/AmazingFeature`) -->
<!-- 5. Open a Pull Request -->
<!--  -->
<!-- <p align="right">(<a href="#readme-top">back to top</a>)</p> -->
<!--  -->
<!--  -->
<!--  -->
<!-- LICENSE -->
## License
Copyright CERN, ALL RIGHTS RESERVED. 

See `LICENSE` for more information.

Temporary license until I decide on more permissive one

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- CONTACT -->
## Contact

Your Name - risto.pejasinovic@gmail.com - risto.pejasinovic@cern.ch

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- ACKNOWLEDGMENTS -->
## List of libraries and tools used

* [SystemC](https://github.com/accellera-official/systemc)
* [SystemC-UVM](https://www.accellera.org/downloads/drafts-review)
* [SystemC Verification](https://www.accellera.org/downloads/drafts-review)
* [Verilator](https://www.veripool.org/verilator/)
* [FC4SC](https://github.com/amiq-consulting/fc4sc)
* [CRAVE](https://github.com/agra-uni-bremen/crave)
* [VCS](https://www.synopsys.com/verification/simulation/vcs.html)

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/Risto97/VeriSC.svg?style=for-the-badge
[contributors-url]: https://github.com/thneildrew/Best-README-Template/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/Risto97/VeriSC.svg?style=for-the-badge
[forks-url]: https://github.com/Risto97/VeriSC/network/members
[stars-shield]: https://img.shields.io/github/stars/Risto97/VeriSC.svg?style=for-the-badge
[stars-url]: https://github.com/Risto97/VeriSC/stargazers
[issues-shield]: https://img.shields.io/github/issues/Risto97/VeriSC.svg?style=for-the-badge
[issues-url]: https://github.com/Risto97/VeriSC/issues
[license-shield]: https://img.shields.io/github/license/Risto97/VeriSC.svg?style=for-the-badge
[license-url]: https://github.com/Risto97/VeriSC/blob/master/LICENSE
<!-- [linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555 -->
<!-- [linkedin-url]: https://linkedin.com/in/othneildrew -->
[product-screenshot]: images/screenshot.png
[Next.js]: https://img.shields.io/badge/next.js-000000?style=for-the-badge&logo=nextdotjs&logoColor=white
[Next-url]: https://nextjs.org/
[React.js]: https://img.shields.io/badge/React-20232A?style=for-the-badge&logo=react&logoColor=61DAFB
[React-url]: https://reactjs.org/
[Vue.js]: https://img.shields.io/badge/Vue.js-35495E?style=for-the-badge&logo=vuedotjs&logoColor=4FC08D
[Vue-url]: https://vuejs.org/
[Angular.io]: https://img.shields.io/badge/Angular-DD0031?style=for-the-badge&logo=angular&logoColor=white
[Angular-url]: https://angular.io/
[Svelte.dev]: https://img.shields.io/badge/Svelte-4A4A55?style=for-the-badge&logo=svelte&logoColor=FF3E00
[Svelte-url]: https://svelte.dev/
[Laravel.com]: https://img.shields.io/badge/Laravel-FF2D20?style=for-the-badge&logo=laravel&logoColor=white
[Laravel-url]: https://laravel.com
[Bootstrap.com]: https://img.shields.io/badge/Bootstrap-563D7C?style=for-the-badge&logo=bootstrap&logoColor=white
[Bootstrap-url]: https://getbootstrap.com
[JQuery.com]: https://img.shields.io/badge/jQuery-0769AD?style=for-the-badge&logo=jquery&logoColor=white
[JQuery-url]: https://jquery.com 

