# GuildGame

![Screenshot](/ReadmeImages/Screenshot.png)

A third-person tactical RPG with positioning-heavy combat inspired by Divinity: Original Sin and tabletop RPGs. Built in Unreal Engine.

## Combat

On each turn a player can move within their movement circle then either perform a 3-hit combo melee attack
or use a skill. Most skills have some kind of AOE shape, circles, cones, arches. The idea being to position
yourself in order to use these skills effectively.

Currently implemented:
- Free movement + 3-hit melee combo system
- Turn and combat state management via a dedicated Combat Subsystem

In development:
- **BP (Battle Points)** - pips that increases each turn and fuels abilities.
- **AOE Skills** - A battle menu for selecting skills with various AOE shapes. 
- **Engage System** - When a unit attacks another with the melee combo they become engaged.
Engaged units can't move anymore but can attack twice using either their melee combo or skills.
You can break engagment by either defeating the other unit or having an allied unit build up enough
aggro to swap

## Systems
- **Dialogue Subsystem** - Data-driven NPC dialogue via Data Tables and textbox widgets
- **Combat Subsystem** - Manages the turn order, performs battle calculations, broadcasts combat events, checks for win conditions

## Unreal Features Used
- **Unreal's Enhanced Input** - Making use of this for quick mapping swaps between combat, dialogue and exploration modes
- **Widgets** - Used for enemy/player health, damage numbers and textboxes
- **DataTables** - For holding dialogue data
- **Blueprint & C++** - The game uses a mix of BP and C++. Mainly C++ for systems and Blueprint for UI elements
