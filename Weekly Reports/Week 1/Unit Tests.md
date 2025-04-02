# Unit Test 1 - Projectile Motion with no Force Applied
This test cuts the boundary between the physics engine and the game state. Instead of sending the updated data to the game state, it would be tested.

The test would create a projectile, then using the physics engine would step through several time slices, ensuring that the projectile would "drop" as expected by gravity, and that it would accelerate as time continued.

This unit test would isolate the gravity system in the physics engine, making sure that it is properly "pulling" objects down.

# Unit Test 2 - Projectile Motion with Force Applied
This test cuts the boundary between the physics engine and the game state. Instead of sending the updated data to the game state, it would be tested.

The test would create a projectile, then using the physics engine would impart a force on it, launching in at an angle and stepping through at various intervals, ensuring that the projectile would follow a parabolic trajectory. 

This unit test would isolate the force system in the physics engine, making sure that the forces do not prevent gravity from working, and that they properly cause the projectile to move.