<img src="https://cdn.rawgit.com/emilwallner/Corewar/master/images/corewarlogo.svg" width="100%">

---

[**Core War was inspired by a malicious virus written in the 80’s.**](https://en.wikipedia.org/wiki/Core_War) To deal with the self-replicating virus, a white hat hacker invented Reaper. It was a virus designed to spread and eliminate the malware. He fought fire with fire.

This inspired A. K. Dewdney to coin the idea for Core War.

The idea was simple. You compete by designing viruses to overtake a computer. You win by protecting your own program and overwriting your opponent's programs. This is all happening on a virtual computer. Think, a simple computer within your computer. 

This is what is looks like in action:

![Alt text](http://g.recordit.co/pyyAggYcWm.gif "Optional title")

Let’s focus on the high-level game dynamics: 

- **The game board**, the memory of our virtual computer. It’s represented in a 64 X 64 grid of bytes.
- **The players**, small programs represented in different colors. The white parts have blank memory.
- **Cursors**, the moving parts with inverted color. They read from the game board. 

The cursors have a couple of features. They can jump on the game board, store and write values, and clone themselves. The cursors belong to no-one, they just read from the game board.

When the game starts, all players have one cursor at the beginning of their program. The game advances as the cursors read the commands stored in their programs. If they end up on a faulty command or a blank memory, it moves to the next byte. 

Below, we see how the pink player starts by cloning their cursors. It then starts attacking the blue player.

![Alt text](http://g.recordit.co/Y9r9E78FVY.gif "Optional title")

Let’s get into a little bit more depth. 

Every byte you see, the pairs of numbers or letters, are represented in hexadecimal. Each hexadecimal has the value of a digit, from 0 - 255. 

There are 16 operations in the game. The operations decide if the cursor should jump to a different address, clone itself and so on. The first 16 hexadecimal are [**coding bytes**](https://docs.google.com/spreadsheets/d/1IuQv2yfx7ewS3d6H2KyDu7N9U67u3mhFBy8W1XUhxuM/edit?usp=sharing), they all store a different operation. The remaining digits, 17 - 255, are regular numbers. 

The coding byte decides which operation to execute and how many bytes to read. In the image above, the pink player goes to the hexadecimal **0c**. It's 12 in decimal, telling the cursor to clone itself. The two bytes after decide where the cloned cursor starts. 

![Alt text](http://g.recordit.co/XQdhVmqvrV.gif "Optional title")

There are three main components to determine who wins: 

- **Game rounds**, every game round is measured in cycles. It determines how much each cursor can read from the game board. 
- **Lives**, if a cursor reads a player life from the game board, they are given a life for that round. These are visualized in the colorful progress bars. 
- **Cycle to die**, for every game round, the number of bytes a cursor can read from the game board is reduced.

You win if your program is the last one to receive a life. 

There are more nuances to the game that I don’t cover. The best way to learn about them is to run it with the below code.  

<br>
<br>

## Executing Core War

Open your terminal and make it full-screen, copy paste the below, and hit enter.

```shell
> git clone https://github.com/livankrekh/corewar.git
> cd corewar
> make
> ./corewar -v -n 1 ./Bots/players/helltrain.cor -n 2 ./Bots/players/bee_gees.cor
```
Once the cycle to die reaches 0, it will announce a winner. Then you can press any key to leave the program.

The usage for the Core War game: 

```
Usage:
 	Example: ./corewar -v -n -1 <player1_file>.cor
 	GUI: ./corewar -v [Players]
 	Default Player Numbers: 1, 2, 3, 4...
 	Change Player Number: -n -1 <player1_file>.cor
 	Dump: ./corewar -dump 300 [Players](prints the memory after 300 cycles)
 	Game on! 👾 🤖 🔫
```
If you want to create and then compile players, use the asm: 

```
./asm [<player_file>.s] (Compiles bot from RedCode to binary -> <player_file>.cor)
```

<br>
<br>


## Technical Implementation Overview

I did Core War with three friends: [@ngulya](https://github.com/ngulya), [@beardings](https://github.com/beardings), and [@ybelilov](https://github.com/ybelilov).

The Core War consists of players that are written in assembly, a compiler to turn them into binary, and the virtual computer to run the programs on. 

I focussed on the main architecture of the virtual machine (VM), the game dynamics, implementing three operations, handling the cursors and implementing the graphical user interface (GUI).  

The program is written using read, open, write, malloc, free and exit from **stdlib.h**. We used the **ncurses** library to create the GUI.  

This is an example of a simple player, a normal player has around 150 lines of assembly code:


```assembly
    .name "zork"
    .comment "I'M ALIIIIVE"
    
    l2:          sti r1,%:live,%1
                 and r1,%0,r1
    
    live:        live %1
                 zjmp %:live
```

This complies to binary which is later represented in hexadecimal. The above program looks like this when it’s compiled: 

```binary
    0000000 00 ea 83 f3 7a 6f 72 6b 00 00 00 00 00 00 00 00
    0000010 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    *
    0000080 00 00 00 00 00 00 00 00 00 00 00 17 49 27 4d 20
    0000090 41 4c 49 49 49 49 56 45 00 00 00 00 00 00 00 00
    00000a0 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
    *
    0000890 0b 68 01 00 0f 00 01 06 64 01 00 00 00 00 01 01
    00008a0 00 00 00 01 09 ff fb
```

The first part includes an identification code and the name, followed by the comment, and ending with the program which will be stored in the memory of the virtual machine. 

<br>
<br>

Current project description was taken from Corewar description of @emilwallner (https://github.com/emilwallner/Corewar) and modified.
