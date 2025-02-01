# Project Title: Craigslist CarCache
## COP3530 Project 3
---
## Problem Statement:
- `Due to the volatility of the used car market, it is difficult to determine a fair value for a used vehicle. In recent years, the Kelley Blue Book, which used to be the gold standard for used car values, has become obsolete and completely unreliable. This means there must be an updated, accurate way to see what cars should be listed for and purchased for in today’s climate. A visualization and easy way to compare vehicle values will allow car buyers and sellers to appropriately price and purchase vehicles in a scalable way`


---
### Team Name : *The Other Guys*
### Team Number : **3**
### Team Members: *Alexander Fisher | Jacob Frankel | Meynard Guillermo*
### Tools : *C++ / ReactJs / HTML / CSS / Tailwind*

### Data 

+ Kaggle Craigslist **[LINK](https://www.kaggle.com/datasets/austinreese/craigslist-carstrucks-data)**
---

**Installation Guide**

1. Download the ***[Dataset](https://www.kaggle.com/datasets/austinreese/craigslist-carstrucks-data)*** and extract vehicles.csv to `./backend/data`

2. If you have installed *`g++`* & *`Make`*, go to 3.
    - **To Install Make and g++**
        1. Visit the MSYS2s [website](https://www.msys2.org/) and download the installer
            - Open MSYS2 shell and run `pacman -Syu`
            - Then run `pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-make`
        2. Once MSYS2 is downloaded, use the msys2.exe shell to run `pacman -S make`
        3. Navigate to the project root directory using the *msys2.exe* shell

3. If `g++ --version && make --version` return the most up to date version, open the terminal in the root directory, then run `cd backend`

4. Finally, run `make && ./build/car_cache`

5. For the most effective usage, full-screen the terminal

** Video Demo **

[Video Link](https://www.youtube.com/watch?v=QvmcvuQ6Jyk)
