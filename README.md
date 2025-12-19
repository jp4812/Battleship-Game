# Battleship Game — C++

## Overview
The Battleship Game is a console-based implementation of the classic Battleship game developed in C++ using object-oriented programming principles.  
The project supports both multiplayer (2v2) and single-player (1vBot) modes and demonstrates the practical application of data structures and game logic.

---

## Game Modes
- 2v2 Multiplayer Mode – Two players compete against each other on separate grids.
- 1vBot Mode – A player competes against an AI-controlled opponent.

---

## Concepts and Data Structures Used
- Object-Oriented Programming
  - Classes and Objects
  - Inheritance and Polymorphism
  - Encapsulation
- Data Structures
  - 2D Arrays – Represent player game boards
  - Vectors – Store ships and attack history
  - Sets / Maps – Track hits, misses, and previously attacked positions

---

## How It Works (Backend Logic)
- Each player is assigned a separate 2D grid representing their board.
- Ships are placed on the grid with validation to prevent overlap and out-of-bound placement.
- During each turn, the player or bot selects a target coordinate.
- The backend logic checks the grid to determine a hit or miss and updates the board state.
- Ship health is tracked, and a ship is marked as destroyed when all its positions are hit.
- The game continues in a turn-based manner until all ships of one player are destroyed.

---

## Game Features
- Validated ship placement
- Hit and miss detection
- Turn-based gameplay
- AI opponent with randomized attack strategy
- Win condition detection
- Console-based board visualization

---

## Tech Stack
- Language: C++
- Programming Paradigm: Object-Oriented Programming
- Interface: Console

---

## How to Run
1. Compile the program:
   ```bash
   g++ main.cpp -o battleship
