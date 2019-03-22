# SteamVR Input Guide

## What is SteamVR Input?

- A universal keybind system for all controllers (in steamVR)
- A way for You to share/find keybind layouts.
- A way for developers to easily program for all controllers [needs a driver]

## What do I need to know?

- You don’t bind buttons like a/b. you bind “actions”
- An action is simply just that an action…. Like “jump,” “shoot,” or “interact”
 - Action Have Types such as : Digital (on/off), Analog (range of values 0-100 say), or special ones like Vibration (specifically for haptics)

## Okay Cool? How do I do this?

1. Go to SteamVR dashboard->Steam->Controller Bindings OR go to http://127.0.0.1:8998/dashboard/controllerbinding.html from a desktop browser WHILE steamVR is running
(pic)
2. Select your application you want to modify keybinds (Advanced Settings will currently always be at the bottom)
3. Select your Controller, (the controller must be on)
4. Select your binding.
5. Now We Are Ready to Bind our Actions!

### Binding UI

(pic)
- A – Action sets, these tabs essentially let programmers have different keybinds for different events… say you may want different controls if you are driving a car versus running around. [In Advanced Settings we simply use them as a grouping utility, and ALL action sets are active ALL the time.]
- B – Input options – your “buttons”
- C – Chords/ Special Binds. More on these later.
- D – Controller Diagram, visually shows you the button location on a depiction of the controller

### My First Bind

In this example we are going to use the "grip" button on a vive wand.

- First WE click the  [+] on the grip line to create an emtpy binding
(pic)
- We then select the behavior of the button.
(pic)
 - Button – action is active when button is pressed
 - Toggle Button – Action toggles state when button is pressed i.e. on until pressed again
 - Set Analog Action – for Actions that are not binary i.e. trigger is pressed 50%
- Now we select our action for the button “click” By Clicking the “none” box.
- Upon Clicking it a popup will let us choose the desired action.
(pic)
- Now Simply Click the Checkmark to save changes, and you should be good to go!

### Advanced Bindings

And you thought you were done?

- First we will start by editing an action, hover over a set action and click the pen
(pic)
- Now you will notice two important things the “more options” and the cog.
- The Cog allows you to set up haptics for activating an action 
(pic)
 - Amplitude - how powerful 0-100% the haptics should be.
 - Duration – how long it will last 0-4 seconds.
 - Frequency – The frequency/”feel” of the vibration
- But What about the “more options?!”
(pic)
 - Single – single click
 - Double – double click (about .25 seconds in between clicks max)
 - Long Hold – Long hold ~1-2 seconds
 - Held – Action active when held!
 - Touch – Active when the button is being touched (knuckles/ touch)
 - And More – Analog inputs (i.e. Triggers/joysticks/ trackpads ) have more options and configurations you can try out!
 
### Chord Bindings
 
 The Combo Maker! Chords are just that, they allow you to combine a combination of Keys to do an action for example a Grip + trigger + trackpad Press.
 
**NOTE:** For you to be able to select a button in the chord menu you need to have clicked the [+] for the button and have an empty slot like below, And the behavior (toggle/Button etc.) must also be correct.
(pic empty action) 

- Make sure you have empty Bindings or a binding for all keys you will want to use in your chord, then click the “Add Chords” followed by “Add New Chord”.
- You will be prompted for an action, select the result of your chord.
- You will Now see the prototype for your chord and can add inputs as you desire
(pic)
- And keep adding if you want to make sure you really don’t accidently press it!
(pic)
- That’s is it add more or close and you are done!

### Odds and Ends.... and Haptics!

-	Haptics are a special Action. as it essentially tells the program which controller this haptic “action” belongs to.
-	The add/edit Haptics button will be  next to the add/edit chord button. [in our app haptics only exist in the “haptics” action set, as such you will want to set them.]
-	**YOU MUST HAVE AN EMPTY BINDING**, otherwise haptics will not be bind-able. Anything will do… even “unused” 
(pic)
-	Other than that just place the appropriate haptic action in the appropriate spot!

### But Wait it’s Broke!

-	We recommend a Separate bind for every action. Multiple actions to one bind can cause un-predictable behavior.
(pic)
-	This Guide Did not cover Legacy Applications or Simulated Actions, as the old input method is deprecated.



