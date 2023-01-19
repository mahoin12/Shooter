# Shooter

Developed with Unreal Engine 5


```ruby
Step 1
```
```ruby
• Create a new project by selecting the "First Person" template project with C++.
• Implement a limited ammo mechanic to the player. The ammo amount is the number of bullets that the player can use by shooting. The maximum amount of ammo should be editable in the editor.
• Set the minimum ammo amount to 0 and set the maximum ammo amount however you
want. If the player gets out of ammo, s/he cannot fire the weapon.
• Create pick-up objects that increase the ammo amount of the player on the overlap. Each
pick up carries a different amount of ammo, and it should be editable in the editor.
```
```ruby
Step 2
```
```ruby
• Place two characters (which you made Step1) into the scene.
• Add a function that allows users to switch between characters by pressing "P".
```
```ruby
Step 3
```
```ruby
• Create a UI that displays the ammo amount of the player. Then, when players switch to
other players, UI shows the ammo of the controlling character.
```
```ruby
Step 4
```
```ruby
• Create a dummy stable (non-moving) enemy.
• The enemy has 100 health points.
• Implement a health bar above enemies that indicates current health and full health.
• Implement the shooting mechanic. Players can shoot with the weapon and hit the enemy.
• When players hit the enemy, enemy health should decrease randomly between 5 and 10
health points.
• When enemy health is equal and lower than 0, the enemy will die.
```
```ruby
Step 5
```
```ruby
• Create another type of enemy that has the same functionalities as the enemy you created
before (in Step 4). But this enemy will walk to the players when they entered the enemy’s
range (You can decide the range).
```
```ruby
Step 6
```
```ruby
• Create another pick-up object that increases the players’ maximum amount of ammo.
```
