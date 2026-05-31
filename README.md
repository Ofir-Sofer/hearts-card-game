# Hearts Card Game

A terminal-based implementation of the Hearts card game written in C, supporting 2–4 players. Any slots not filled by human players are taken by computer-controlled players with rule-based logic.

Built as part of an intensive C/C++ embedded systems bootcamp at Experis Academy.

## Features

- Full Hearts card game rules
- Configurable number of human and computer-controlled players (2–4 total)
- Computer players use rule-based logic to handle game situations
- Custom generic vector data structure reused across the game
- Modular design — Card, Deck, Player, Round, Game, and UI layers are cleanly separated
- Built with a Makefile

## Building and Running

```bash
make
./hearts
```

## Project Structure

| File | Description |
|------|-------------|
| `Hearts_main.c` | Entry point |
| `Game.c/h` | Game loop and state management |
| `Round.c/h` | Round logic |
| `Player.c/h` | Player management (human and computer-controlled) |
| `Deck.c/h` | Deck creation and shuffling |
| `Card.c/h` | Card representation |
| `HeartsUI.c/h` | Terminal UI |
| `Vector.c/h` | Generic dynamic array implementation |
| `GenericVectorBubbleSort.c/h` | Generic bubble sort using the vector |

## Design Highlights

- **Generic vector** — a reusable dynamic array implemented from scratch in C, used throughout the game for hand management and sorting
- **Rule-based computer players** — each computer-controlled player follows a set of rules to decide which card to play based on the current game state
- **Separation of concerns** — game logic, UI, and data structures are fully decoupled

## Notes

This project was written in C as part of a bootcamp focused on systems programming, data structures, and algorithms. The generic vector and sort implementations demonstrate manual memory management and generic programming patterns in C using `void*`.