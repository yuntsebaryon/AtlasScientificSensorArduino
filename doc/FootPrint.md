Foot Print of Electronics
=========================

1. Define the edge cuts
2. Use the "entire" back side for grounding
    - `Add a filled zone`
3. Draw the power lines:
    - Be generous on the power lines
    - In the schematics, select a wire, left click, `add net class directive`, chose the `wide` (Steffen defined it)
4. Select a pin, connect it according to the direction from the PCBeditor
5. If there is an issue to cross another wire, we can add a `via`: in the middle of routing, press `v` key to add a via.  That means the wire on the front side goes through the board and continues in the back side, or vice versa.
